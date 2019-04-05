/**
 * @file agent.cpp
 *
 * @brief File containing all functions in the Agent class
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include <iostream>
#include <random>
#include <iterator>

#include "../include/agent.h"
#include "../include/random_element.h"

Agent::Agent() {
    this->grade = 0;
    this->id = 0;

    this->susceptible = true;
    this->vaccinated = false;
    this->exposed = false;
    this->infected = false;
    this->recovered = false;
}

Agent::Agent(int id, int grade) {
    this->id = id;
    this->grade = grade;

    this->susceptible = true;
    this->vaccinated = false;
    this->exposed = false;
    this->infected = false;
    this->recovered = false;

    this->exposed_minute_count = 0;
    this->infected_minute_count = 0;

    this->p1 = "";
    this->p2 = "";
    this->p3 = "";
    this->p4 = "";
    this->p5 = "";
}

void Agent::add_to_connections(Agent *new_agent) {
    this->connections.push_back(new_agent);
}

void Agent::individual_disease_progression() {
    if (this->exposed and this->exposed_minute_count < (EXPOSED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->exposed_minute_count++;
    } else if (this->exposed and this->exposed_minute_count == (EXPOSED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->exposed = false;
        this->exposed_minute_count = 0;
        this->infected = true;
    } else if (this->infected and this->infected_minute_count < (INFECTED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->infected_minute_count++;
    } else if (this->infected and this->infected_minute_count == (INFECTED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->infected = false;
        this->recovered = true;
        this->infected_minute_count = 0;
    }
}

void Agent::interact(Agent &other_agent) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_int_distribution<int> minutes_in_day(0, 24 * 60);

    bool should_infect = dist(mt) < PROBABILITY_OF_INFECTION;

    // Check if the other agent is infectious
    if (other_agent.infected and this->susceptible and should_infect) {
        this->susceptible = false;
        this->exposed = true;
        this->exposed_minute_count = minutes_in_day(mt);    // Random value to add some stochasticity
    } else if (other_agent.susceptible and this->infected and should_infect) {
        other_agent.susceptible = false;
        other_agent.exposed = true;
        other_agent.exposed_minute_count = minutes_in_day(mt); // Random value to add some stochasticity
    }
}

void Agent::process_washroom_needs() {}

void Agent::interact_with_friend_random() {
    this->interact(**random_element(this->connections.begin(), this->connections.end()));
}

void Agent::resolve_classroom() {}

Agent::~Agent()=default;
