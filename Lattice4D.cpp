//
// Created by Jyotirmai Singh on 11/28/19.
//

#include "Lattice4D.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <unordered_set>
#include <cmath>
#include <random>
#include <stack>

Lattice4D::Lattice4D(int latSize) : Lattice(latSize) {

    for (int i = 0; i < latSize; i++) {
        std::vector<std::vector<std::vector<int>>> row;
        for (int j = 0; j < latSize; j++) {
            std::vector<std::vector<int>> innerRow;
            for (int k = 0; k < latSize; k++) {
                std::vector<int> innerInnerRow;
                for (int l = 0; l < latSize; l++) {
                int spin = rand() % 2;
                if (spin == 0) spin = -1;
                innerInnerRow.emplace_back(spin);
                }
            innerRow.emplace_back(innerInnerRow);
            }
            row.emplace_back(innerRow);
        }
        _lat.emplace_back(row);
    }
}

int Lattice4D::getSite(int x, int y, int z, int w) {
    return _lat[x][y][z][w];
}

void Lattice4D::flipSite(int x, int y, int z, int w) {
    _lat[x][y][z][w] = -_lat[x][y][z][w];
}

/*void Lattice4D::display() {
    for (int i = 0; i < _lat.size(); i++) {
        for (int j = 0; j < _lat[i].size(); j++)
            std::cout << _lat[i][j];
            if (j != _lat.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}*/

struct quad_hash {
    inline std::size_t operator()(const std::tuple<int,int,int, int> & v) const {
        int x = std::get<0>(v);
        int y = std::get<1>(v);
        int z = std::get<2>(v);
        int w = std::get<3>(v);
        return (x * 18397) + (y * 20483) + (z * 29303) + (w * 31873);
    }
};





