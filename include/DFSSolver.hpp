#pragma once
#include "Solver.hpp"

namespace rcs {

class DFSSolver : public Solver {
public:
    explicit DFSSolver(int maxDepth) : Solver(maxDepth) {}
    std::string name() const override { return "DFS"; }
    SolveResult solve(const Cube& scrambled) override;

private:
    bool dfs(Cube cube, int depth, Move lastMove, std::vector<Move>& path, uint64_t& explored);
};

}
