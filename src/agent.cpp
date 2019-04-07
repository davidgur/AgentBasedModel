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
	
    this->at_home = false;

    this->exposed_minute_count = 0;
    this->infected_minute_count = 0;
}

Agent::Agent(int id, int grade) {
    this->id = id;
    this->grade = grade;

    this->susceptible = true;
    this->vaccinated = false;
    this->exposed = false;
    this->infected = false;
    this->recovered = false;

    this->at_home = false;

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
    if (this->exposed && this->exposed_minute_count < (EXPOSED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->exposed_minute_count++;
    } else if (this->exposed && this->exposed_minute_count == (EXPOSED_DAY_COUNT * MINUTES_PER_DAY)) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<long long> stoch_range(-24 * 60, 24 * 60);

        this->exposed = false;
        this->exposed_minute_count = 0;
        this->infected = true;
        this->infected_minute_count = stoch_range(mt);
	} else if (this->infected && this->infected_minute_count == (1 * MINUTES_PER_DAY)) {
	    this->at_home = true;
        this->infected_minute_count++;

    } else if (this->infected && this->infected_minute_count < (INFECTED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->infected_minute_count++;

    } else if (this->infected && this->infected_minute_count == (INFECTED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->infected = false;
	    this->at_home = false;
        this->recovered = true;
        this->infected_minute_count = 0;
    }
}

void Agent::interact(Agent &other_agent) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_int_distribution<long long> stoch_range(-24 * 60, 24 * 60);

    bool should_infect = dist(mt) < PROBABILITY_OF_INFECTION;

    // Check if the other agent is infectious
    if (other_agent.infected && this->susceptible && should_infect) {
        this->susceptible = false;
        this->exposed = true;
        this->exposed_minute_count = stoch_range(mt);    // Random value to add some stochasticity
    } else if (other_agent.susceptible && this->infected && should_infect) {
        other_agent.susceptible = false;
        other_agent.exposed = true;
        other_agent.exposed_minute_count = stoch_range(mt); // Random value to add some stochasticity
    }
}

void Agent::process_washroom_needs() {}

void Agent::interact_with_friend_random() {
    this->interact(**random_element(this->connections.begin(), this->connections.end()));
}

void
Agent::resolve_classroom(int current_period, std::map<std::string, std::array<std::vector<Agent *>, 5>> &classrooms) {
    // TODO: Add special circumstances for more disease-prone classes, such as gym or lunch.
    switch (current_period) {
        case 1:
            if (this->p1 == "LUNCH")
                this->interact(**random_element(this->connections.begin(), this->connections.end()));
            else {
                this->interact(
                        **random_element(classrooms[this->p1][0].begin(), classrooms[this->p1][0].end())
                );
                break;
            }
        case 2:
            if (this->p2 == "LUNCH")
                this->interact(**random_element(this->connections.begin(), this->connections.end()));
            else {
                this->interact(
                        **random_element(classrooms[this->p2][1].begin(), classrooms[this->p2][1].end())
                );
                break;
            }
        case 3:
            if (this->p3 == "LUNCH")
                this->interact(**random_element(this->connections.begin(), this->connections.end()));
            else {
                this->interact(
                        **random_element(classrooms[this->p3][2].begin(), classrooms[this->p3][2].end())
                );
                break;
            }
        case 4:
            if (this->p4 == "LUNCH")
                this->interact(**random_element(this->connections.begin(), this->connections.end()));
            else {
                this->interact(
                        **random_element(classrooms[this->p4][3].begin(), classrooms[this->p4][3].end())
                );
                break;
            }
        case 5:
            if (this->p5 == "LUNCH")
                this->interact(**random_element(this->connections.begin(), this->connections.end()));
            else {
                this->interact(
                        **random_element(classrooms[this->p5][4].begin(), classrooms[this->p5][4].end())
                );
                break;
            }
        default:
            this->interact(**random_element(this->connections.begin(), this->connections.end()));
            break;
    }
}


Agent::~Agent() = default;
