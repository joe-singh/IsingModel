//
// Created by Jyotirmai Singh on 11/17/19.
//

#include "Lattice2D.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <cmath>
#include <random>

Lattice2D::Lattice2D(int latSize) : Lattice(latSize) {

   for (int i = 0; i < latSize; i++) {
       std::vector<int> row (latSize, 1);
       _lat.push_back(row);
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

void Lattice2D::Wolff(double J, double b) {
    srand(time(0));
    int randX = rand() % _latSize;
    int randY = rand() % _latSize;
    std::array<int, 2> randomPoint = {randX, randY};

    std::set<std::array<int, 2>> cluster;
    std::set<std::array<int, 2>> frontier;
    cluster.insert(randomPoint);
    frontier.insert(randomPoint);

    double p = 1 - exp(-2*b*J);

    while (!frontier.empty()) {
        std::set<std::array<int, 2>> newFrontier;
        std::set<std::array<int , 2>>::iterator it;
        for (it = frontier.begin(); it != frontier.end(); ++it) {
            std::array<int, 2> point = *it;
            int x = point[0];
            int y = point[1];
            int siteSpin = getSite(x, y);

            std::vector<std::array<int, 2>> neighbours = getNeighbours(x, y);
            for (auto neighbour : neighbours) {
                if (!isValidCoord(neighbour)) continue;
                int neighbourX = neighbour[0];
                int neighbourY = neighbour[1];
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
        int x = site[0];
        int y = site[1];
        flipSite(x, y);
    }

    display();
}
/*
 *     rand_x, rand_y = np.random.randint(N), np.random.randint(N)
    collection_set = set([(rand_x, rand_y)])
    frontier = set([(rand_x, rand_y)])
    #print(collection_set)
    p = 1 - np.exp(-2*b*J)

    while frontier:
        new_frontier = set()
        for point in frontier:
            x,y = point[0], point[1]
            site_spin = get_site_2d(lat, point)
            point_neighbours = get_neighbours_2d(x,y,N)
            for neighbour in point_neighbours:
                if not neighbour:
                    continue
                neighbour_spin = get_site_2d(lat, neighbour)
                if site_spin == neighbour_spin and neighbour not in collection_set:
                    if np.random.uniform() < p:
                        new_frontier.update([neighbour])
                        collection_set.update([neighbour])
        frontier = new_frontier

    for site in collection_set:
        flip_site_2d(lat, site)
    #print(lat)
    return lat
 * */
std::vector<std::array<int, 2>> Lattice2D::getNeighbours(int x, int y) {

    std::array<int, 2> x_plus = {GARBAGE, GARBAGE};
    std::array<int, 2> y_plus = {GARBAGE, GARBAGE};
    std::array<int, 2> x_minus = {GARBAGE, GARBAGE};
    std::array<int, 2> y_minus = {GARBAGE, GARBAGE};

   if (x + 1 < _latSize) {
       x_plus[0] = x + 1;
       x_plus[1] = y;
   } if (x - 1 >= 0) {
       x_minus[0] = x - 1;
       x_minus[1] = y;
   } if (y + 1 < _latSize) {
       y_plus[0] = x;
       y_plus[1] = y + 1;
   } if (y - 1 >= 0) {
       y_minus[0] = x;
       y_minus[1] = y - 1;
   }

   std::vector<std::array<int, 2>> neighbours;

   neighbours.push_back(x_plus);
   neighbours.push_back(x_minus);
   neighbours.push_back(y_plus);
   neighbours.push_back(y_minus);

   return neighbours;
}

bool Lattice2D::isValidCoord(std::array<int, 2> coord) {
    return coord[0] != GARBAGE && coord[1] != GARBAGE;
}
