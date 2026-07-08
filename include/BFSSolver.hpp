#pragma once
#include "Solver.hpp"

namespace rcs {

class BFSSolver : public Solver {
public:
    explicit BFSSolver(int maxDepth) : Solver(maxDepth) {}
    std::string name() const override { return "BFS"; }
    SolveResult solve(const Cube& scrambled) override;
};

}
