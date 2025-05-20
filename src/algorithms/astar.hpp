#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "solver.hpp"
#include <string>

class AStar : public Solver {
public:
    AStar(std::string heuristic = "none");  
    virtual Solution solve(const Board& initialBoard) override;
    
private:
    std::string heuristic;
};

#endif