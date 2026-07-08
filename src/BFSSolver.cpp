#include "BFSSolver.hpp"
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

namespace rcs {

SolveResult BFSSolver::solve(const Cube& scrambled) {
    return timed([&]() {
        SolveResult result;

        if (scrambled.isSolved()) {
            result.found = true;
            return result;
        }

        auto moves = allMoves();
        std::unordered_map<std::string, std::pair<std::string, Move>> parent;
        std::unordered_set<std::string> visited;

        std::string startKey = scrambled.serialize();
        visited.insert(startKey);

        std::queue<std::tuple<Cube, int, Move>> frontier;
        frontier.push({scrambled, 0, Move::COUNT});

        while (!frontier.empty()) {
            auto [cube, depth, lastMove] = frontier.front();
            frontier.pop();
            result.statesExplored++;

            if (depth >= maxDepth_) continue;

            for (Move m : moves) {
                if (depth > 0 && moveFace(m) == moveFace(lastMove)) continue;
                Cube next = cube;
                next.applyMove(m);
                std::string key = next.serialize();
                if (visited.count(key)) continue;
                visited.insert(key);
                parent[key] = {cube.serialize(), m};

                if (next.isSolved()) {
                    std::vector<Move> path;
                    std::string cur = key;
                    while (cur != startKey) {
                        auto& [prevKey, mv] = parent[cur];
                        path.push_back(mv);
                        cur = prevKey;
                    }
                    std::reverse(path.begin(), path.end());
                    result.found = true;
                    result.moves = path;
                    return result;
                }

                frontier.push({next, depth + 1, m});
            }
        }

        return result;
    });
}

}
