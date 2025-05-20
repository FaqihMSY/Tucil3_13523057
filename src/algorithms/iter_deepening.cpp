#include "iter_deepening.hpp"
#include <chrono>
#include <unordered_set>
#include <algorithm>

IterativeDeepening::IterativeDeepening() {}

struct IDState {
    Board board;
    std::vector<std::pair<char, std::string>> moves;
    int depth;
    
    bool operator==(const IDState& other) const {
        return board.toString() == other.board.toString();
    }
};

Solution IterativeDeepening::solve(const Board& initialBoard) {
    auto startTime = std::chrono::high_resolution_clock::now();
    int maxDepth = 1;
    int nodesVisited = 0;
    
    Position exitPos = initialBoard.getExitPosition();
    Car primaryCar = initialBoard.getPrimaryCar();
    bool isHorizontal = primaryCar.getIsHorizontal();
    
    while (maxDepth <= MAX_DEPTH) {
        std::unordered_set<std::string> visited;
        visited.reserve(10000);
        
        IDState initial{initialBoard, {}, 0};
        auto result = dfs(initial, 0, maxDepth, visited, nodesVisited, exitPos, isHorizontal);
        
        if (!result.moves.empty()) {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            return {result.moves, nodesVisited, duration.count()};
        }
        
        if (nodesVisited > MAX_NODES) {
            break; 
        }
        
        maxDepth++;
    }
    
    return {{}, nodesVisited, 0};
}


Solution IterativeDeepening::dfs(
    const IDState& state,
    int depth,
    int maxDepth,
    std::unordered_set<std::string>& visited,
    int& nodesVisited,
    const Position& exitPos,
    bool isHorizontal
) {
    if (depth > maxDepth) return {{}, nodesVisited, 0};
    
    nodesVisited++;
    std::string stateStr = state.board.toString();
    
    if (state.board.isGoalState()) {
        return {state.moves, nodesVisited, 0};
    }
    
    if (visited.count(stateStr)) {
        return {{}, nodesVisited, 0};
    }
    visited.insert(stateStr);
    
    auto moves = state.board.getPossibleMoves();
    Car primary = state.board.getPrimaryCar();
    Position primPos = primary.getPosition();
    
    std::sort(moves.begin(), moves.end(), 
    [&](const auto& a, const auto& b) {
        if ((a.first == 'P') != (b.first == 'P')) {
            return a.first == 'P';
        }
        
        if (a.first == 'P') {
            if (isHorizontal) {
                return (exitPos.col > primPos.col) ? 
                       (a.second == "kanan") : (a.second == "kiri");
            } else {
                return (exitPos.row > primPos.row) ? 
                       (a.second == "bawah") : (a.second == "atas");
            }
        }
        
        Position piecePos;
        for (int row = 0; row < state.board.getRows(); row++) {
            for (int col = 0; col < state.board.getCols(); col++) {
                if (state.board.getCell(row, col) == a.first) {
                    piecePos = {row, col};
                    break;
                }
            }
        }
        
        if (isPieceBlocking(state.board, a.first, exitPos)) {
            if (isHorizontal) {
                return a.second == (piecePos.row > primPos.row ? "bawah" : "atas");
            } else {
                return a.second == (piecePos.col > primPos.col ? "kanan" : "kiri");
            }
        }
        
        return false;
    });

    
    for (const auto& [piece, direction] : moves) {
        if (depth > maxDepth/2 && piece != 'P' && 
            !isPieceBlocking(state.board, piece, exitPos)) {
            continue;
        }
        
        Board nextBoard = state.board;
        if (!nextBoard.makeMove(piece, direction)) continue;
        
        auto nextMoves = state.moves;
        nextMoves.push_back({piece, direction});
        
        IDState nextState{nextBoard, nextMoves, depth + 1};
        auto result = dfs(nextState, depth + 1, maxDepth, visited, nodesVisited, exitPos, isHorizontal);
        
        if (!result.moves.empty()) return result;
        
        if (nodesVisited > MAX_NODES) break;
    }
    
    return {{}, nodesVisited, 0};
}


bool IterativeDeepening::isPieceBlocking(const Board& board, char piece, const Position& exitPos) {
    Car primary = board.getPrimaryCar();
    Position primPos = primary.getPosition();
    int length = primary.getLength();
    
    if (primary.getIsHorizontal()) {
        if (primPos.row != exitPos.row) return false;
        int startCol = primPos.col + length;
        int endCol = exitPos.col;
        
        for (int col = startCol; col < endCol; col++) {
            if (board.getCell(primPos.row, col) == piece) {
                return true;
            }
        }
    } else {
        if (primPos.col != exitPos.col) return false;
        int startRow = primPos.row + length;
        int endRow = exitPos.row;
        
        for (int row = startRow; row < endRow; row++) {
            if (board.getCell(row, primPos.col) == piece) {
                return true;
            }
        }
    }
    
    return false;
}