#include <bits/stdc++.h>
using namespace std;

enum Side
{
    BLACK,
    WHITE
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
    virtual bool isValid(Position) = 0;
    virtual bool move() = 0;
};

class Pawn : public IPeice
{
private:
    // rule: can only move 1 step at a time, if any opposition peice is there diagonal by a step,it can also move there
public:
    Side side;
    Position current;
    Pawn(Side side, Position current)
    {
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
        if (dx == forwardDx and dy == 0 and board.find(target) == board.end())
        {
            return true;
        }
        if (abs(dy) == 1 and dx == forwardDx and board.find(target) != board.end())
        {
            return true;
        }
        return false;
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

        return true;
    }
};

class Bishop : public IPeice{
    private:
        // can move 2 step in any four direction and one step at 90 degree after 2 step;
    public:
        

};