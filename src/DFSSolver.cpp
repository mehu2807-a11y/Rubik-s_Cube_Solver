#include "DFSSolver.hpp"

namespace rcs {

bool DFSSolver::dfs(Cube cube, int depth, Move lastMove, std::vector<Move>& path, uint64_t& explored) {
    explored++;
    if (cube.isSolved()) return true;
    if (depth >= maxDepth_) return false;

    for (Move m : allMoves()) {
        if (depth > 0 && moveFace(m) == moveFace(lastMove)) continue;
        Cube next = cube;
        next.applyMove(m);
        path.push_back(m);
        if (dfs(next, depth + 1, m, path, explored)) return true;
        path.pop_back();
    }
    return false;
}

SolveResult DFSSolver::solve(const Cube& scrambled) {
    return timed([&]() {
        SolveResult result;
        std::vector<Move> path;
        result.found = dfs(scrambled, 0, Move::COUNT, path, result.statesExplored);
        if (result.found) result.moves = path;
        return result;
    });
}

}
