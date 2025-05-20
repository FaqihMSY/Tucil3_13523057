#include "manhattan.hpp"
#include <cstdlib>
#include <algorithm>

int ManhattanHeuristic::calculate(const Board& board) {
    Car primaryCar = board.getPrimaryCar();
    if (primaryCar.getLength() == 0) {
        return INT_MAX;  
    }

    Position exitPos = board.getExitPosition();
    Position carPos = primaryCar.getPosition();

    if (primaryCar.getIsHorizontal()) {
        if (carPos.row != exitPos.row || carPos.col > exitPos.col) {
            return INT_MAX;
        }
        
        int endPos = carPos.col + primaryCar.getLength() - 1;
        return std::max(0, exitPos.col - endPos);
    }
    else {
        if (carPos.col != exitPos.col || carPos.row > exitPos.row) {
            return INT_MAX;
        }
        
        int endPos = carPos.row + primaryCar.getLength() - 1;
        return std::max(0, exitPos.row - endPos);
    }
}

int ManhattanHeuristic::manhattanDistance(const Position& p1, const Position& p2) {
    return std::abs(p1.row - p2.row) + std::abs(p1.col - p2.col);
}

int ManhattanHeuristic::calculateReversed(const Board& board) {
    const Car& primary = board.getPrimaryCar();
    Position pos = primary.getPosition();
    Position exitPos = board.getExitPosition();
    
    if (primary.getIsHorizontal()) {
        return pos.col; 
    } else {
        return std::abs(pos.row - exitPos.row); 
    }
}