#pragma once
#include "Solver.hpp"

namespace rcs {

class IDDFSSolver : public Solver {
public:
    explicit IDDFSSolver(int maxDepth) : Solver(maxDepth) {}
    std::string name() const override { return "IDDFS"; }
    SolveResult solve(const Cube& scrambled) override;

private:
    bool dls(Cube cube, int depth, int limit, Move lastMove, std::vector<Move>& path, uint64_t& explored);
};

}
