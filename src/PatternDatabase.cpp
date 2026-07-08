#include "PatternDatabase.hpp"
#include <queue>
#include <array>

namespace rcs {

namespace {
const std::array<int, 4> kCornerIdx = {0, 2, 6, 8};
}

PatternDatabase::PatternDatabase(int depthCap) : depthCap_(depthCap) {}

std::string PatternDatabase::cornerKey(const Cube& c) {
    std::string key;
    key.reserve(24);
    for (int f = 0; f < 6; ++f)
        for (int idx : kCornerIdx)
            key.push_back(static_cast<char>('0' + c.faces()[f][idx]));
    return key;
}

void PatternDatabase::build() {
    table_.clear();
    Cube start = Cube::solved();
    std::string startKey = cornerKey(start);
    table_[startKey] = 0;

    std::queue<std::pair<Cube, int>> frontier;
    frontier.push({start, 0});

    auto moves = allMoves();

    while (!frontier.empty()) {
        auto [cube, depth] = frontier.front();
        frontier.pop();
        if (depth >= depthCap_) continue;

        for (Move m : moves) {
            Cube next = cube;
            next.applyMove(m);
            std::string key = cornerKey(next);
            auto it = table_.find(key);
            if (it == table_.end()) {
                table_[key] = static_cast<uint8_t>(depth + 1);
                frontier.push({next, depth + 1});
            }
        }
    }
}

int PatternDatabase::heuristic(const Cube& c) const {
    std::string key = cornerKey(c);
    auto it = table_.find(key);
    if (it != table_.end()) return it->second;
    return depthCap_;
}

}
