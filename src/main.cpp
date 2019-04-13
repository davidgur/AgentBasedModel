/**
 * @file main.cpp
 *
 * @brief Main configuration and execution function for model
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include "../include/simulation.h"

int main(int argc, char **argv) {
    Simulation main_simulation;

    main_simulation.set_day_limit(-1);
    main_simulation.initialize_simulation();
    main_simulation.create_vaccinated(std::stod(argv[1]));
    main_simulation.pick_random_sick();

    main_simulation.start_simulation();

    return 0;
}