void Lattice4D::Wolff(double J, double b) {
    srand(time(0));
    int randX = rand() % _latSize;
    int randY = rand() % _latSize;
    int randZ = rand() % _latSize;
    int randW = rand() % _latSize;

    std::tuple<int, int, int, int> randomPoint = std::make_tuple(randX, randY, randZ, randW);

    std::unordered_set<std::tuple<int, int, int, int>, quad_hash> cluster;
    //std::set<std::array<int, 2>> cluster;
    //std::set<std::array<int, 2>> frontier;
    std::unordered_set<std::tuple<int, int, int, int>, quad_hash> frontier;
    cluster.insert(randomPoint);
    frontier.insert(randomPoint);

    double p = 1 - exp(-2*b*J);

    while (!frontier.empty()) {
        std::unordered_set<std::tuple<int, int, int, int>, quad_hash> newFrontier;
        //std::set<std::array<int , 2>>::iterator it;
        for (auto point : frontier) {
            int x = std::get<0>(point);
            int y = std::get<1>(point);
            int z = std::get<2>(point);
            int w = std::get<3>(point);

            int siteSpin = getSite(x, y, z, w);

            std::vector<std::tuple<int, int, int, int>> neighbours = getNeighbours(x, y, z, w);
            for (auto neighbour : neighbours) {
                if (!isValidCoord(neighbour)) continue;
                int neighbourX = std::get<0>(neighbour);
                int neighbourY = std::get<1>(neighbour);
                int neighbourZ = std::get<2>(neighbour);
                int neighbourW = std::get<3>(neighbour);

                int neighbourSpin = getSite(neighbourX, neighbourY, neighbourZ, neighbourW);

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
        int x = std::get<0>(site);
        int y = std::get<1>(site);
        int z = std::get<2>(site);
        int w = std::get<3>(site);
        flipSite(x, y, z, w);
    }

    //display();
}

std::vector<std::tuple<int, int, int, int>> Lattice4D::getNeighbours(int x, int y, int z, int w) {

    std::tuple<int, int, int, int> x_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> y_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> x_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> y_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> z_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> z_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> w_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int> w_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE};

    if (x + 1 < _latSize) {
        std::get<0>(x_plus) = x + 1;
        std::get<1>(x_plus) = y;
        std::get<2>(x_plus) = z;
        std::get<3>(x_plus) = w;
    } else {
        std::get<0>(x_plus) = 0;
        std::get<1>(x_plus) = y;
        std::get<2>(x_plus) = z;
        std::get<3>(x_plus) = w;
    }

    if (x - 1 >= 0) {
        std::get<0>(x_minus) = x - 1;
        std::get<1>(x_minus) = y;
        std::get<2>(x_minus) = z;
        std::get<3>(x_minus) = w;
    } else {
        std::get<0>(x_minus) = _latSize - 1;
        std::get<1>(x_minus) = y;
        std::get<2>(x_minus) = z;
        std::get<3>(x_minus) = w;
    }


    if (y + 1 < _latSize) {
        std::get<0>(y_plus) = x;
        std::get<1>(y_plus) = y + 1;
        std::get<2>(y_plus) = z;
        std::get<3>(y_plus) = w;
    } else {
        std::get<0>(y_plus) = x;
        std::get<1>(y_plus) = 0;
        std::get<2>(y_plus) = z;
        std::get<3>(y_plus) = w;
    }


    if (y - 1 >= 0) {
        std::get<0>(y_minus) = x;
        std::get<1>(y_minus) = y - 1;
        std::get<2>(y_minus) = z;
        std::get<3>(y_minus) = w;
    } else {
        std::get<0>(y_minus) = x;
        std::get<1>(y_minus) = _latSize - 1;
        std::get<2>(y_minus) = z;
        std::get<3>(y_minus) = w;
    }

    if (z + 1 < _latSize) {
        std::get<0>(z_plus) = x;
        std::get<1>(z_plus) = y;
        std::get<2>(z_plus) = z + 1;
        std::get<3>(z_plus) = w;
    } else {
        std::get<0>(z_plus) = x;
        std::get<1>(z_plus) = y;
        std::get<2>(z_plus) = 0;
        std::get<3>(z_plus) = w;
    }

    if (z - 1 >= 0) {
        std::get<0>(z_minus) = x;
        std::get<1>(z_minus) = y;
        std::get<2>(z_minus) = z - 1;
        std::get<3>(z_minus) = w;
    } else {
        std::get<0>(z_minus) = x;
        std::get<1>(z_minus) = y;
        std::get<2>(z_minus) = _latSize - 1;
        std::get<3>(z_minus) = w;
    }

    if (w + 1 < _latSize) {
        std::get<0>(w_plus) = x;
        std::get<1>(w_plus) = y;
        std::get<2>(w_plus) = z;
        std::get<3>(w_plus) = w + 1;
    } else {
        std::get<0>(w_plus) = x;
        std::get<1>(w_plus) = y;
        std::get<2>(w_plus) = z;
        std::get<3>(w_plus) = 0;
    }

    if (w - 1 >= 0) {
        std::get<0>(w_minus) = x;
        std::get<1>(w_minus) = y;
        std::get<2>(w_minus) = z;
        std::get<3>(w_minus) = w - 1;
    } else {
        std::get<0>(w_minus) = x;
        std::get<1>(w_minus) = y;
        std::get<2>(w_minus) = z;
        std::get<3>(w_minus) = _latSize - 1;
    }


    std::vector<std::tuple<int, int, int, int>> neighbours;

    neighbours.emplace_back(x_plus);
    neighbours.emplace_back(x_minus);
    neighbours.emplace_back(y_plus);
    neighbours.emplace_back(y_minus);
    neighbours.emplace_back(z_plus);
    neighbours.emplace_back(z_minus);
    neighbours.emplace_back(w_plus);
    neighbours.emplace_back(w_minus);

    return neighbours;
}

bool Lattice4D::isValidCoord(std::tuple<int, int, int, int> coord) {
    int first = std::get<0>(coord);
    int second = std::get<1>(coord);
    int third = std::get<2>(coord);
    int fourth = std::get<3>(coord);
    return first != GARBAGE && third != GARBAGE && second != GARBAGE && fourth != GARBAGE;
}

double Lattice4D::magnetisation() {
    int tot = 0;

    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int k = 0; k < _latSize; k++) {
                for (int l = 0; l < _latSize; l++) {
                    tot += _lat[i][j][k][l];
                }
            }
        }
    }
    return abs(1/pow(_latSize, 4) * tot);
}

int Lattice4D::M() {
    int tot = 0;
    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int k = 0; k < _latSize; k++) {
                for (int l = 0; l < _latSize; l++) {
                    tot += _lat[i][j][k][l];
                }
            }
        }
    }
    return tot;
}

double Lattice4D::energy() {
    double ene = 0;
    for (int x = 0; x < _latSize; x++) {
        for (int y = 0; y < _latSize; y++) {
            for (int z = 0; z < _latSize; z++) {
                for (int t = 0; t < _latSize; t++) {
                    int site = getSite(x, y, z, t);
                    std::vector<std::tuple<int, int, int, int>> neighbours = getNeighbours(x, y, z, t);
                    for (auto neighbour : neighbours) {
                        int nb = getSite(std::get<0>(neighbour), std::get<1>(neighbour),
                                         std::get<2>(neighbour), std::get<3>(neighbour));
                        ene += -nb * site;
                }
                }
            }
        }
    }
    return ene/4.0;
}

double Lattice4D::susceptibility(double beta) {
    int m2 = 0;

    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int k = 0; k < _latSize; k++) {
                for (int l = 0; l < _latSize; l++) {
                    m2 += pow(_lat[i][j][k][l], 2);
                }
            }
        }
    }
    double m2avg = 1/pow(_latSize,4) * m2;
    return beta * (m2avg - pow(magnetisation(), 2));
}