#ifndef BLOCKING_CARS_HPP
#define BLOCKING_CARS_HPP

#include "../board/board.hpp"

class BlockingCarsHeuristic {
public:
    static int calculate(const Board& board);
    static int calculateReversed(const Board& board);
};

#endif