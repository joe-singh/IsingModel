//
// Created by Jyotirmai Singh on 11/17/19.
//

#include "Lattice.h"
#include <tuple>
#include <array>

#ifndef ISING_LATTICE2D_H
#define ISING_LATTICE2D_H


class Lattice2D : public Lattice {
public:
    explicit Lattice2D(int latSize);
    int getSite(int x, int y);
    void flipSite(int x, int y);
    void display() override;
    bool isValidCoord(std::array<int, 2> coord);
    void Wolff(double J, double b) override;
    std::vector<std::array<int, 2>> getNeighbours(int x, int y);
};


#endif //ISING_LATTICE2D_H
