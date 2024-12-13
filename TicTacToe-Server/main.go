package main

import (
	"fmt"
	"net/http"
	"sync"
	"tictactoeserver/server"
	"time"

	"github.com/google/uuid"
	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,
}
var (
	mu sync.Mutex
)

func wsHandler(s *server.Server) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {

		conn, err := upgrader.Upgrade(w, r, nil)
		if err != nil {
			http.Error(w, "Failed to upgrade to websocket", http.StatusInternalServerError)
			return
		}
		connID := uuid.New().String()
		fmt.Println("New connection Recieved with connection ID: ", connID)
		time.Sleep(100 * time.Millisecond)
		s.AddPlayer(connID, conn)
		// read mesage from connections and play their moves in game
		for {
			var msg server.Message
			err := conn.ReadJSON(&msg)
			if err != nil {
				if websocket.IsUnexpectedCloseError(err, websocket.CloseGoingAway, websocket.CloseAbnormalClosure) {
					fmt.Printf("Unexpected close error: %v\n", err)
				} else {
					fmt.Printf("Error reading JSON: %v\n", err)
				}
				mu.Lock()
				s.RemovePlayer(connID)
				mu.Unlock()
				break
			}
			fmt.Println("Message Recieved:", msg, "from connection ID:", connID)
			if s.PlayerGameMap[connID] != nil {
				s.PlayMove(connID, msg)
			} else {
				fmt.Println("Game not found for connection ID: ", connID)
			}
		}
	})
}

func main() {
	s := server.NewServer()
	fmt.Println("Server is running on port 8080")
	http.Handle("/ws", wsHandler(s))
	http.ListenAndServe(":8080", nil)
}
