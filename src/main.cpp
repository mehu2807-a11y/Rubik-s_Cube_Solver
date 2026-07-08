#include "Cube.hpp"
#include "BFSSolver.hpp"
#include "DFSSolver.hpp"
#include "IDDFSSolver.hpp"
#include "IDASolver.hpp"
#include "PatternDatabase.hpp"

#include <iostream>
#include <iomanip>

using namespace rcs;

namespace {

void printResult(const std::string& label, const SolveResult& r) {
    std::cout << std::left << std::setw(8) << label
              << " | found=" << (r.found ? "yes" : "no ")
              << " | moves=" << std::setw(3) << r.moves.size()
              << " | states=" << std::setw(10) << r.statesExplored
              << " | time=" << std::fixed << std::setprecision(3) << r.elapsedSeconds << "s";
    if (r.found) {
        std::cout << " | ";
        for (Move m : r.moves) std::cout << moveToString(m) << " ";
    }
    std::cout << "\n";
}

void blindSearchBattery(int scrambleLen, int trials) {
    std::cout << "\n=== Uninformed search: " << scrambleLen << "-move scrambles (" << trials << " trials) ===\n";
    for (int t = 0; t < trials; ++t) {
        Cube cube = Cube::solved();
        auto scramble = cube.scramble(scrambleLen, 1000 + t);

        std::cout << "\ntrial " << (t + 1) << " scramble: ";
        for (Move m : scramble) std::cout << moveToString(m) << " ";
        std::cout << "\n";

        BFSSolver bfs(scrambleLen);
        DFSSolver dfs(scrambleLen);
        IDDFSSolver iddfs(scrambleLen);

        printResult(bfs.name(), bfs.solve(cube));
        printResult(dfs.name(), dfs.solve(cube));
        printResult(iddfs.name(), iddfs.solve(cube));
    }
}

void informedSearchBattery(int scrambleLen, int trials, const PatternDatabase& pdb) {
    std::cout << "\n=== IDA* with pattern-database heuristic: " << scrambleLen
              << "-move scrambles (" << trials << " trials) ===\n";
    for (int t = 0; t < trials; ++t) {
        Cube cube = Cube::solved();
        auto scramble = cube.scramble(scrambleLen, 2000 + t);

        std::cout << "\ntrial " << (t + 1) << " scramble: ";
        for (Move m : scramble) std::cout << moveToString(m) << " ";
        std::cout << "\n";

        IDASolver ida(scrambleLen + 4, pdb);
        printResult(ida.name(), ida.solve(cube));
    }
}

}

int main(int argc, char** argv) {
    int pdbDepth = argc > 1 ? std::atoi(argv[1]) : 5;
    int shallowLen = argc > 2 ? std::atoi(argv[2]) : 6;
    int deepLen = argc > 3 ? std::atoi(argv[3]) : 10;

    std::cout << "Building corner pattern database (depth cap " << pdbDepth << ")...\n";
    PatternDatabase pdb(pdbDepth);
    pdb.build();
    std::cout << "Pattern database built: " << pdb.size() << " entries\n";

    blindSearchBattery(shallowLen, 2);
    informedSearchBattery(deepLen, 2, pdb);

    return 0;
}
