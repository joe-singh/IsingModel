#include <iostream>
#include "Lattice2D.h"
#include <chrono>

int main() {

    auto start = std::chrono::high_resolution_clock::now();
    //Lattice2D* lat = new Lattice2D(20);
    //lat->display();

    //lat->Wolff(1, 1e-6);

    //std::cout << "Lattice Magnetisation = " << lat->magnetisation() << std::endl;

    std::vector<double> magnetisations;
    std::vector<double> temperatures;

    const int nlat = 40;
    const int n_iter = 20000;
    for (int i = 100; i < 300; i += 40) {

        Lattice2D lat = Lattice2D(nlat);
        double temperature = i/100.0;
        temperatures.push_back(temperature);

        std::cout << "Doing temperature " << temperature << std::endl;

        double beta = 1/temperature;
        double J = 1.0;

        for (int j = 0; j < n_iter; j++) {

            if (j % 1000 == 0) std::cout << "At iteration " << j << std::endl;
            lat.Wolff(J, beta);
        }
        double magnet = lat.magnetisation();
        magnetisations.push_back(magnet);
        std::cout << "Done temperature " << temperature << std::endl;
    }
    //lat->display();
    for (int  i = 0 ; i < temperatures.size(); i++) {
        std::cout << temperatures[i] << ", ";
    }
    std::cout << "\n\n";
    for (int i = 0; i < magnetisations.size(); i++) {
        std::cout << magnetisations[i] << ", ";
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Finished in " << duration.count() << " for N = " << nlat << ", n_iter = " << n_iter << std::endl;
    return 0;
}