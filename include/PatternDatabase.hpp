#pragma once

#include "Cube.hpp"
#include <unordered_map>
#include <string>
#include <cstdint>

namespace rcs {

class PatternDatabase {
public:
    explicit PatternDatabase(int depthCap);

    void build();

    int heuristic(const Cube& c) const;

    size_t size() const { return table_.size(); }
    int depthCap() const { return depthCap_; }

private:
    int depthCap_;
    std::unordered_map<std::string, uint8_t> table_;

    static std::string cornerKey(const Cube& c);
};

}
