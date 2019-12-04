#include <iostream>
#include "Lattice2D.h"
#include "Lattice3D.h"
#include "Lattice4D.h"
#include "Lattice5D.h"
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
    std::unordered_map<double, double> m2Data;
    std::unordered_map<double, double> susceptibilityData;
    std::unordered_map<double, double> heatCapData;

    std::vector<double> magnetisations;
    std::vector<double> temperatures;
    std::vector<double> susceptibilities;
    std::vector<double> magnetisationSq;
    std::vector<double> heatCap;

    const int THERMAL = 100;
    const int nlat = 8;
    const size_t n_temp = 100;
    int counter = 0;
    const size_t n_iter = 1000;
    const size_t nthreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(nthreads);
    std::mutex critical;
    for (int t = 0; t<nthreads; t++) {

        threads[t] = std::thread(std::bind(
                [&](const int bi, const int ei,const int t) {
                    for(int i = bi; i <ei; i++) {

                        Lattice5D lat = Lattice5D(nlat);
                        int latSize = pow(nlat, 2);
                        double temperature = i/100.0 + 8.0;
                        double magnetisationSum = 0.0;
                        //double susceptibilitySum = 0.0;
                        double m2Sum = 0.0;
                        //std::vector<double> tempStorage;
                        double energySum = 0.0;
                        double e2sum = 0.0;

                        if (temperature == 0) continue;

                        double beta = 1/temperature;
                        double J = 1.0;

                        for (int j = 0; j < n_iter; j++) {
                            lat.Wolff(J, beta);
                            if (j > THERMAL) {
                                double energy = lat.energy();
                                energySum += energy;

                                double E2 = pow(energy, 2);
                                e2sum += E2;

                                int sumOfSpins = lat.M();
                                int M = abs(sumOfSpins);
                                int M2 = pow(M, 2);
                                magnetisationSum += M;

                                m2Sum += M2;
                                //double magnetisation = lat.magnetisation();

                                //magnetisationSum += magnetisation;

                            }
                        }
                        std::lock_guard<std::mutex> lock(critical);
                        int numAvg = n_iter - THERMAL;
                        double avgE = energySum / numAvg;
                        double avgE2 = e2sum / numAvg;
                        double C = (avgE2 - pow(avgE, 2)) * pow(beta, 2);
                        double avgMagnet = magnetisationSum / (n_iter - THERMAL);
                        //double avgSuscept = susceptibilitySum / (n_iter - THERMAL);
                        double avgM2 = m2Sum / (n_iter - THERMAL);
                        double susceptibitlity = (avgM2 - pow(avgMagnet ,2)) * beta;

                        counter++;
                        susceptibilityData[temperature] = susceptibitlity;
                        //magnetisationData[temperature] = avgMagnet;
                        //m2Data[temperature] = avgM2;
                        heatCapData[temperature] = C;
                        std::cout << "Done temperature " << temperature << " Counter: " << counter << std::endl;
                    }
                },t*n_temp/nthreads,(t+1)==nthreads?n_temp:(t+1)*n_temp/nthreads,t));

    }
    std::for_each(threads.begin(), threads.end(), [](std::thread& x){x.join();});


    /*for (auto x : susceptibilityData) {
        temperatures.emplace_back(x.first);
        //magnetisations.emplace_back(x.second);
        susceptibilities.emplace_back(x.second);
    }

    for (auto x : magnetisationData) {
        //temperatures.emplace_back(x.first);
        magnetisations.emplace_back(x.second);
    }

    for (auto x : m2Data) {
        magnetisationSq.emplace_back(x.second);
    }*/

    for (auto x : heatCapData) {
        temperatures.emplace_back(x.first);
        heatCap.emplace_back(x.second);
    }

    for (auto x : susceptibilityData) {
        susceptibilities.emplace_back(x.second);
    }

    for (double temperature : temperatures) {
        std::cout << temperature << ", ";
    }
    std::cout << "\nHeat Capacities\n\n";
    for (double capacity : heatCap) {
        std::cout << capacity << ", ";
    }

    std::cout << "\nSusceptibilities\n\n";
    for (double sus : susceptibilities) {
        std::cout << sus << ", ";
    }

    //std::cout << "\n<M>\n\n";
    //for (double m : magnetisations) {
    //    std::cout << m << ", ";
    //}

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "\nFinished in " << duration.count() << " for N = " << nlat << ", n_iter = " << n_iter << std::endl;

    return 0;
}