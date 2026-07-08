cmake_minimum_required(VERSION 3.14)
project(rubiks_cube_solver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(cube_solver_core
  src/Cube.cpp
  src/PatternDatabase.cpp
  src/BFSSolver.cpp
  src/DFSSolver.cpp
  src/IDDFSSolver.cpp
  src/IDASolver.cpp
)

add_executable(solver_demo src/main.cpp)
target_link_libraries(solver_demo cube_solver_core)

enable_testing()
add_executable(test_cube tests/test_cube.cpp src/Cube.cpp)
add_test(NAME test_cube COMMAND test_cube)
