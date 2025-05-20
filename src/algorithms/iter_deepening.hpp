#ifndef ITER_DEEPENING_HPP
#define ITER_DEEPENING_HPP

#include "solver.hpp"
#include "../board/board.hpp"
#include <vector>
#include <string>
#include <unordered_set>

class IterativeDeepening : public Solver {
public:
    IterativeDeepening();
    Solution solve(const Board& initialBoard) override;

private:
    struct IDState {
        Board board;
        std::vector<std::pair<char, std::string>> moves;
        int depth;
        
        bool operator==(const IDState& other) const {
            return board.toString() == other.board.toString();
        }
    };

    Solution dfs(
        const IDState& state,
        int depth,
        int maxDepth,
        std::unordered_set<std::string>& visited,
        int& nodesVisited,
        const Position& exitPos,
        bool isHorizontal
    );
    
    bool isPieceBlocking(const Board& board, char piece, const Position& exitPos);
    
    static constexpr int MAX_DEPTH = 50;
    static constexpr int MAX_NODES = 1000000;
};

#endif 