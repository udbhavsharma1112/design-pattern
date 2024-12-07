#include <bits/stdc++.h>
using namespace std;
enum symbols
{
    X,
    O,
    NONE
};

class player
{
public:
    string name;
    symbols symbol;

    player(string name, symbols symbol)
    {
        this->name = name;
        this->symbol = symbol;
    }
    symbols getSymbol()
    {
        return symbol;
    }
};

class board
{
    vector<vector<symbols>> playingBoard;
    int size;
    int attemptsCount;
    bool inrange(int row, int col)
    {
        if (row < 0 || col < 0 || row >= size || col >= size)
            return 0;
        return 1;
    }

public:
    board(int size)
    {
        this->size = size;
        this->playingBoard.assign(size, vector<symbols>(size, NONE));
        attemptsCount = size * size;
    }
    bool isValidMove(int row, int col)
    {
        if (!inrange(row, col) || playingBoard[row][col] != NONE)
            return 0;
        return 1;
    }

    void printBoard()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (playingBoard[i][j] != NONE)
                {
                    cout << playingBoard[i][j] << " ";
                }
                else
                {
                    cout << "_ ";
                }
                cout << " | ";
            }
            cout << endl;
        }
    }

    void updateBoard(int row, int col, symbols symbol)
    {
        playingBoard[row][col] = symbol;
        attemptsCount--;
        printBoard();
    }

    bool isAnyWinner(int row, int col, symbols symbol)
    {
        bool checkRow = 1, checkCol = 1, checkDiagonal1 = 1, checkDiagonal2 = 1;
        for (int i = 0; i < size; i++)
        {
            if (playingBoard[i][col] != symbol)
            {
                checkRow = 0;
                break;
            }
        }
        for (int i = 0; i < size; i++)
        {
            if (playingBoard[row][i] != symbol)
            {
                checkCol = 0;
                break;
            }
        }

        for (int i = 0, j = 0; i < size; i++, j++)
        {
            if (playingBoard[i][j] != symbol)
            {
                checkDiagonal1 = 0;
                break;
            }
        }
        for (int i = 0, j = size - 1; i < size; i++, j--)
        {
            if (playingBoard[i][j] != symbol)
            {
                checkDiagonal2 = 0;
                break;
            }
        }
        return checkRow || checkCol || checkDiagonal1 || checkDiagonal2;
    }

    bool getAttemtedCount()
    {
        return attemptsCount;
    }
};

class ticTacToe
{
    deque<player *> players;
    int sizeOfBoard;
    player *player1;
    player *player2;
    board *playingBoard;

public:
    ticTacToe(int sizeOfBoard, symbols symbol1, symbols symbol2)
    {
        playingBoard = new board(sizeOfBoard);
        string player1Name;
        string player2Name;
        cout << "Enter Players name: ";
        cin >> player1Name >> player2Name;
        cout << endl;
        cout << "Welcome " << player1Name << " " << player2Name << endl;
        player1 = new player(player1Name, symbol1);
        player2 = new player(player2Name, symbol2);
        players.push_back(player1);
        players.push_back(player2);
    }

    void startGame()
    {
        int isGameEnded = false;
        while (!isGameEnded)
        {
            player *currentPlayer = players.front();
            players.pop_front();
            cout << currentPlayer->name << "'s turn (row col)" << endl;
            int row, col;
            cin >> row >> col;
            bool isValidMove = playingBoard->isValidMove(row, col);
            if (!isValidMove)
            {
                cout << "it's inValidMove " << currentPlayer->name << ", try again!!" << endl;
                players.push_front(currentPlayer);
                continue;
            }
            playingBoard->updateBoard(row, col, currentPlayer->symbol);
            players.push_back(currentPlayer);
            bool isAnyWinner = playingBoard->isAnyWinner(row, col, currentPlayer->symbol);
            if (isAnyWinner)
            {
                cout << "Congratulation " << currentPlayer->name << "!!! you won this game" << endl;
                isGameEnded = 1;
            }
            else if (playingBoard->getAttemtedCount() == 0)
            {
                cout << "looks like a tie" << endl;
                isGameEnded = 1;
            }
        }
    }
};

int main()
{

    ticTacToe *ticTacToeGame = new ticTacToe(3, X, O);
    ticTacToeGame->startGame();
}