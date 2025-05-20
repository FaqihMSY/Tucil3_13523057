#ifndef GREEDY_HPP
#define GREEDY_HPP

#include "solver.hpp"
#include <string>

class GreedyBestFirst : public Solver {
public:
    GreedyBestFirst(std::string heuristic = "none"); 
    virtual Solution solve(const Board& initialBoard) override;
    
private:
    std::string heuristic;
};

#endif