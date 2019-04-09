/**
 * @file main.cpp
 *
 * @brief Main configuration and execution function for model
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include "../include/simulation.h"

int main() {
    Simulation main_simulation;
    main_simulation.set_day_limit(115);
    main_simulation.initialize_simulation();
    main_simulation.create_vaccinated(0.95);
    main_simulation.pick_random_sick();
    main_simulation.start_simulation();

    return 0;
}
