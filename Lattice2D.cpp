//
// Created by Jyotirmai Singh on 11/17/19.
//

#include "Lattice2D.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <unordered_set>
#include <cmath>
#include <random>
#include <stack>

Lattice2D::Lattice2D(int latSize) : Lattice(latSize) {

   for (int i = 0; i < latSize; i++) {
       std::vector<int> row;
       for (int j = 0; j < latSize; j++) {
           int spin = rand() % 2;
           if (spin == 0) spin = -1;
           row.emplace_back(spin);
       }
       _lat.emplace_back(row);
   }
}

int Lattice2D::getSite(int x, int y) {
    return _lat[x][y];
}

void Lattice2D::flipSite(int x, int y) {
    _lat[x][y] = -_lat[x][y];
}

void Lattice2D::display() {
    for (int i = 0; i < _lat.size(); i++) {
        for (int j = 0; j < _lat[i].size(); j++) {
            std::cout << _lat[i][j];
            if (j != _lat.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

void Lattice2D::Wolff(double J, double b) {
    srand(time(0));
    int randX = rand() % _latSize;
    int randY = rand() % _latSize;
    std::pair<int, int> randomPoint = {randX, randY};

    std::unordered_set<std::pair<int, int>, pair_hash> cluster;
    //std::set<std::array<int, 2>> cluster;
    //std::set<std::array<int, 2>> frontier;
    std::unordered_set<std::pair<int, int>, pair_hash> frontier;
    cluster.insert(randomPoint);
    frontier.insert(randomPoint);

    double p = 1 - exp(-2*b*J);

    while (!frontier.empty()) {
        std::unordered_set<std::pair<int, int>, pair_hash> newFrontier;
        //std::set<std::array<int , 2>>::iterator it;
        for (auto point : frontier) {
            int x = point.first;
            int y = point.second;
            int siteSpin = getSite(x, y);

            std::vector<std::pair<int, int>> neighbours = getNeighbours(x, y);
            for (auto neighbour : neighbours) {
                if (!isValidCoord(neighbour)) continue;
                int neighbourX = neighbour.first;
                int neighbourY = neighbour.second;
                int neighbourSpin = getSite(neighbourX, neighbourY);

                auto iter = cluster.find(neighbour);

                // Making random number to do the r < 1-exp(2bJ) test
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> distr(0.0, 1.0);
                double randomNum = distr(gen);
                bool probabilityTest = randomNum < p;
                if (siteSpin == neighbourSpin && iter == cluster.end() && probabilityTest) {
                    newFrontier.insert(neighbour);
                    cluster.insert(neighbour);
                }
            }

        }
        frontier = newFrontier;
    }

    for (auto site : cluster) {
        int x = site.first;
        int y = site.second;
        flipSite(x, y);
    }

    //display();
}

std::vector<std::pair<int, int>> Lattice2D::getNeighbours(int x, int y) {

    std::pair<int, int> x_plus = {GARBAGE, GARBAGE};
    std::pair<int, int> y_plus = {GARBAGE, GARBAGE};
    std::pair<int, int> x_minus = {GARBAGE, GARBAGE};
    std::pair<int, int> y_minus = {GARBAGE, GARBAGE};


   if (x + 1 < _latSize) {
       x_plus.first = x + 1;
       x_plus.second = y;
   } else {
       x_plus.first = 0;
       x_plus.second = y;
   }

   if (x - 1 >= 0) {
       x_minus.first = x - 1;
       x_minus.second = y;
   } else {
       x_minus.first = _latSize-1;
       x_minus.second = y;
   }


   if (y + 1 < _latSize) {
       y_plus.first = x;
       y_plus.second = y + 1;
   } else {
       y_plus.first = x;
       y_plus.second = 0;
   }


   if (y - 1 >= 0) {
       y_minus.first = x;
       y_minus.second = y - 1;
   } else {
       y_minus.first = x;
       y_minus.second = _latSize-1;
   }

   std::vector<std::pair<int, int>> neighbours;

   neighbours.emplace_back(x_plus);
   neighbours.emplace_back(x_minus);
   neighbours.emplace_back(y_plus);
   neighbours.emplace_back(y_minus);

   return neighbours;
}

bool Lattice2D::isValidCoord(std::pair<int, int> coord) {
    return coord.first != GARBAGE && coord.second != GARBAGE;
}

int Lattice2D::M() {
    int tot = 0;
    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            tot += _lat[i][j];
        }
    }
    return tot;
}

int Lattice2D::M2() {
    return pow(M(), 2);
}

double Lattice2D::magnetisation() {
    return abs(1/pow(_latSize, 2) * M());
}

double Lattice2D::susceptibility(double beta) {
    int m2 = 0;
    // i, j = first particle. l, n = second particle
    /*for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int l = 0; l < _latSize; l++) {
                for (int n = 0; n < _latSize; n++) {
                    int spin1 = getSite(i, j);
                    int spin2 = getSite(l, n);
                    m2 += spin1*spin2;
                }
            }
        }
    }*/
    //int m2_square = pow(m2, 2);
    double mag = magnetisation();
    double m2avg = 1/pow(_latSize,2) * pow(mag, 2);
    return beta * (m2avg - pow(mag, 2));
}

double Lattice2D::energy() {
    double ene = 0;
    for (int x = 0; x < _latSize; x++) {
        for (int y = 0; y < _latSize; y++) {
            int site = getSite(x, y);
            std::vector<std::pair<int, int>> neighbours = getNeighbours(x, y);
            for (auto neighbour : neighbours) {
                int nb = getSite(neighbour.first, neighbour.second);
                ene += -nb*site;
            }
        }
    }
    return ene/4.0;
}

