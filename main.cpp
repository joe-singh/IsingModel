#include <iostream>
#include "Lattice2D.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Lattice2D* lat = new Lattice2D(10);
    lat->display();

    lat->Wolff(1, 0.441);

    std::cout << "EXTERIOR DISPLAY" << std::endl;
    lat->display();
    return 0;
}