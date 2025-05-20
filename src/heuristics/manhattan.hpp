#ifndef MANHATTAN_HPP
#define MANHATTAN_HPP

#include "../board/board.hpp"

class ManhattanHeuristic {
public:
    static int calculate(const Board& board);
    static int calculateReversed(const Board& board);
private:
    static int manhattanDistance(const Position& p1, const Position& p2);
    
};

#endif