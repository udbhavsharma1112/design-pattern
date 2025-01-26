#include <bits/stdc++.h>
using namespace std;

enum Side
{
    BLACK,
    WHITE
};

enum Peice
{
    ROOK,
    PAWN,
    KING,
    QUEEN,
    BISHOP,
    KNIGHT
};
class Position
{
public:
    int x, y;
};

class IPeice
{
public:
    Side side;
    Peice peice;
    Position current;
    virtual bool isValid(Position, map<Position, IPeice *>) = 0;

    Position getKingPosition(map<Position, IPeice *> &board)
    {
        Position kingPosition;
        if (this->side == BLACK)
        {

            kingPosition.x = -1;
            kingPosition.y = 0;
        }
        else
        {
            kingPosition.x = 0;
            kingPosition.y = -1;
        }

        return board[kingPosition]->current;
    }
    void UpdateKingPosition(map<Position, IPeice *> &board)
    {
        Position kingPosition;
        if (this->side == BLACK)
        {
            kingPosition.x = -1;
            kingPosition.y = 0;
        }
        else
        {
            kingPosition.x = 0;
            kingPosition.y = -1;
        }
        board[kingPosition] = this;
    }

    bool move(Position target, map<Position, IPeice *> &board)
    {
        if (isValid(target, board) == false)
        {
            return false;
        }

        this->current = target;
        board.erase(target);
        board[target] = this;
        if (peice == KING)
        {
            this->UpdateKingPosition(board);
        }
        return true;
    }

    bool isAttacked(Position target, map<Position, IPeice *> &board)
    {
        for (auto peicePosition : board)
        {
            if ((peicePosition.second)->isValid(target, board) and this->side != (peicePosition.second)->side)
            {
                return true;
            }
        }
        return false;
    }
    bool isChecked(Position target, map<Position, IPeice *> &board)
    {
        IPeice *temp = board[target];
        Position curr = this->current;
        board[target] = this;
        this->current = target;
        bool attacked = false;
        Position KingPosition = this->getKingPosition(board);
        if (peice == KING)
        {
            KingPosition = target;
        }
        if (this->isAttacked(KingPosition, board))
        {
            attacked = true;
        }
        board[target] = temp;
        current = curr;
        return attacked;
    }
};

class Pawn : public IPeice
{
private:
    // rule: can only move 1 step at a time, if any opposition peice is there diagonal by a step,it can also move there
public:
    Pawn(Side side, Position current)
    {
        this->peice = PAWN;
        this->side = side;
        this->current = current;
    }

    bool isValid(Position target, map<Position, IPeice *> &board)
    {
        IPeice *targetPeice = board[target];
        if (targetPeice->side == this->side)
            return false;
        int forwardDx = side == BLACK ? -1 : 1;
        int dx = target.x - this->current.x;
        int dy = target.y - this->current.y;
        if ((dx == forwardDx and dy == 0 and board.find(target) == board.end()) or (abs(dy) == 1 and dx == forwardDx and board.find(target) != board.end()))
        {
            if (this->isChecked(target, board))
                return false;
            return true;
        }

        return false;
    }
};

class Knight : public IPeice
{
private:
    // can move 2 step in any four direction and one step at 90 degree after 2 step;
    // int dx[8] = {2,2,-2,-2,1,1,-1,-1};
    // int dy[8] = {1,-1,1,-1,2,-2,2,-2};
public:
    Peice peice = KNIGHT;
    Side side;
    Position current;
    Knight(Side side, Position current)
    {
        this->side = side;
        this->current = current;
    }

    bool isValid(Position target, map<Position, IPeice *> &board)
    {
        IPeice *targetPeice = board[target];
        if (targetPeice->side == this->side)
            return false;

        int dx = target.x - this->current.x;
        int dy = target.y - this->current.y;
        if ((abs(dx) == 2 and abs(dy) == 1) or (abs(dy) == 2 and abs(dx) == 1))
        {
            if (this->isChecked(target, board))
                return false;
            return true;
        }
        return false;
    }
};

