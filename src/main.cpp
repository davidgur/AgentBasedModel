/*
 * main.cpp
 *
 * Main file for executing simulation.
 *
 * ----------
 * Agent Based Model (ABM) for analyzing the spread of measles within secondary schools
 *
 * Author: David Gurevich
 * email: david@gurevich.ca
 *
 * ----------
 * TODO: Start super basic simulation
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "../include/agent.h"
#include "../include/configure_agents.h"
#include "../include/graph_building.h"
#include "../include/simulation.h"

int main() {
    Simulation main_simulation;
    main_simulation.set_day_limit(-1);
    main_simulation.initialize_simulation();
    main_simulation.start_simulation();

    return 0;
}