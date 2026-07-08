# Algorithmic Rubik's Cube Solver

A C++17 Rubik's Cube solver comparing uninformed search (BFS, DFS, IDDFS)
against an informed IDA* search guided by a custom corner pattern database.

## Architecture

```
include/            Public headers
  Cube.hpp           Move-based cube representation (54-facelet state)
  Solver.hpp          Abstract base class for all search strategies
  BFSSolver.hpp        Breadth-first search
  DFSSolver.hpp        Depth-first search
  IDDFSSolver.hpp      Iterative deepening DFS
  IDASolver.hpp        IDA* using the pattern database heuristic
  PatternDatabase.hpp  Admissible heuristic table built from a corner
                       projection of the cube state

src/                 Implementations + CLI demo (main.cpp)
tests/               Standalone correctness tests for the cube engine
```

### Object-oriented design

`Solver` is an abstract base class exposing a pure virtual `solve()` and a
`name()` accessor. `BFSSolver`, `DFSSolver`, `IDDFSSolver`, and `IDASolver`
all derive from it, so a solver can be selected and swapped polymorphically
(see `main.cpp`). `Cube` overloads `operator==` for state comparison and
exposes a small, move-oriented API (`applyMove`, `applyMoves`, `scramble`)
rather than exposing raw facelet arrays for mutation.

### Search strategies

- **BFS** — explores the state graph level by level with a visited-state
  hash set, guaranteeing a shortest solution within the configured depth
  bound; memory grows quickly with depth.
- **DFS** — explores a single branch to the depth bound before
  backtracking, with same-face-move pruning (two turns of the same face in
  a row are never optimal).
- **IDDFS** — repeatedly runs depth-limited DFS with an increasing depth
  bound, combining DFS's low memory footprint with BFS's shortest-path
  guarantee.
- **IDA\*** — iterative-deepening search over `f = g + h`, where `h` comes
  from `PatternDatabase`: a breadth-first-search table built once, offline,
  by exploring every reachable state of an 8-corner projection of the cube
  up to a configurable depth cap. Any state outside the table has a true
  distance greater than the cap, so the cap itself is a valid (admissible)
  lower bound. This prunes the search far more aggressively than the
  blind strategies, which is what makes deeper scrambles tractable.

### A note on performance

This reference implementation uses a 54-byte facelet string as the search
key for simplicity and readability. That keeps `Cube` easy to follow but
means every state hash/copy touches 54 bytes and the visited-state sets
used by BFS/IDDFS grow fast, which is the standard tradeoff for uninformed
search on a state space this size — expect BFS to become slow past
6-7 move scrambles on a laptop. IDA* with the pattern database scales
noticeably further because it doesn't materialize a visited set at all.
To push past that (e.g., consistently sub-second solves on 12-14 move
scrambles), the usual next steps are: switch to a packed cubie-level
representation (corner permutation/orientation + edge permutation/
orientation as integers), add an edge pattern database alongside the
corner one, and exploit the cube's symmetry group to shrink the tables
further. The algorithmic structure (IDA*, pattern databases, iterative
deepening) is unchanged by that optimization — only the state encoding is.

## Building

```bash
mkdir build && cd build
cmake ..
make -j
```

This produces:
- `solver_demo` — runs BFS/DFS/IDDFS on a shallow scramble and IDA* on a
  deeper one, printing move count, states explored, and wall-clock time
  for each.
- `test_cube` — correctness tests for the move engine (also runnable via
  `ctest`).

```bash
./solver_demo [pattern_db_depth] [shallow_scramble_len] [deep_scramble_len]
./solver_demo 6 6 12
```

Building without CMake also works:

```bash
g++ -std=c++17 -Iinclude -O3 src/*.cpp -o solver_demo
g++ -std=c++17 -Iinclude -O3 tests/test_cube.cpp src/Cube.cpp -o test_cube
```

## Tests

```bash
cd build
ctest --output-on-failure
```

`test_cube.cpp` verifies: every move has the expected order (quarter turns
return to solved after 4 applications, half turns after 2), every move is
undone by its inverse, a random scramble is exactly undone by replaying
its inverses in reverse order, serialization round-trips, and every color
appears exactly 9 times after scrambling (a basic permutation-validity
check).