class Bishop : public IPeice
{
public:
    Peice peice = BISHOP;
    Side side;
    Position current;
    Bishop(Side side, Position current)
    {
        this->side = side;
        this->current = current;
    }
    bool isValid(Position target, map<Position, IPeice *> &board)
    {
        IPeice *targetPeice = board[target];
        int dx = target.x - this->current.x;
        int dy = target.y - this->current.y;
        if (targetPeice->side == this->side or abs(dx) != abs(dy))
            return false;

        Position temp = current;
        while (target.x != temp.x + (dx > 0 ? 1 : -1) or target.y != temp.y + (dy > 0 ? 1 : -1))
        {
            temp.x += (dx > 0 ? 1 : -1);
            temp.y += (dy > 0 ? 1 : -1);
            if (board.find(temp) != board.end())
            {
                return false;
            }
        }
        if (this->isChecked(target, board))
            return false;
        return true;
    }
};

class Rook : public IPeice
{
public:
    Peice peice = ROOK;
    Side side;
    Position current;
    Rook(Side side, Position current)
    {
        this->side = side;
        this->current = current;
    }
    bool isValid(Position target, map<Position, IPeice *> &board)
        {
        IPeice *targetPeice = board[target];
        int dx = target.x - this->current.x;
        int dy = target.y - this->current.y;
        if (targetPeice->side == this->side or (dx != 0 and dy != 0))
            return false;

        Position temp = current;
        while (target.x != temp.x + (dx != 0) * (dx > 0 ? 1 : -1) or target.y != temp.y + (dy != 0) * (dy > 0 ? 1 : -1))
        {
            temp.x += (dx != 0) * (dx > 0 ? 1 : -1);
            temp.y += (dy != 0) * (dy > 0 ? 1 : -1);
            if (board.find(temp) != board.end())
            {
                return false;
            }
        }
        if (this->isChecked(target, board))
            return false;
        return true;
    }
};

class Queen : public IPeice
{
    public:
    Peice peice = QUEEN;
    Side side;
    Position current;
    Queen(Side side, Position current)
    {
        this->side = side;
        this->current = current;
    }
    bool isValid(Position target, map<Position, IPeice *> &board)
    {
        IPeice *targetPeice = board[target];
        int dx = target.x - this->current.x;
        int dy = target.y - this->current.y;
        if (targetPeice->side == this->side or (dx != 0 and dy != 0 and abs(dx) != abs(dy)))
            return false;

        if (abs(dx) == abs(dy))
        {
            Position temp = current;
            while (target.x != temp.x + (dx != 0) * (dx > 0 ? 1 : -1) or target.y != temp.y + (dy != 0) * (dy > 0 ? 1 : -1))
            {
                temp.x += (dx != 0) * (dx > 0 ? 1 : -1);
                temp.y += (dy != 0) * (dy > 0 ? 1 : -1);
                if (board.find(temp) != board.end())
                {
                    return false;
                }
            }
        }
        else
        {
            Position temp = current;
            while (target.x != temp.x + (dx > 0 ? 1 : -1) or target.y != temp.y + (dy > 0 ? 1 : -1))
            {
                temp.x += (dx > 0 ? 1 : -1);
                temp.y += (dy > 0 ? 1 : -1);
                if (board.find(temp) != board.end())
                {
                    return false;
                }
            }
        }
        if (this->isChecked(target, board))
            return false;
        return true;
    }
};

class King : public IPeice
{
public:
    Peice peice = KING;
    Side side;
    Position current;
    King(Side side, Position current)
    {
        this->side = side;
        this->current = current;
    }
    bool isValid(Position target, map<Position, IPeice *> &board)
    {
        IPeice *targetPeice = board[target];
        int dx = target.x - this->current.x;
        int dy = target.y - this->current.y;
        if (targetPeice->side == this->side or !(abs(dx) + abs(dy) <= 2 and abs(dx) < 2 and abs(dy) < 2))
            return false;
        if (this->isChecked(target, board))
            return false;
        return true;
    }
};