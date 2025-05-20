#include "blocking_cars.hpp"
#include <set>
#include <queue>
#include <algorithm>

int BlockingCarsHeuristic::calculate(const Board& board) {
    const Car& primary = board.getPrimaryCar();
    Position pos = primary.getPosition();
    Position exitPos = board.getExitPosition();
    int blocking = 0;

    if (primary.getIsHorizontal()) {
        if (pos.row != exitPos.row) {
            return INT_MAX; 
        }
        int startCol = pos.col + primary.getLength();
        int endCol = exitPos.col;
        for (int col = startCol; col < endCol; col++) {
            char piece = board.getCell(pos.row, col);
            if (piece != '.' && piece != 'P') blocking++;
        }
    } else {
        if (pos.col != exitPos.col) {
            return INT_MAX;
        }
        int startRow = pos.row + primary.getLength();
        int endRow = exitPos.row;
        for (int row = startRow; row < endRow; row++) {
            char piece = board.getCell(row, pos.col);
            if (piece != '.' && piece != 'P') blocking++;
        }
    }

    return blocking;
}

int BlockingCarsHeuristic::calculateReversed(const Board& board) {
    const Car& primary = board.getPrimaryCar();
    Position pos = primary.getPosition();
    Position exitPos = board.getExitPosition();
    int blocking = 0;

    if (exitPos.row == -1) {
        if (primary.getIsHorizontal()) {
            return INT_MAX;
        }
        for (int row = 0; row < pos.row; row++) {
            char piece = board.getCell(row, pos.col);
            if (piece != '.' && piece != 'P') blocking++;
        }
        return blocking;
    }
    
    if (exitPos.col == -1) {
        if (!primary.getIsHorizontal()) {
            return INT_MAX;
        }
        for (int col = 0; col < pos.col; col++) {
            char piece = board.getCell(pos.row, col);
            if (piece != '.' && piece != 'P') blocking++;
        }
        return blocking;
    }

    if (primary.getIsHorizontal()) {
        for (int col = pos.col + primary.getLength(); col < board.getCols(); col++) {
            char piece = board.getCell(pos.row, col);
            if (piece != '.' && piece != 'P') blocking++;
        }
    } else {
        for (int row = pos.row + primary.getLength(); row < board.getRows(); row++) {
            char piece = board.getCell(row, pos.col);
            if (piece != '.' && piece != 'P') blocking++;
        }
    }
    
    return blocking;
}