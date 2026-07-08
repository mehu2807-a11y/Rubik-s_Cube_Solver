#include "Cube.hpp"
#include <stdexcept>
#include <random>
#include <unordered_map>

namespace rcs {

namespace {

struct Sticker { int face; int idx; };
using Strip = std::array<Sticker, 3>;
using StripSet = std::array<Strip, 4>;

const StripSet& stripsFor(int face) {
    static const StripSet U = {{
        {{{2,0},{2,1},{2,2}}},
        {{{1,0},{1,1},{1,2}}},
        {{{5,0},{5,1},{5,2}}},
        {{{4,0},{4,1},{4,2}}}
    }};
    static const StripSet D = {{
        {{{2,6},{2,7},{2,8}}},
        {{{4,6},{4,7},{4,8}}},
        {{{5,6},{5,7},{5,8}}},
        {{{1,6},{1,7},{1,8}}}
    }};
    static const StripSet L = {{
        {{{0,0},{0,3},{0,6}}},
        {{{2,0},{2,3},{2,6}}},
        {{{3,0},{3,3},{3,6}}},
        {{{5,8},{5,5},{5,2}}}
    }};
    static const StripSet R = {{
        {{{0,2},{0,5},{0,8}}},
        {{{2,2},{2,5},{2,8}}},
        {{{3,2},{3,5},{3,8}}},
        {{{5,6},{5,3},{5,0}}}
    }};
    static const StripSet F = {{
        {{{0,6},{0,7},{0,8}}},
        {{{1,0},{1,3},{1,6}}},
        {{{3,2},{3,1},{3,0}}},
        {{{4,8},{4,5},{4,2}}}
    }};
    static const StripSet B = {{
        {{{0,0},{0,1},{0,2}}},
        {{{4,0},{4,3},{4,6}}},
        {{{3,6},{3,7},{3,8}}},
        {{{1,8},{1,5},{1,2}}}
    }};
    switch (face) {
        case 0: return U;
        case 3: return D;
        case 4: return L;
        case 1: return R;
        case 2: return F;
        case 5: return B;
        default: throw std::out_of_range("invalid face");
    }
}

}

Cube::Cube() {
    for (int f = 0; f < 6; ++f)
        for (int i = 0; i < 9; ++i)
            faces_[f][i] = static_cast<uint8_t>(f);
}

Cube Cube::solved() { return Cube(); }

void Cube::rotateFaceCW(int face) {
    auto old = faces_[face];
    faces_[face][0] = old[6];
    faces_[face][1] = old[3];
    faces_[face][2] = old[0];
    faces_[face][3] = old[7];
    faces_[face][4] = old[4];
    faces_[face][5] = old[1];
    faces_[face][6] = old[8];
    faces_[face][7] = old[5];
    faces_[face][8] = old[2];
}

void Cube::rotateFaceCCW(int face) {
    auto old = faces_[face];
    faces_[face][0] = old[2];
    faces_[face][1] = old[5];
    faces_[face][2] = old[8];
    faces_[face][3] = old[1];
    faces_[face][4] = old[4];
    faces_[face][5] = old[7];
    faces_[face][6] = old[0];
    faces_[face][7] = old[3];
    faces_[face][8] = old[6];
}

namespace {
void applyStrips(std::array<std::array<uint8_t, 9>, 6>& faces, const StripSet& strips, bool ccw) {
    std::array<std::array<uint8_t, 3>, 4> saved;
    for (int s = 0; s < 4; ++s)
        for (int k = 0; k < 3; ++k)
            saved[s][k] = faces[strips[s][k].face][strips[s][k].idx];

    for (int s = 0; s < 4; ++s) {
        int src = ccw ? (s + 1) % 4 : (s + 3) % 4;
        for (int k = 0; k < 3; ++k)
            faces[strips[s][k].face][strips[s][k].idx] = saved[src][k];
    }
}
}

void Cube::turnU(bool ccw) { ccw ? rotateFaceCCW(0) : rotateFaceCW(0); applyStrips(faces_, stripsFor(0), ccw); }
void Cube::turnD(bool ccw) { ccw ? rotateFaceCCW(3) : rotateFaceCW(3); applyStrips(faces_, stripsFor(3), ccw); }
void Cube::turnL(bool ccw) { ccw ? rotateFaceCCW(4) : rotateFaceCW(4); applyStrips(faces_, stripsFor(4), ccw); }
void Cube::turnR(bool ccw) { ccw ? rotateFaceCCW(1) : rotateFaceCW(1); applyStrips(faces_, stripsFor(1), ccw); }
void Cube::turnF(bool ccw) { ccw ? rotateFaceCCW(2) : rotateFaceCW(2); applyStrips(faces_, stripsFor(2), ccw); }
void Cube::turnB(bool ccw) { ccw ? rotateFaceCCW(5) : rotateFaceCW(5); applyStrips(faces_, stripsFor(5), ccw); }

void Cube::applyMove(Move m) {
    switch (m) {
        case Move::U:  turnU(false); break;
        case Move::Ui: turnU(true);  break;
        case Move::U2: turnU(false); turnU(false); break;
        case Move::D:  turnD(false); break;
        case Move::Di: turnD(true);  break;
        case Move::D2: turnD(false); turnD(false); break;
        case Move::L:  turnL(false); break;
        case Move::Li: turnL(true);  break;
        case Move::L2: turnL(false); turnL(false); break;
        case Move::R:  turnR(false); break;
        case Move::Ri: turnR(true);  break;
        case Move::R2: turnR(false); turnR(false); break;
        case Move::F:  turnF(false); break;
        case Move::Fi: turnF(true);  break;
        case Move::F2: turnF(false); turnF(false); break;
        case Move::B:  turnB(false); break;
        case Move::Bi: turnB(true);  break;
        case Move::B2: turnB(false); turnB(false); break;
        default: throw std::out_of_range("invalid move");
    }
}

void Cube::applyMoves(const std::vector<Move>& moves) {
    for (Move m : moves) applyMove(m);
}

bool Cube::isSolved() const {
    for (int f = 0; f < 6; ++f)
        for (int i = 0; i < 9; ++i)
            if (faces_[f][i] != static_cast<uint8_t>(f)) return false;
    return true;
}

std::string Cube::serialize() const {
    std::string s;
    s.reserve(54);
    for (int f = 0; f < 6; ++f)
        for (int i = 0; i < 9; ++i)
            s.push_back(static_cast<char>('0' + faces_[f][i]));
    return s;
}

Cube Cube::deserialize(const std::string& s) {
    Cube c;
    if (s.size() != 54) throw std::invalid_argument("bad serialized cube");
    for (int f = 0; f < 6; ++f)
        for (int i = 0; i < 9; ++i)
            c.faces_[f][i] = static_cast<uint8_t>(s[f * 9 + i] - '0');
    return c;
}

int Cube::misplacedStickers() const {
    int count = 0;
    for (int f = 0; f < 6; ++f)
        for (int i = 0; i < 9; ++i)
            if (faces_[f][i] != static_cast<uint8_t>(f)) ++count;
    return count;
}

std::vector<Move> Cube::scramble(int length, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, static_cast<int>(Move::COUNT) - 1);
    std::vector<Move> moves;
    moves.reserve(length);
    Move last = Move::COUNT;
    for (int i = 0; i < length; ++i) {
        Move m;
        do { m = static_cast<Move>(dist(rng)); } while (m == last);
        last = m;
        applyMove(m);
        moves.push_back(m);
    }
    return moves;
}

