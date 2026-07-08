#pragma once
#include "Solver.hpp"
#include "PatternDatabase.hpp"

namespace rcs {

class IDASolver : public Solver {
public:
    IDASolver(int maxDepth, const PatternDatabase& pdb) : Solver(maxDepth), pdb_(pdb) {}
    std::string name() const override { return "IDA*"; }
    SolveResult solve(const Cube& scrambled) override;

private:
    const PatternDatabase& pdb_;
    int search(Cube cube, int g, int bound, Move lastMove, std::vector<Move>& path, uint64_t& explored);
};

}
