#include "astar.hpp"
#include "../heuristics/manhattan.hpp"
#include "../heuristics/blocking_cars.hpp"
#include <queue>
#include <unordered_map>
#include <unordered_set>

AStar::AStar(std::string h) : heuristic(h) {}

struct AStarState {
    Board board;
    std::vector<std::pair<char, std::string>> moves;
    int g_cost;
    int h_cost;
    
    int f_cost() const { return g_cost + h_cost; }
    
    bool operator>(const AStarState& other) const {
        if (f_cost() == other.f_cost()) {
            return h_cost > other.h_cost;  
        }
        return f_cost() > other.f_cost();
    }
};

Solution AStar::solve(const Board& initialBoard) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    std::priority_queue<AStarState, std::vector<AStarState>, std::greater<>> openSet;
    std::unordered_map<std::string, int> gScore;
    std::unordered_set<std::string> closedSet;
    int nodesVisited = 0;
    
    Position exitPos = initialBoard.getExitPosition();
    bool exitOnTopOrLeft = exitPos.col == -1 || exitPos.row == -1;
    int initial_h = 0;
    
    if (heuristic != "none") {
        if (exitOnTopOrLeft) {
            initial_h = (heuristic == "manhattan") ? 
                ManhattanHeuristic::calculateReversed(initialBoard) : 
                BlockingCarsHeuristic::calculateReversed(initialBoard);
        } else {
            initial_h = (heuristic == "manhattan") ? 
                ManhattanHeuristic::calculate(initialBoard) : 
                BlockingCarsHeuristic::calculate(initialBoard);
        }
        
        if (initial_h == INT_MAX) {
            return {{}, nodesVisited, 0};
        }
    }
    
    std::string initialStr = initialBoard.toString();
    gScore[initialStr] = 0;
    openSet.push({initialBoard, {}, 0, initial_h});
    
    while (!openSet.empty()) {
        AStarState current = openSet.top();
        openSet.pop();
        nodesVisited++;
        
        if (current.board.isGoalState()) {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            return {current.moves, nodesVisited, duration.count()};
        }
        
        std::string currentStr = current.board.toString();
        if (closedSet.count(currentStr)) continue;
        closedSet.insert(currentStr);
        for (const auto& [piece, direction] : current.board.getPossibleMoves()) {
            Board nextBoard = current.board;
            if (!nextBoard.makeMove(piece, direction)) continue;
            
            std::string nextStr = nextBoard.toString();
            if (closedSet.count(nextStr)) continue;
            
            int tentative_g = current.g_cost + 1;
            if (gScore.count(nextStr) && tentative_g >= gScore[nextStr]) continue;
            
            int next_h = 0;
            if (heuristic != "none") {
                if (exitOnTopOrLeft) {
                    next_h = (heuristic == "manhattan") ? 
                        ManhattanHeuristic::calculateReversed(nextBoard) : 
                        BlockingCarsHeuristic::calculateReversed(nextBoard);
                } else {
                    next_h = (heuristic == "manhattan") ? 
                        ManhattanHeuristic::calculate(nextBoard) : 
                        BlockingCarsHeuristic::calculate(nextBoard);
                }
                
                if (next_h == INT_MAX) {
                    continue;
                }
            }
            
            auto nextMoves = current.moves;
            nextMoves.push_back({piece, direction});
            gScore[nextStr] = tentative_g;
            openSet.push({nextBoard, nextMoves, tentative_g, next_h});
        }
    }
    
    return {{}, nodesVisited, 0};
}