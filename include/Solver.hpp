#pragma once

#include "Cube.hpp"
#include <vector>
#include <string>
#include <chrono>

namespace rcs {

struct SolveResult {
    bool found = false;
    std::vector<Move> moves;
    uint64_t statesExplored = 0;
    double elapsedSeconds = 0.0;
};

class Solver {
public:
    explicit Solver(int maxDepth) : maxDepth_(maxDepth) {}
    virtual ~Solver() = default;

    virtual std::string name() const = 0;
    virtual SolveResult solve(const Cube& scrambled) = 0;

protected:
    int maxDepth_;

    template <typename Fn>
    static SolveResult timed(Fn&& fn) {
        auto start = std::chrono::steady_clock::now();
        SolveResult r = fn();
        auto end = std::chrono::steady_clock::now();
        r.elapsedSeconds = std::chrono::duration<double>(end - start).count();
        return r;
    }
};

}
