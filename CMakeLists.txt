cmake_minimum_required(VERSION 3.15)
project(Ising)

set(CMAKE_CXX_STANDARD 14)

include_directories("/Users/joesingh/desktop/Software/matplotlib-cpp")
add_executable(Ising main.cpp lattice.cpp lattice.h Lattice2D.cpp Lattice2D.h Lattice3D.cpp Lattice3D.h Lattice4D.cpp Lattice4D.h Lattice5D.cpp Lattice5D.h)