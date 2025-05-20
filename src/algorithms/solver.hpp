#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "../board/board.hpp"
#include <vector>
#include <chrono>
#include <utility>

struct Solution {
    std::vector<std::pair<char, std::string>> moves;
    int nodesVisited;
    long long executionTime;
};

class Solver {
public:
    virtual Solution solve(const Board& initialBoard) = 0;
    virtual ~Solver() = default;
};

#endif