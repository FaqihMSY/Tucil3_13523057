#include "board.hpp"
#include <cctype>
#include <iostream>
#include <set>
#include <sstream>
#include <utility>


Board::Board(int rows, int cols, int numPieces): rows(rows), cols(cols), numPieces(numPieces) {
    grid.assign(rows, std::vector<char>(cols, '.'));
}

bool Board::isValidPosition(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

bool Board::isEdge(int r, int c) const {
    return r == 0 || r == rows - 1 || c == 0 ||  c == cols - 1;
}

bool Board::isValidConfiguration(const std::vector<std::string> &cfg) const {
    if (static_cast<int>(cfg.size()) != rows) {
        std::cerr << "Invalid row count. Expected: " << rows << ", Got: " << cfg.size() << "\n";
        return false;
    }
    for (size_t i = 0; i < cfg.size(); ++i) {
        if (static_cast<int>(cfg[i].size()) != cols) {
            std::cerr << "Invalid column count at row " << i << ". Expected: " << cols 
                      << ", Got: " << cfg[i].size() << "\n";
            return false;
        }
    }
    return true;
}

bool Board::validateAlignment() const {
    auto it = cars.find('P');
    if (it == cars.end()) return false;

    const Car& primary = it->second;
    Position p = primary.getPosition();

    if (primary.getIsHorizontal()){
        
        // std::cout<<"hor\n";
        return p.row == exitPos.row;
    }
    else{

        // std::cout<<"nahhor\n";
        return p.col == exitPos.col;
    }
}

bool Board::loadFromString(const std::vector<std::string> &cfg) {
    if (!isValidConfiguration(cfg)) {
        std::cerr << "Invalid configuration dimensions\n" << toString() << "\n";
        return false;
    }

    cars.clear();
    grid.clear();
    for (const auto &line : cfg) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }


    Position pStart{-1,-1};
    bool horiz = true;
    int leng = 0;

    for (int r = 0; r < rows && pStart.row == -1; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 'P') {
                pStart = {r,c};
                break;
            }
        }
    }

    if (pStart.row == -1) {
        std::cerr << "Primary piece (P) not found\n" << toString() << "\n";
        return false;
    }
    if (pStart.col + 1 < cols && grid[pStart.row][pStart.col + 1] == 'P') {
        horiz = true;
        int c = pStart.col;
        while (c < cols && grid[pStart.row][c] == 'P') {
            ++leng;
            ++c;
        }
    } else {
        horiz = false;
        int r = pStart.row;
        while (r < rows && grid[r][pStart.col] == 'P') {
            ++leng;
            ++r;
        }
    }

    if (leng < 2 || leng > 3) {
        std::cerr << "Primary piece has invalid length: " << leng << "\n" << toString() << "\n";
        return false;
    }

    cars.emplace('P', Car('P', pStart, leng, horiz, true));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char id = grid[r][c];
            if (id == '.' || id == 'P' || cars.find(id) != cars.end()) continue;

            bool h = true;
            int l = 1;
            if (c + 1 < cols && grid[r][c + 1] == id) {
                int cc = c + 1;
                while (cc < cols && grid[r][cc] == id) {
                    ++l;
                    ++cc;
                }
            } else {
                h = false;
                int rr = r + 1;
                while (rr < rows && grid[rr][c] == id) {
                    ++l;
                    ++rr;
                }
            }

            if (l < 2 || l > 3) {
                std::cerr << "Car " << id << " has invalid length: " << l << "\n" << toString() << "\n";
                return false;
            }
            cars.emplace(id, Car(id, Position{r,c}, l, h, false));
        }
    }

    if (!validateAlignment()) {
        std::cerr << "Primary piece not aligned with exit\n"
                  << "Primary position: (" << cars['P'].getPosition().row << "," 
                  << cars['P'].getPosition().col << ")\n"
                  << "Exit position: (" << exitPos.row << "," << exitPos.col << ")\n"
                  << toString() << "\n";
        return false;
    }

    return true;
}

