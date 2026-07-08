#include "Cube.hpp"
#include <iostream>
#include <cassert>

using namespace rcs;

int main() {
    std::vector<Move> all = {
        Move::U, Move::U2, Move::Ui, Move::D, Move::D2, Move::Di,
        Move::L, Move::L2, Move::Li, Move::R, Move::R2, Move::Ri,
        Move::F, Move::F2, Move::Fi, Move::B, Move::B2, Move::Bi
    };

    for (Move m : all) {
        Cube c = Cube::solved();
        int order = (m == Move::U2 || m == Move::D2 || m == Move::L2 ||
                     m == Move::R2 || m == Move::F2 || m == Move::B2) ? 2 : 4;
        for (int i = 0; i < order; ++i) c.applyMove(m);
        if (!c.isSolved()) {
            std::cerr << "FAIL order test: " << moveToString(m) << "\n";
            return 1;
        }
    }
    std::cout << "order test passed\n";

    for (Move m : all) {
        Cube c = Cube::solved();
        c.applyMove(m);
        c.applyMove(inverseMove(m));
        if (!c.isSolved()) {
            std::cerr << "FAIL inverse test: " << moveToString(m) << "\n";
            return 1;
        }
    }
    std::cout << "inverse test passed\n";

    Cube c = Cube::solved();
    auto scramble = c.scramble(25, 42);
    assert(!c.isSolved());
    for (auto it = scramble.rbegin(); it != scramble.rend(); ++it)
        c.applyMove(inverseMove(*it));
    if (!c.isSolved()) {
        std::cerr << "FAIL scramble/unscramble round trip\n";
        return 1;
    }
    std::cout << "round trip test passed\n";

    Cube s1 = Cube::solved();
    s1.applyMove(Move::U);
    Cube s2 = Cube::deserialize(s1.serialize());
    if (!(s1 == s2)) {
        std::cerr << "FAIL serialize round trip\n";
        return 1;
    }
    std::cout << "serialize test passed\n";

    for (int i = 0; i < 6; ++i) {
        Cube face_c = Cube::solved();
        int counts[6] = {0,0,0,0,0,0};
        auto sc = face_c.scramble(15, 100 + i);
        for (int f = 0; f < 6; ++f)
            for (int k = 0; k < 9; ++k)
                counts[face_c.faces()[f][k]]++;
        for (int k = 0; k < 6; ++k) {
            if (counts[k] != 9) {
                std::cerr << "FAIL color conservation\n";
                return 1;
            }
        }
    }
    std::cout << "color conservation test passed\n";

    std::cout << "ALL TESTS PASSED\n";
    return 0;
}
