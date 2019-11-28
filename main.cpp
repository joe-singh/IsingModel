#include <iostream>
#include "Lattice2D.h"
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <chrono>
#include <numeric>

int main() {

    auto start = std::chrono::high_resolution_clock::now();
    /*Lattice2D* lat = new Lattice2D(10);
    lat->display();
    std::vector<double> magnetisations;
    const int thermalisation = 100;

    for (int i = 0; i <2; i++) {
        lat->Wolff(1, 1/0.5);
        //if (i > thermalisation) {
        //    magnetisations.emplace_back(lat->magnetisation());
        //}
        lat->display();
        std::cout << "-------------------------------\n";
    }
    lat->display();
    std::cout << "-------------------------------\n";
    std::cout << "Lattice Magnetisation = " << lat->magnetisation() << std::endl; //std::accumulate(magnetisations.begin(), magnetisations.end(), 0.0)/magnetisations.size() << std::endl;
*/

    std::unordered_map<double, double> magnetisationData;
    std::unordered_map<double, double> susceptibilityData;

    std::vector<double> magnetisations;
    std::vector<double> temperatures;
    std::vector<double> susceptibilities;

    const int THERMAL = 100;
    const int nlat = 75;
    const size_t n_temp = 60;
    const size_t n_iter = 1000;
    const size_t nthreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(nthreads);
    std::mutex critical;
    for (int t = 0; t<nthreads; t++) {

        threads[t] = std::thread(std::bind(
                [&](const int bi, const int ei,const int t) {
                    for(int i = bi; i <ei; i++) {

                        Lattice2D lat = Lattice2D(nlat);
                        double temperature = i/100.0 + 2.0;
                        double magnetisationSum = 0.0;
                        //std::vector<double> tempStorage;

                        if (temperature == 0) continue;

                        double beta = 1/temperature;
                        double J = 1.0;

                        for (int j = 0; j < n_iter; j++) {

                            //std::cout << "At iteration " << j << std::endl;
                            lat.Wolff(J, beta);
                            if (j > THERMAL) {
                                double magnet = lat.magnetisation();
                                magnetisationSum += magnet;

                            }
                        }
                        std::lock_guard<std::mutex> lock(critical);
                        //double magnet = lat.magnetisation();
                        //double suscept = lat.susceptibility(beta);
                        double avgMagnet = magnetisationSum / (n_iter - THERMAL); //std::accumulate(tempStorage.begin(), tempStorage.end(), 0.0)/tempStorage.size();

                        magnetisationData[temperature] = avgMagnet;
                        //data[temperature] = magnet;
                        std::cout << "Done temperature " << temperature << std::endl;
                    }
                },t*n_temp/nthreads,(t+1)==nthreads?n_temp:(t+1)*n_temp/nthreads,t));

    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& x){x.join();});


    /*for (int i = 0; i < 30; i++) {

        Lattice2D lat = Lattice2D(nlat);
        double temperature = i/100.0 + 2.0;
        double magnetisationSum = 0.0;
        //std::vector<double> tempStorage;

        if (temperature == 0) continue;

        double beta = 1/temperature;
        double J = 1.0;

        for (int j = 0; j < n_iter; j++) {

            //std::cout << "At iteration " << j << std::endl;
            lat.Wolff(J, beta);
            if (j > THERMAL) {
                double magnet = lat.magnetisation();
                magnetisationSum += magnet;

            }
        }
        //std::lock_guard<std::mutex> lock(critical);
        //double magnet = lat.magnetisation();
        //double suscept = lat.susceptibility(beta);
        double avgMagnet = magnetisationSum / (n_iter - THERMAL);
        magnetisationData[temperature] = avgMagnet;
        std::cout << "Done temperature " << temperature << std::endl;
    }*/

    for (auto x : susceptibilityData) {
        temperatures.emplace_back(x.first);
        //magnetisations.emplace_back(x.second);
        susceptibilities.emplace_back(x.second);
    }

    for (auto x : magnetisationData) {
        temperatures.emplace_back(x.first);
        magnetisations.emplace_back(x.second);
    }

    for (double temperature : temperatures) {
        std::cout << temperature << ", ";
    }
    std::cout << "\n\n";
    for (double magnetisation : magnetisations) {
        std::cout << magnetisation << ", ";
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "\nFinished in " << duration.count() << " for N = " << nlat << ", n_iter = " << n_iter << std::endl;

    return 0;
}