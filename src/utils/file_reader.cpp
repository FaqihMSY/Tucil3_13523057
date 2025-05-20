#include "file_reader.hpp"
#include "board/board.hpp"

#include <algorithm>
#include <fstream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

static inline void rtrim(string &s) {
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' ||
                          s.back() == '\r' || s.back() == '\n'))
        s.pop_back();
}

Board FileReader::readBoard(const std::string &path) {
    ifstream fin(path);
    if (!fin) throw runtime_error("Tidak bisa membuka " + path);

    int rows{}, cols{}, numPieces{};
    if (!(fin >> rows >> cols >> numPieces))
        throw runtime_error("Header file tidak valid");
    string dummy; getline(fin, dummy);                   

    vector<string> raw;                                  
    Position exitPos{-1, -1};
    bool foundK = false, foundPrimary = false;

    enum class Side { NONE, LEFT, RIGHT, TOP, BOTTOM };
    Side exitSide = Side::NONE;

    int logicalRow = 0;
    string line;
    while (logicalRow <= rows && getline(fin, line)) {
        rtrim(line);
        if (line.empty()) continue;

        size_t kPos = line.find('K');
        if (kPos != string::npos) {                     
            if (foundK) throw runtime_error("Lebih dari satu K");

            foundK = true;
            if (logicalRow == 0) {                       
                exitSide = Side::TOP;
                exitPos   = {-1, static_cast<int>(kPos)};
                ++logicalRow;                            
                continue;                                
            }
            if (logicalRow == rows) {                    
                exitSide = Side::BOTTOM;
                exitPos   = {rows, static_cast<int>(kPos)};
                break;                                   
            }
            if (kPos == 0) {                             
                exitSide = Side::LEFT;
                exitPos   = {logicalRow, -1};
            } else if (kPos + 1 == line.length()) {      
                exitSide = Side::RIGHT;
                exitPos   = {logicalRow, cols};
            } else {
                throw runtime_error("Exit (K) harus di pinggir papan");
            }
            line[kPos] = '.';                            
        }

        raw.push_back(line);
        ++logicalRow;
    }

    if (!foundK) throw runtime_error("Exit (K) tidak ditemukan");
    if (static_cast<int>(raw.size()) != rows)
        throw runtime_error("Jumlah baris papan kurang dari header");

    auto padOrCut = [&](string &row) {
        if (static_cast<int>(row.length()) < cols)
            row.append(cols - row.length(), '.');
        else if (static_cast<int>(row.length()) > cols)
            row = row.substr(0, cols);
    };

    if (exitSide == Side::LEFT || exitSide == Side::RIGHT) {
        for (auto &row : raw) {
            if (exitSide == Side::LEFT && static_cast<int>(row.length()) == cols + 1)
                row.erase(row.begin());                  
            else if (exitSide == Side::RIGHT && static_cast<int>(row.length()) == cols + 1)
                row.pop_back();                          
            padOrCut(row);
        }
    } else { 
        int indent = numeric_limits<int>::max();
        for (auto &row : raw) {
            int first = 0;
            while (first < static_cast<int>(row.size()) && row[first] == ' ') ++first;
            indent = min(indent, first);
        }
        for (auto &row : raw) {
            if (indent > 0 && indent < static_cast<int>(row.size()))
                row.erase(0, indent);
            padOrCut(row);
        }
    }

    set<char> pieces;
    for (auto &row : raw)
        for (char c : row) {
            if (c == 'P') foundPrimary = true;
            else if (c != '.' && (c < 'A' || c > 'Z'))
                throw runtime_error("Karakter tidak valid: " + string(1, c));
            if (c >= 'A' && c <= 'Z' && c != 'P') pieces.insert(c);
        }

    if (!foundPrimary) throw runtime_error("Primary piece (P) tidak ditemukan");
    if (static_cast<int>(pieces.size()) != numPieces)
        throw runtime_error("Jumlah piece tidak cocok header");

    Board board(rows, cols, numPieces);
    board.setExitPosition(exitPos.row, exitPos.col);
    if (!board.loadFromString(raw))
        throw runtime_error("Konfigurasi papan tidak valid");

    return board;
}
