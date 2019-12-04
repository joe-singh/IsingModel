//
// Created by Jyotirmai Singh on 11/28/19.
//

#include "Lattice5D.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <unordered_set>
#include <cmath>
#include <random>
#include <stack>

Lattice5D::Lattice5D(int latSize) : Lattice(latSize) {

    for (int i = 0; i < latSize; i++) {
        std::vector<std::vector<std::vector<std::vector<int>>>> row;
        for (int j = 0; j < latSize; j++) {
            std::vector<std::vector<std::vector<int>>> innerRow;
            for (int k = 0; k < latSize; k++) {
                std::vector<std::vector<int>> innerInnerRow;
                for (int l = 0; l < latSize; l++) {
                    std::vector<int> innerInnerInnerRow;
                    for (int n = 0; n < latSize; n++) {
                        int spin = rand() % 2;
                        if (spin == 0) spin = -1;
                        innerInnerInnerRow.emplace_back(spin);
                    }
                    innerInnerRow.emplace_back(innerInnerInnerRow);
                }
                innerRow.emplace_back(innerInnerRow);
            }
            row.emplace_back(innerRow);
        }
        _lat.emplace_back(row);
    }
}

int Lattice5D::getSite(int x, int y, int z, int w, int u) {
    return _lat[x][y][z][w][u];
}

void Lattice5D::flipSite(int x, int y, int z, int w, int u) {
    _lat[x][y][z][w][u] = -_lat[x][y][z][w][u];
}

