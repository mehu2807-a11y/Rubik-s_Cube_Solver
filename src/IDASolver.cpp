#include "IDASolver.hpp"
#include <limits>

namespace rcs {

namespace {
constexpr int kFound = -1;
constexpr int kInfinity = std::numeric_limits<int>::max();
}

int IDASolver::search(Cube cube, int g, int bound, Move lastMove, std::vector<Move>& path, uint64_t& explored) {
    explored++;
    int h = pdb_.heuristic(cube);
    int f = g + h;
    if (f > bound) return f;
    if (cube.isSolved()) return kFound;
    if (g >= maxDepth_) return kInfinity;

    int minExceeded = kInfinity;
    for (Move m : allMoves()) {
        if (g > 0 && moveFace(m) == moveFace(lastMove)) continue;
        Cube next = cube;
        next.applyMove(m);
        path.push_back(m);
        int t = search(next, g + 1, bound, m, path, explored);
        if (t == kFound) return kFound;
        if (t < minExceeded) minExceeded = t;
        path.pop_back();
    }
    return minExceeded;
}

SolveResult IDASolver::solve(const Cube& scrambled) {
    return timed([&]() {
        SolveResult result;
        int bound = pdb_.heuristic(scrambled);

        while (bound <= maxDepth_) {
            std::vector<Move> path;
            uint64_t explored = 0;
            int t = search(scrambled, 0, bound, Move::COUNT, path, explored);
            result.statesExplored += explored;
            if (t == kFound) {
                result.found = true;
                result.moves = path;
                return result;
            }
            if (t == kInfinity) break;
            bound = t;
        }
        return result;
    });
}

}
