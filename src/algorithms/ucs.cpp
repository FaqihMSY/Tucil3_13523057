#include "ucs.hpp"
#include <queue>
#include <unordered_set>
#include <unordered_map>

struct UCSState {
    Board board;
    std::vector<std::pair<char, std::string>> moves;
    int cost;
    
    bool operator>(const UCSState& other) const {
        return cost > other.cost;
    }
};

Solution UCS::solve(const Board& initialBoard) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::priority_queue<UCSState, std::vector<UCSState>, std::greater<>> pq;
    std::unordered_map<std::string, int> bestCost;
    int nodesVisited = 0;
    
    std::string initialStr = initialBoard.toString();
    pq.push({initialBoard, {}, 0});
    bestCost[initialStr] = 0;
    
    while (!pq.empty()) {
        UCSState current = pq.top();
        pq.pop();
        nodesVisited++;
        
        std::string currentStr = current.board.toString();
        
        if (current.cost > bestCost[currentStr]) {
            continue;
        }
        
        if (current.board.isGoalState()) {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            return {current.moves, nodesVisited, duration.count()};
        }
        
        for (const auto& [piece, direction] : current.board.getPossibleMoves()) {
            Board nextBoard = current.board;
            if (!nextBoard.makeMove(piece, direction)) {
                continue;
            }
            
            std::string nextStr = nextBoard.toString();
            int nextCost = current.cost + 1;
            
            if (bestCost.find(nextStr) != bestCost.end() && 
                nextCost >= bestCost[nextStr]) {
                continue;
            }
            
            bestCost[nextStr] = nextCost;
            
            auto nextMoves = current.moves;
            nextMoves.push_back({piece, direction});
            pq.push({nextBoard, nextMoves, nextCost});
        }
    }
    
    return {{}, nodesVisited, 0}; 
}