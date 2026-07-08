#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstdint>

namespace rcs {

enum class Face : uint8_t { U = 0, R = 1, F = 2, D = 3, L = 4, B = 5 };

enum class Move : uint8_t {
    U, U2, Ui,
    D, D2, Di,
    L, L2, Li,
    R, R2, Ri,
    F, F2, Fi,
    B, B2, Bi,
    COUNT
};

std::string moveToString(Move m);
Move stringToMove(const std::string& s);
Move inverseMove(Move m);
int moveFace(Move m);
std::vector<Move> allMoves();

class Cube {
public:
    Cube();

    static Cube solved();

    void applyMove(Move m);
    void applyMoves(const std::vector<Move>& moves);

    bool isSolved() const;

    std::string serialize() const;
    static Cube deserialize(const std::string& s);

    std::vector<Move> scramble(int length, uint32_t seed);

    int misplacedStickers() const;

    const std::array<std::array<uint8_t, 9>, 6>& faces() const { return faces_; }

    bool operator==(const Cube& other) const { return faces_ == other.faces_; }

private:
    std::array<std::array<uint8_t, 9>, 6> faces_;

    void rotateFaceCW(int face);
    void rotateFaceCCW(int face);
    void cycleEdges(const std::array<std::array<int, 3>, 4>& strips, bool reverse);

    void turnU(bool ccw);
    void turnD(bool ccw);
    void turnL(bool ccw);
    void turnR(bool ccw);
    void turnF(bool ccw);
    void turnB(bool ccw);
};

}
