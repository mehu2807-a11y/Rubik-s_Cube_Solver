#include "IDDFSSolver.hpp"

namespace rcs {

bool IDDFSSolver::dls(Cube cube, int depth, int limit, Move lastMove, std::vector<Move>& path, uint64_t& explored) {
    explored++;
    if (cube.isSolved()) return true;
    if (depth >= limit) return false;

    for (Move m : allMoves()) {
        if (depth > 0 && moveFace(m) == moveFace(lastMove)) continue;
        Cube next = cube;
        next.applyMove(m);
        path.push_back(m);
        if (dls(next, depth + 1, limit, m, path, explored)) return true;
        path.pop_back();
    }
    return false;
}

SolveResult IDDFSSolver::solve(const Cube& scrambled) {
    return timed([&]() {
        SolveResult result;
        for (int limit = 0; limit <= maxDepth_; ++limit) {
            std::vector<Move> path;
            uint64_t explored = 0;
            bool found = dls(scrambled, 0, limit, Move::COUNT, path, explored);
            result.statesExplored += explored;
            if (found) {
                result.found = true;
                result.moves = path;
                return result;
            }
        }
        return result;
    });
}

}
