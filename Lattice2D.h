//
// Created by Jyotirmai Singh on 11/17/19.
//

#include "Lattice.h"
#include <tuple>
#include <array>
#include <utility>
#ifndef ISING_LATTICE2D_H
#define ISING_LATTICE2D_H


class Lattice2D : public Lattice {
public:
    explicit Lattice2D(int latSize);
    int getSite(int x, int y);
    void flipSite(int x, int y);
    void display() override;
    bool isValidCoord(std::pair<int, int> coord);
    void Wolff(double J, double b) override;
    double magnetisation() override;
    double susceptibility(double beta) override;
    double energy();
    std::vector<std::pair<int, int>> getNeighbours(int x, int y);
    int M();
    int M2();
};


#endif //ISING_LATTICE2D_H