/*void Lattice5D::display() {
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

struct penta_hash {
    inline std::size_t operator()(const std::tuple<int,int,int,int,int> & v) const {
        int x = std::get<0>(v);
        int y = std::get<1>(v);
        int z = std::get<2>(v);
        int w = std::get<3>(v);
        int u = std::get<4>(v);
        return (x * 18397) + (y * 20483) + (z * 29303) + (w * 31873) + (u * 36919);
    }
};

void Lattice5D::Wolff(double J, double b) {
    srand(time(0));
    int randX = rand() % _latSize;
    int randY = rand() % _latSize;
    int randZ = rand() % _latSize;
    int randW = rand() % _latSize;
    int randU = rand() % _latSize;

    std::tuple<int, int, int, int, int> randomPoint = std::make_tuple(randX, randY, randZ, randW, randU);

    std::unordered_set<std::tuple<int, int, int, int, int>, penta_hash> cluster;
    //std::set<std::array<int, 2>> cluster;
    //std::set<std::array<int, 2>> frontier;
    std::unordered_set<std::tuple<int, int, int, int, int>, penta_hash> frontier;
    cluster.insert(randomPoint);
    frontier.insert(randomPoint);

    double p = 1 - exp(-2*b*J);

    while (!frontier.empty()) {
        std::unordered_set<std::tuple<int, int, int, int, int>, penta_hash> newFrontier;
        //std::set<std::array<int , 2>>::iterator it;
        for (auto point : frontier) {
            int x = std::get<0>(point);
            int y = std::get<1>(point);
            int z = std::get<2>(point);
            int w = std::get<3>(point);
            int u = std::get<4>(point);

            int siteSpin = getSite(x, y, z, w, u);

            std::vector<std::tuple<int, int, int, int, int>> neighbours = getNeighbours(x, y, z, w, u);
            for (auto neighbour : neighbours) {
                if (!isValidCoord(neighbour)) continue;
                int neighbourX = std::get<0>(neighbour);
                int neighbourY = std::get<1>(neighbour);
                int neighbourZ = std::get<2>(neighbour);
                int neighbourW = std::get<3>(neighbour);
                int neighbourU = std::get<4>(neighbour);

                int neighbourSpin = getSite(neighbourX, neighbourY, neighbourZ, neighbourW, neighbourU);

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
        int u = std::get<4>(site);
        flipSite(x, y, z, w, u);
    }

    //display();
}

std::vector<std::tuple<int, int, int, int, int>> Lattice5D::getNeighbours(int x, int y, int z, int w, int u) {

    std::tuple<int, int, int, int, int> x_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> y_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> x_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> y_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> z_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> z_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> w_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> w_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> u_plus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};
    std::tuple<int, int, int, int, int> u_minus = {GARBAGE, GARBAGE, GARBAGE, GARBAGE, GARBAGE};

    if (x + 1 < _latSize) {
        std::get<0>(x_plus) = x + 1;
        std::get<1>(x_plus) = y;
        std::get<2>(x_plus) = z;
        std::get<3>(x_plus) = w;
        std::get<4>(x_plus) = u;
    } else {
        std::get<0>(x_plus) = 0;
        std::get<1>(x_plus) = y;
        std::get<2>(x_plus) = z;
        std::get<3>(x_plus) = w;
        std::get<4>(x_plus) = u;
    }

    if (x - 1 >= 0) {
        std::get<0>(x_minus) = x - 1;
        std::get<1>(x_minus) = y;
        std::get<2>(x_minus) = z;
        std::get<3>(x_minus) = w;
        std::get<4>(x_minus) = u;
    } else {
        std::get<0>(x_minus) = _latSize - 1;
        std::get<1>(x_minus) = y;
        std::get<2>(x_minus) = z;
        std::get<3>(x_minus) = w;
        std::get<4>(x_minus) = u;
    }


    if (y + 1 < _latSize) {
        std::get<0>(y_plus) = x;
        std::get<1>(y_plus) = y + 1;
        std::get<2>(y_plus) = z;
        std::get<3>(y_plus) = w;
        std::get<4>(y_plus) = u;
    } else {
        std::get<0>(y_plus) = x;
        std::get<1>(y_plus) = 0;
        std::get<2>(y_plus) = z;
        std::get<3>(y_plus) = w;
        std::get<4>(y_plus) = u;
    }


    if (y - 1 >= 0) {
        std::get<0>(y_minus) = x;
        std::get<1>(y_minus) = y - 1;
        std::get<2>(y_minus) = z;
        std::get<3>(y_minus) = w;
        std::get<4>(y_minus) = u;
    } else {
        std::get<0>(y_minus) = x;
        std::get<1>(y_minus) = _latSize - 1;
        std::get<2>(y_minus) = z;
        std::get<3>(y_minus) = w;
        std::get<4>(y_minus) = u;
    }

    if (z + 1 < _latSize) {
        std::get<0>(z_plus) = x;
        std::get<1>(z_plus) = y;
        std::get<2>(z_plus) = z + 1;
        std::get<3>(z_plus) = w;
        std::get<4>(z_plus) = u;
    } else {
        std::get<0>(z_plus) = x;
        std::get<1>(z_plus) = y;
        std::get<2>(z_plus) = 0;
        std::get<3>(z_plus) = w;
        std::get<4>(z_plus) = u;
    }

    if (z - 1 >= 0) {
        std::get<0>(z_minus) = x;
        std::get<1>(z_minus) = y;
        std::get<2>(z_minus) = z - 1;
        std::get<3>(z_minus) = w;
        std::get<4>(z_minus) = u;
    } else {
        std::get<0>(z_minus) = x;
        std::get<1>(z_minus) = y;
        std::get<2>(z_minus) = _latSize - 1;
        std::get<3>(z_minus) = w;
        std::get<4>(z_minus) = u;
    }

    if (w + 1 < _latSize) {
        std::get<0>(w_plus) = x;
        std::get<1>(w_plus) = y;
        std::get<2>(w_plus) = z;
        std::get<3>(w_plus) = w + 1;
        std::get<4>(w_plus) = u;
    } else {
        std::get<0>(w_plus) = x;
        std::get<1>(w_plus) = y;
        std::get<2>(w_plus) = z;
        std::get<3>(w_plus) = 0;
        std::get<4>(w_plus) = u;
    }

    if (w - 1 >= 0) {
        std::get<0>(w_minus) = x;
        std::get<1>(w_minus) = y;
        std::get<2>(w_minus) = z;
        std::get<3>(w_minus) = w - 1;
        std::get<4>(w_minus) = u;
    } else {
        std::get<0>(w_minus) = x;
        std::get<1>(w_minus) = y;
        std::get<2>(w_minus) = z;
        std::get<3>(w_minus) = _latSize - 1;
        std::get<4>(w_minus) = u;
    }

    if (u + 1 < _latSize) {
        std::get<0>(u_plus) = x;
        std::get<1>(u_plus) = y;
        std::get<2>(u_plus) = z;
        std::get<3>(u_plus) = w;
        std::get<4>(u_plus) = u + 1;
    } else {
        std::get<0>(u_plus) = x;
        std::get<1>(u_plus) = y;
        std::get<2>(u_plus) = z;
        std::get<3>(u_plus) = w;
        std::get<4>(u_plus) = 0;
    }

    if (u - 1 >= 0) {
        std::get<0>(u_minus) = x;
        std::get<1>(u_minus) = y;
        std::get<2>(u_minus) = z;
        std::get<3>(u_minus) = w;
        std::get<4>(u_minus) = u - 1;
    } else {
        std::get<0>(u_minus) = x;
        std::get<1>(u_minus) = y;
        std::get<2>(u_minus) = z;
        std::get<3>(u_minus) = w;
        std::get<4>(u_minus) = _latSize - 1;
    }


    std::vector<std::tuple<int, int, int, int, int>> neighbours;

    neighbours.emplace_back(x_plus);
    neighbours.emplace_back(x_minus);
    neighbours.emplace_back(y_plus);
    neighbours.emplace_back(y_minus);
    neighbours.emplace_back(z_plus);
    neighbours.emplace_back(z_minus);
    neighbours.emplace_back(w_plus);
    neighbours.emplace_back(w_minus);
    neighbours.emplace_back(u_plus);
    neighbours.emplace_back(u_minus);

    return neighbours;
}

bool Lattice5D::isValidCoord(std::tuple<int, int, int, int, int> coord) {
    int first = std::get<0>(coord);
    int second = std::get<1>(coord);
    int third = std::get<2>(coord);
    int fourth = std::get<3>(coord);
    int fifth = std::get<4>(coord);
    return first != GARBAGE && third != GARBAGE && second != GARBAGE && fourth != GARBAGE && fifth != GARBAGE;
}

double Lattice5D::magnetisation() {
    int tot = 0;

    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int k = 0; k < _latSize; k++) {
                for (int l = 0; l < _latSize; l++) {
                    for (int n = 0; n < _latSize; n++) {
                        tot += _lat[i][j][k][l][n];
                    }
                }
            }
        }
    }
    return abs(1/pow(_latSize, 5) * tot);
}

int Lattice5D::M() {
    int tot = 0;
    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int k = 0; k < _latSize; k++) {
                for (int l = 0; l < _latSize; l++) {
                    for (int n = 0; n < _latSize; n++) {
                        tot += _lat[i][j][k][l][n];
                    }
                }
            }
        }
    }
    return tot;
}

double Lattice5D::energy() {
    double ene = 0;
    for (int x = 0; x < _latSize; x++) {
        for (int y = 0; y < _latSize; y++) {
            for (int z = 0; z < _latSize; z++) {
                for (int t = 0; t < _latSize; t++) {
                    for (int u = 0; u < _latSize; u++) {
                    int site = getSite(x, y, z, t, u);
                    std::vector<std::tuple<int, int, int, int, int>> neighbours = getNeighbours(x, y, z, t, u);
                    for (auto neighbour : neighbours) {
                        int nb = getSite(std::get<0>(neighbour), std::get<1>(neighbour),
                                         std::get<2>(neighbour), std::get<3>(neighbour),
                                         std::get<4>(neighbour));
                        ene += -nb * site;
                    }
                    }
                }
            }
        }
    }
    return ene/4.0;
}

double Lattice5D::susceptibility(double beta) {
    int m2 = 0;

    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            for (int k = 0; k < _latSize; k++) {
                for (int l = 0; l < _latSize; l++) {
                    for (int n = 0; n < _latSize; n++) {
                        m2 += pow(_lat[i][j][k][l][n], 2);
                    }
                }
            }
        }
    }
    double m2avg = 1/pow(_latSize,5) * m2;
    return beta * (m2avg - pow(magnetisation(), 2));
}