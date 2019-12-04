//
// Created by Jyotirmai Singh on 11/28/19.
//
#include "Lattice.h"
#ifndef ISING_LATTICE5D_H
#define ISING_LATTICE5D_H


class Lattice5D : public Lattice {
public:
    explicit Lattice5D(int latSize);
    int getSite(int x, int y, int z, int w, int u);
    void flipSite(int x, int y, int z, int w, int u);
    //void display() override;
    bool isValidCoord(std::tuple<int, int, int, int, int> coord);
    void Wolff(double J, double b) override;
    double magnetisation() override;
    double susceptibility(double beta) override;
    double energy();
    int M();
    std::vector<std::tuple<int, int, int, int, int>> getNeighbours(int x, int y, int z, int w, int u);
private:
    std::vector<std::vector<std::vector<std::vector<std::vector<int>>>>> _lat;

};


#endif //ISING_LATTICE5D_H
