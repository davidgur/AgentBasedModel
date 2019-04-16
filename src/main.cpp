/**
 * @file main.cpp
 *
 * @brief Main configuration and execution function for model
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include "../include/simulation.h"

#include <thread>
#include <sys/stat.h>

int main() {
    const int NUM_OF_SIMULATIONS = 10;
    const double VACC_RATE = 0.5;

    std::vector<std::thread> sim_threads;

    mkdir(("export/SIM_" + std::to_string(VACC_RATE)).c_str(), 0777);

    for (int i = 1; i <= NUM_OF_SIMULATIONS; i++) {
        auto *sim = new Simulation(i, VACC_RATE);
        sim->set_day_limit(-1);
        sim->initialize_simulation();
        sim->create_vaccinated();
        sim->pick_random_sick();

        sim_threads.push_back(sim->start_simulation_thread());
    }

    for (auto &th : sim_threads)
        th.join();

    return 0;
}
