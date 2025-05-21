#include "utils/output_writer.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

const std::string OutputWriter::RED   = "\033[31m";
const std::string OutputWriter::GREEN = "\033[32m";
const std::string OutputWriter::BLUE  = "\033[34m";
const std::string OutputWriter::RESET = "\033[0m";

static inline void color(std::ostream &out, bool colored, const std::string &code) {
    if (colored) out << code;
}

void OutputWriter::printBoard(const Board &board,
                              char movedPiece,
                              std::ostream &out,
                              bool colored)
{
    std::istringstream ss(board.toString());
    std::string line;
    while (std::getline(ss, line)) {
        for (char c : line) {
            if (c == 'P') {
                color(out, colored, RED);  out << c;
            } else if (c == 'K') {
                color(out, colored, BLUE); out << c;
            } else if (c == movedPiece) {
                color(out, colored, GREEN); out << c;
            } else {
                out << c;
                continue;                  
            }
            color(out, colored, RESET);
        }
        out << '\n';
    }
}

void OutputWriter::printSolution(const Board &initial,
                                 const Solution &solution,
                                 std::ostream &out,
                                 bool colored)
{
    out << "Papan Awal:\n";
    printBoard(initial, '.', out, colored);
    out << '\n';
    int moveCount = 0;
    if (solution.moves.empty()) {
        out << "Tidak ditemukan solusi!\n";
    } else {
        Board current = initial;
        
        char tempPiece = 'P';
        for (size_t i = 0; i < solution.moves.size(); ++i) {
            const auto &[piece, dir] = solution.moves[i];
            if(tempPiece != piece) {
                moveCount++;
                tempPiece = piece;
                out << "Gerakan " << moveCount << ": " << piece << "-" << dir << "\n";
                current.makeMove(piece, dir);
                printBoard(current, piece, out, colored);
                out << '\n';
            }
        }
    }

    out << "\nStatistik:\n";
    out << "Jumlah node diperiksa: " << solution.nodesVisited << '\n';
    out << "Jumlah langkah: " << moveCount << '\n';
}
