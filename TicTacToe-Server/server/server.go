package server

import (
	"fmt"
	"sync"
	"tictactoeserver/game"

	"github.com/gorilla/websocket"
)

var (
	mu sync.Mutex
)

type Message struct {
	Row int `json:"row"`
	Col int `json:"col"`
}
type MessageToSend struct {
	Board [3][3]string `json:"board"`
	Turn  int          `json:"turn"`
	State string       `json:"state"`
}
type Server struct {
	WaitingPlayers []string
	Games          []*game.Game
	PlayerGameMap  map[string]*game.Game
	Connections    map[string]*websocket.Conn
}

func NewServer() *Server {
	return &Server{
		WaitingPlayers: []string{},
		Games:          []*game.Game{},
		PlayerGameMap:  map[string]*game.Game{},
		Connections:    map[string]*websocket.Conn{},
	}
}

// map 0 to X and 1 to O
var playerMap = map[int]string{
	0: "X",
	1: "O",
}

func Run() {
	// ...
}
func (s *Server) AddPlayer(connID string, conn *websocket.Conn) {
	mu.Lock()
	s.Connections[connID] = conn
	s.WaitingPlayers = append(s.WaitingPlayers, connID)
	if len(s.WaitingPlayers) >= 2 {
		game := game.NewGame(s.WaitingPlayers[0], s.WaitingPlayers[1])
		s.Games = append(s.Games, game)
		s.PlayerGameMap[s.WaitingPlayers[0]] = game
		s.PlayerGameMap[s.WaitingPlayers[1]] = game
		s.WaitingPlayers = s.WaitingPlayers[2:]
		
		mu.Unlock()
		// Let players know that the game has started
		// time.Sleep(800 * time.Millisecond)
		SendGameState(game.Players[0], game.Players[1], s, game)
	} else {
		// Let player know that they are waiting
		var temp MessageToSend
		temp.State = "Waiting for another Player"
		mu.Unlock()
		// time.Sleep(800 * time.Millisecond)
		mu.Lock()
		err := conn.WriteJSON(temp)
		ErrorHandle(err)
		mu.Unlock()
	}

}

func SendGameState(p1, p2 string, s *Server, game *game.Game) {
	conn1 := s.Connections[p1]
	conn2 := s.Connections[p2]
	// Send game state to both players
	var msg1, msg2 MessageToSend
	msg1.Board = game.Board
	msg1.Turn = 1 - game.Turn
	msg1.State = "In Progress"
	msg2.Turn = game.Turn
	msg2.Board = game.Board
	msg2.State = "In Progress"
	// time.Sleep(800 * time.Millisecond)
	fmt.Println("Sending game state to both players")
	// mu.Lock()
	err := conn1.WriteJSON(msg1)
	ErrorHandle(err)
	err = conn2.WriteJSON(msg2)
	ErrorHandle(err)
	// mu.Unlock()
}

func ErrorHandle(err error) {
	if err != nil {
		fmt.Println("Error: ", err)
	}
}

// send win or loose message
func SendGameEnd(p1, p2 string, s *Server, game *game.Game, state1 string, state2 string) {

	fmt.Println("Sending game end message to both players")
	conn1 := s.Connections[p1]
	conn2 := s.Connections[p2]
	// Send game state to both players
	var msg1, msg2 MessageToSend
	msg1.Board = game.Board
	msg1.Turn = 2
	msg1.State = state1
	msg2.Turn = 2
	msg2.Board = game.Board
	msg2.State = state2
	mu.Lock()
	err := conn1.WriteJSON(msg1)
	ErrorHandle(err)
	err = conn2.WriteJSON(msg2)
	ErrorHandle(err)
	mu.Unlock()
	err = conn1.Close()
	ErrorHandle(err)
	err = conn2.Close()
	ErrorHandle(err)
	fmt.Println("Deleting the game and connection")
	delete(s.Connections, p1)
	delete(s.Connections, p2)
	delete(s.PlayerGameMap, p1)
	delete(s.PlayerGameMap, p2)
}
func (s *Server) PlayMove(connID string, msg Message) {
	curgame := s.PlayerGameMap[connID]
	fmt.Println("Playing move for turn by connection id:", curgame.Turn, connID)
	if curgame.Players[curgame.Turn] == connID {
		if curgame.Board[msg.Row][msg.Col] != "" {
			fmt.Println("Not a valid move")
			return
		}
		fmt.Println("Move played by player")
		curgame.Board[msg.Row][msg.Col] = playerMap[curgame.Turn]
	} else {
		fmt.Println("Not your turn")
		return
	}
	// Check if the game is over
	if game.GameWin(curgame.Board, playerMap[curgame.Turn]) {
		SendGameEnd(curgame.Players[curgame.Turn], curgame.Players[1-curgame.Turn], s, curgame, "Win", "Lose")
		return
	}
	if game.GameDraw(curgame.Board) {
		SendGameEnd(curgame.Players[curgame.Turn], curgame.Players[1-curgame.Turn], s, curgame, "Draw", "Draw")
		return
	}
	curgame.Turn = 1 - curgame.Turn
	SendGameState(curgame.Players[0], curgame.Players[1], s, curgame)
}
func (s *Server) RemovePlayer(connID string) {
	if s.PlayerGameMap[connID] != nil {
		game := s.PlayerGameMap[connID]
		otherPlayer := game.Players[0]
		if connID == game.Players[0] {
			otherPlayer = game.Players[1]
		}
		SendGameEnd(connID, otherPlayer, s, game, "Lose", "Win")
	} else {
		flag := 0
		for _, v := range s.WaitingPlayers {
			if v == connID {
				flag = 1
				break
			}
		}
		if flag == 1 {
			fmt.Println("Removing the connection from waiting queue due to lose of connection.")
			s.WaitingPlayers = s.WaitingPlayers[1:]
		}
	}

}
