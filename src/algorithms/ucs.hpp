#ifndef UCS_HPP
#define UCS_HPP

#include "solver.hpp"

class UCS : public Solver {
public:
    virtual Solution solve(const Board& initialBoard) override;
};

#endif