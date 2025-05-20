#ifndef OUTPUT_WRITER_HPP
#define OUTPUT_WRITER_HPP

#include "../board/board.hpp"
#include "../algorithms/solver.hpp"
#include <string>
#include <iostream>   

class OutputWriter {
public:
    static void printBoard(const Board&, char,
                           std::ostream& = std::cout,
                           bool colored = true);
    static void printSolution(const Board&, const Solution&,
                              std::ostream& = std::cout,
                              bool colored = true);

private:
    static const std::string RED;
    static const std::string GREEN;
    static const std::string BLUE;
    static const std::string RESET;
};

#endif