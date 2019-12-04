//
// Created by Jyotirmai Singh on 11/28/19.
//
#include "Lattice.h"
#ifndef ISING_LATTICE4D_H
#define ISING_LATTICE4D_H


class Lattice4D : public Lattice {
public:
    explicit Lattice4D(int latSize);
    int getSite(int x, int y, int z, int w);
    void flipSite(int x, int y, int z, int w);
    //void display() override;
    bool isValidCoord(std::tuple<int, int, int, int> coord);
    void Wolff(double J, double b) override;
    double magnetisation() override;
    double susceptibility(double beta) override;
    std::vector<std::tuple<int, int, int, int>> getNeighbours(int x, int y, int z, int w);
    int M();
    double energy();
private:
    std::vector<std::vector<std::vector<std::vector<int>>>> _lat;
};


#endif //ISING_LATTICE4D_H
