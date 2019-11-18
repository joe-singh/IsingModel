cmake_minimum_required(VERSION 3.15)
project(Ising)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ising main.cpp lattice.cpp lattice.h Lattice2D.cpp Lattice2D.h)