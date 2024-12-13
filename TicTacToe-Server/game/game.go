package game

type Game struct {
	Board   [3][3]string
	Turn    int
	Players [2]string
}

func NewGame(p1, p2 string) *Game {
	return &Game{
		Board:   [3][3]string{},
		Turn:    0,
		Players: [2]string{p1, p2},
	}
}
func GameWin(board [3][3]string, player string) bool {
	// Check rows
	for i := 0; i < 3; i++ {
		if board[i][0] == player && board[i][1] == player && board[i][2] == player {
			return true
		}
	}
	// Check columns
	for i := 0; i < 3; i++ {
		if board[0][i] == player && board[1][i] == player && board[2][i] == player {
			return true
		}
	}
	// Check diagonals
	if board[0][0] == player && board[1][1] == player && board[2][2] == player {
		return true
	}
	if board[0][2] == player && board[1][1] == player && board[2][0] == player {
		return true
	}
	return false
}
func GameDraw(board [3][3]string) bool {
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			if board[i][j] == "" {
				return false
			}
		}
	}
	return true
}