char Board::getCell(int r,int c) const { return isValidPosition(r,c)?grid[r][c]:'.'; }

bool Board::isValidMove(const Car& car, const std::string& dir) const {
    Position p = car.getPosition(); 
    int l = car.getLength();
    if (car.getIsHorizontal()) {
        if (dir == "kiri") {
            if (!(p.col > 0 && grid[p.row][p.col-1] == '.')) {
                // std::cerr << "Invalid left move for car at (" << p.row << "," << p.col << ")\n";
                return false;
            }
            return true;
        }
        if (dir == "kanan") {
            if (!(p.col + l < cols && grid[p.row][p.col+l] == '.')) {
                // std::cerr << "Invalid right move for car at (" << p.row << "," << p.col << ")\n";
                return false;
            }
            return true;
        }
    } else {
        if (dir == "atas") {
            if (!(p.row > 0 && grid[p.row-1][p.col] == '.')) {
                // std::cerr << "Invalid up move for car at (" << p.row << "," << p.col << ")\n";
                return false;
            }
            return true;
        }
        if (dir == "bawah") {
            if (!(p.row + l < rows && grid[p.row+l][p.col] == '.')) {
                // std::cerr << "Invalid down move for car at (" << p.row << "," << p.col << ")\n";
                return false;
            }
            return true;
        }
    }
    std::cerr << "Invalid direction: " << dir << "\n";
    return false;
}

bool Board::makeMove(char id, const std::string& dir) {
    auto it = cars.find(id);
    if (it == cars.end() || !isValidMove(it->second, dir)) {
        // std::cerr << "Invalid move: car=" << id << ", direction=" << dir << "\n";
        return false;
    }
    Car &car=it->second; Position old=car.getPosition(); Position neu=old;
    if(dir=="kiri")--neu.col; else if(dir=="kanan")++neu.col; else if(dir=="atas")--neu.row; else ++neu.row;
    for(int i=0;i<car.getLength();++i) grid[car.getIsHorizontal()?old.row:old.row+i][car.getIsHorizontal()?old.col+i:old.col] = '.';
    car = Car(id, neu, car.getLength(), car.getIsHorizontal(), id=='P');
    for(int i=0;i<car.getLength();++i) grid[car.getIsHorizontal()?neu.row:neu.row+i][car.getIsHorizontal()?neu.col+i:neu.col] = id;
    return true;
}

std::vector<std::pair<char,std::string>> Board::getPossibleMoves() const {
    std::vector<std::pair<char,std::string>> mv;
    for (auto &kv:cars) {
        const Car &c=kv.second; 
        if(c.getIsHorizontal()){ 
            if(isValidMove(c,"kiri")) mv.push_back({kv.first,"kiri"}); 
            if(isValidMove(c,"kanan")) mv.push_back({kv.first,"kanan"}); 
        }
        else { 
            if(isValidMove(c,"atas")) mv.push_back({kv.first,"atas"}); 
            if(isValidMove(c,"bawah")) mv.push_back({kv.first,"bawah"}); 
        }
    }
    return mv;
}

bool Board::isGoalState() const {
    auto it = cars.find('P');
    if (it == cars.end()) {
        std::cerr << "Primary piece not found in goal check\n";
        return false;
    }
    const Car&p=it->second; Position pos=p.getPosition();
    if(p.getIsHorizontal()) return pos.row==exitPos.row && ((pos.col+p.getLength()==exitPos.col)||(exitPos.col+1==pos.col));
    return pos.col==exitPos.col && ((pos.row+p.getLength()==exitPos.row)||(exitPos.row+1==pos.row));
}

std::string Board::toString() const {
    std::stringstream ss; 
    for(int r=0;r<rows;++r){ 
        for(int c=0;c<cols;++c) 
        ss<<grid[r][c]; 
        if(r<rows-1) ss<<'\n'; 
    } 
    return ss.str();
}

Car Board::getPrimaryCar() const { 
    auto it=cars.find('P'); 
    return it!=cars.end()?it->second:Car(); 
}
Position Board::getExitPosition() const { return exitPos; }
