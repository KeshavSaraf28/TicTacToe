package main

import (
	"fmt"
	"log"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

func main() {
	var wg sync.WaitGroup
	numClients := 301 // Number of concurrent WebSocket connections (reduced for easier debugging)
	conns := make([]*websocket.Conn, numClients)

	wg.Add(numClients)
	for i := 0; i < numClients; i++ {
		go func(i int) {
			defer wg.Done()

			// Connect to the WebSocket server
			conn, _, err := websocket.DefaultDialer.Dial("ws://localhost:8080/ws", nil)
			if err != nil {
				log.Printf("Client %d: Connection failed: %v\n", i, err)
				return
			}

			// Store the connection
			conns[i] = conn

			// Create a timer for 5 seconds
			timer := time.NewTimer(5 * time.Second)
			fmt.Printf("Client %d: Connection established, will close after 5 seconds\n", i)

			for {
				// Set read deadline to 5 seconds to give ample time for message reception
				conn.SetReadDeadline(time.Now().Add(5 * time.Second))

				select {
				case <-timer.C:
					fmt.Printf("Client %d: Timer expired, closing connection\n", i)
					return
				default:
					_, message, err := conn.ReadMessage()
					if err != nil {
						if websocket.IsCloseError(err, websocket.CloseNormalClosure) {
							fmt.Printf("Client %d: Connection closed normally\n", i)
						} else if websocket.IsUnexpectedCloseError(err) {
							fmt.Printf("Client %d: Unexpected close error: %v\n", i, err)
						} else if netErr, ok := err.(interface{ Timeout() bool }); ok && netErr.Timeout() {
							// Read timeout occurred
							fmt.Printf("Client %d: Read timeout, continuing\n", i)
							continue
						} else {
							fmt.Printf("Client %d: Read error: %v\n", i, err)
						}
						return
					}
					fmt.Printf("Client %d received: %s\n", i, message)
				}
			}
		}(i)
	}

	wg.Wait()

	// Close all WebSocket connections after all clients have finished
	for _, conn := range conns {
		if conn != nil {
			conn.Close()
		}
	}

	fmt.Println("All WebSocket clients completed and connections closed")
}
