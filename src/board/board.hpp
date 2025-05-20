#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <map>
#include "car.hpp"

class Board {
public:
    Board(int rows = 0, int cols = 0, int numPieces = 0);
    bool loadFromString(const std::vector<std::string>& cfg);  
    void setExitPosition(int r,int c) { exitPos = {r,c}; }
    bool validateAlignment() const; 
    bool isValidPosition(int row, int col) const;
    bool isValidConfiguration(const std::vector<std::string>& configuration) const;
    bool makeMove(char id,const std::string&dir);
    std::vector<std::pair<char, std::string>> getPossibleMoves() const;
    bool isGoalState() const;
    bool isEdge(int r, int c) const;
    std::string toString() const;
    Car getPrimaryCar() const;
    Position getExitPosition() const;
    char getCell(int row, int col) const; 
    bool isValidMove(const Car&car,const std::string&dir) const;
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getNumPieces() const { return numPieces; }

private:
    int rows;
    int cols;
    int numPieces;
    std::vector<std::vector<char>> grid;
    std::map<char, Car> cars;
    Position exitPos;
};

#endif 