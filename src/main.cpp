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
 */


#include <iostream>
#include <vector>
#include <chrono>

#include "../include/agent.h"
#include "../include/graph_building.h"


// Below data is from the YRDSB Monthly Enrolment Report for Secondary Schools (Westmount Collegiate Institute)
// It can be found at: http://www.yrdsb.ca/AboutUs/DirectorsAnnualReport/Documents/2018-19-Sec.pdf
const int Grade9_Population = 292;
const int Grade10_Population = 356;
const int Grade11_Population = 372;
const int Grade12_Population = 334;

std::vector<Agent> grade9_agents;
std::vector<Agent> grade10_agents;
std::vector<Agent> grade11_agents;
std::vector<Agent> grade12_agents;

auto start_time = std::chrono::high_resolution_clock::now();

void log(std::string to_print) {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::cout << "[" << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() << "]\t";
    std::cout << to_print << std::endl;
}

void populate_agent_vectors() {
    // Grade 9
    for (auto i = 0; i < Grade9_Population; i++) {
        Agent agent(i, 0);
        grade9_agents.push_back(agent);
    }

    // Grade 10
    for (auto i = 0; i < Grade10_Population; i++) {
        Agent agent(i, 1);
        grade10_agents.push_back(agent);
    }

    // Grade 11
    for (auto i = 0; i < Grade11_Population; i++) {
        Agent agent(i, 2);
        grade11_agents.push_back(agent);
    }

    // Grade 12
    for (auto i = 0; i < Grade12_Population; i++) {
        Agent agent(i, 3);
        grade12_agents.push_back(agent);
    }
}

int main() {

    // Create a bunch of agents in each grade
    log("Populating agent vectors.");
    populate_agent_vectors();

    // Create a network among the people in each grade (scale free network)
    log("Creating scale free network for each grade");
    watts_strogatz_in_vector(grade9_agents);
    watts_strogatz_in_vector(grade10_agents);
    watts_strogatz_in_vector(grade11_agents);
    watts_strogatz_in_vector(grade12_agents);

    // Create some (very few) connections for people in different grades
    log("Creating connections between different grades");
    watts_strongatz_between_vectors(grade9_agents, grade10_agents);
    watts_strongatz_between_vectors(grade10_agents, grade11_agents);
    watts_strongatz_between_vectors(grade11_agents, grade12_agents);


    return 0;
}