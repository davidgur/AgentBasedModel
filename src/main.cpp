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
#include <iostream>

int main(int argc, char *argv[]) {
    const int NUM_OF_SIMULATIONS = std::stoi(argv[1]);
    const double VACC_RATE = std::stod(argv[2]);

    std::vector<std::thread> sim_threads;

    system(("rm -rf export/SIM_" + std::to_string(VACC_RATE)).c_str());
    mkdir(("export/SIM_" + std::to_string(VACC_RATE)).c_str(), 0777);

    std::cout << "Starting " << NUM_OF_SIMULATIONS << " simulations with " << VACC_RATE << " vaccination rate."
              << std::endl;
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
