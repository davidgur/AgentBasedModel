/**
 * @file main.cpp
 *
 * @brief Main configuation and execution function for model
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include "../include/simulation.h"

int main() {
    Simulation main_simulation;
    main_simulation.set_day_limit(1);
    main_simulation.initialize_simulation();
    main_simulation.start_simulation();

    return 0;
}