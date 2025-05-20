#include "greedy.hpp"
#include "../heuristics/manhattan.hpp"
#include "../heuristics/blocking_cars.hpp"
#include <queue>
#include <unordered_set>

GreedyBestFirst::GreedyBestFirst(std::string h) : heuristic(h) {}

struct GreedyState {
    Board board;
    std::vector<std::pair<char, std::string>> moves;
    int h_cost;
    
    bool operator>(const GreedyState& other) const {
        return h_cost > other.h_cost;
    }
};

Solution GreedyBestFirst::solve(const Board& initialBoard) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::priority_queue<GreedyState, std::vector<GreedyState>, std::greater<>> openSet;
    std::unordered_set<std::string> visited;
    int nodesVisited = 0;
    
    int initial_h;
    if (heuristic == "manhattan") {
        initial_h = ManhattanHeuristic::calculate(initialBoard);
    } else {
        initial_h = BlockingCarsHeuristic::calculate(initialBoard);
    }
    
    openSet.push({initialBoard, {}, initial_h});
    visited.reserve(10000); 
    
    while (!openSet.empty()) {
        GreedyState current = openSet.top();
        openSet.pop();
        nodesVisited++;
        
        if (current.board.isGoalState()) {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            return {current.moves, nodesVisited, duration.count()};
        }
        
        std::string currentStr = current.board.toString();
        if (visited.find(currentStr) != visited.end()) {
            continue;
        }
        visited.insert(currentStr);
        
        auto possibleMoves = current.board.getPossibleMoves();
        for (const auto& move : possibleMoves) {
            Board nextBoard = current.board;
            if (!nextBoard.makeMove(move.first, move.second)) {
                continue;
            }
            
            std::string nextStr = nextBoard.toString();
            if (visited.find(nextStr) != visited.end()) {
                continue;
            }
            
            int next_h;
            if (heuristic == "manhattan") {
                next_h = ManhattanHeuristic::calculate(nextBoard);
            } else {
                next_h = BlockingCarsHeuristic::calculate(nextBoard);
            }
            
            auto nextMoves = current.moves;
            nextMoves.push_back(move);
            openSet.push({nextBoard, nextMoves, next_h});
        }
    }
    
    return {{}, nodesVisited, 0};
}