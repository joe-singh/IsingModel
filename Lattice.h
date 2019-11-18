//
// Created by Jyotirmai Singh on 11/17/19.
//

#include <vector>
#include <tuple>

#ifndef ISING_LATTICE_H
#define ISING_LATTICE_H


class Lattice {
protected:
    explicit Lattice(int latSize);
    virtual ~Lattice() {}
    virtual int getSite() {}
    virtual void flipSite() {}
    virtual void display() {}
    virtual bool isValidCoord() {}
    virtual void Wolff(double J, double b) {}
    int _latSize;
    std::vector<std::vector<int>> _lat;
    int GARBAGE = -420;
};


#endif //ISING_LATTICE_H