std::string moveToString(Move m) {
    static const std::unordered_map<Move, std::string> names = {
        {Move::U, "U"}, {Move::U2, "U2"}, {Move::Ui, "U'"},
        {Move::D, "D"}, {Move::D2, "D2"}, {Move::Di, "D'"},
        {Move::L, "L"}, {Move::L2, "L2"}, {Move::Li, "L'"},
        {Move::R, "R"}, {Move::R2, "R2"}, {Move::Ri, "R'"},
        {Move::F, "F"}, {Move::F2, "F2"}, {Move::Fi, "F'"},
        {Move::B, "B"}, {Move::B2, "B2"}, {Move::Bi, "B'"}
    };
    auto it = names.find(m);
    return it == names.end() ? "?" : it->second;
}

Move stringToMove(const std::string& s) {
    static const std::unordered_map<std::string, Move> byName = {
        {"U", Move::U}, {"U2", Move::U2}, {"U'", Move::Ui},
        {"D", Move::D}, {"D2", Move::D2}, {"D'", Move::Di},
        {"L", Move::L}, {"L2", Move::L2}, {"L'", Move::Li},
        {"R", Move::R}, {"R2", Move::R2}, {"R'", Move::Ri},
        {"F", Move::F}, {"F2", Move::F2}, {"F'", Move::Fi},
        {"B", Move::B}, {"B2", Move::B2}, {"B'", Move::Bi}
    };
    auto it = byName.find(s);
    if (it == byName.end()) throw std::invalid_argument("unknown move: " + s);
    return it->second;
}

Move inverseMove(Move m) {
    switch (m) {
        case Move::U: return Move::Ui;   case Move::Ui: return Move::U;   case Move::U2: return Move::U2;
        case Move::D: return Move::Di;   case Move::Di: return Move::D;   case Move::D2: return Move::D2;
        case Move::L: return Move::Li;   case Move::Li: return Move::L;   case Move::L2: return Move::L2;
        case Move::R: return Move::Ri;   case Move::Ri: return Move::R;   case Move::R2: return Move::R2;
        case Move::F: return Move::Fi;   case Move::Fi: return Move::F;   case Move::F2: return Move::F2;
        case Move::B: return Move::Bi;   case Move::Bi: return Move::B;   case Move::B2: return Move::B2;
        default: throw std::out_of_range("invalid move");
    }
}

int moveFace(Move m) {
    switch (m) {
        case Move::U: case Move::U2: case Move::Ui: return 0;
        case Move::R: case Move::R2: case Move::Ri: return 1;
        case Move::F: case Move::F2: case Move::Fi: return 2;
        case Move::D: case Move::D2: case Move::Di: return 3;
        case Move::L: case Move::L2: case Move::Li: return 4;
        case Move::B: case Move::B2: case Move::Bi: return 5;
        default: throw std::out_of_range("invalid move");
    }
}

std::vector<Move> allMoves() {
    return {
        Move::U, Move::U2, Move::Ui, Move::D, Move::D2, Move::Di,
        Move::L, Move::L2, Move::Li, Move::R, Move::R2, Move::Ri,
        Move::F, Move::F2, Move::Fi, Move::B, Move::B2, Move::Bi
    };
}

}
