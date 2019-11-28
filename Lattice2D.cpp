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

double randomNo() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    double randomNum = distr(gen);
    return randomNum;
}

/*
void Lattice2D::Wolff(double J, double b) {
    srand(time(0));
    int randX = rand() % _latSize;
    int randY = rand() % _latSize;


    std::pair<int, int> randomPoint = {randX, randY};
    int originalSpin = getSite(randX, randY);
    double p = 1 - exp(-2*b*J);
    double randomnum = randomNo();

    if (randomnum < p) flipSite(randX, randY);
    std::stack<std::pair<int, int>> cluster;
    std::vector<std::pair<int, int>> seen;
    cluster.push(randomPoint);
    //seen.emplace_back(randomPoint);

    while (!cluster.empty()) {
        std::pair<int, int> currPoint = cluster.top();
        cluster.pop();
        int x = currPoint.first;
        int y = currPoint.second;
        seen.emplace_back(currPoint);
        //int currSpin = getSite(x, y);
        std::vector<std::pair<int, int>> neighbours = getNeighbours(x, y);



        for (auto neighbour : neighbours) {
            if (!isValidCoord(neighbour)) continue;
            int neighbourX = neighbour.first;
            int neighbourY = neighbour.second;
            int neighbourSpin = getSite(neighbourX, neighbourY);

            double randomNum = randomNo();
            bool probabilityTest = randomNum < p;
            bool alreadySeenPoint = std::find(seen.begin(), seen.end(), neighbour) != seen.end();
            if (neighbourSpin == originalSpin && probabilityTest && !alreadySeenPoint) {
                cluster.push(neighbour);
                flipSite(neighbourX, neighbourY);
            }
        }
    }
}*/

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

double Lattice2D::magnetisation() {
    int tot = 0;
    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            tot += _lat[i][j];
        }
    }
    return abs(1/pow(_latSize, 2) * tot);
}

double Lattice2D::susceptibility(double beta) {
    int m2 = 0;

    for (int i = 0; i < _latSize; i++) {
        for (int j = 0; j < _latSize; j++) {
            m2 += pow(_lat[i][j], 2);
        }
    }
    double m2avg = 1/pow(_latSize,2) * m2;
    return beta * (m2avg - pow(magnetisation(), 2));
}

