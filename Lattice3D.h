//
// Created by Jyotirmai Singh on 11/27/19.
//
#include "Lattice.h"
#include <tuple>
#include <array>
#include <utility>
#ifndef ISING_LATTICE3D_H
#define ISING_LATTICE3D_H


class Lattice3D : public Lattice {
public:
    explicit Lattice3D(int latSize);
    int getSite(int x, int y, int z);
    void flipSite(int x, int y, int z);
    //void display() override;
    bool isValidCoord(std::tuple<int, int, int> coord);
    void Wolff(double J, double b) override;
    double magnetisation() override;
    double susceptibility(double beta) override;
    double energy();
    std::vector<std::tuple<int, int, int>> getNeighbours(int x, int y, int z);
    int M();
private:
    std::vector<std::vector<std::vector<int>>> _lat;

};


#endif //ISING_LATTICE3D_H
