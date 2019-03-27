//
// Created by david on 3/27/2019.
//

#include <iostream>
#include "agent.h"

Agent::Agent() {
    std::cout << "Default Agent created" << std::endl;

    this->exposed = false;
    this->vaccinated = false;
    this->grade = 0;
    this->id = 0;
    this->infected = false;
    this->recovered = false;
}

Agent::Agent(int id, int grade) {
    this->id = id;
    this->grade = grade;

    this->vaccinated = false;
    this->exposed = false;
    this->infected = false;
    this->recovered = false;
    std::cout << "Agent with ID: " << this->id << " in grade " << this->grade << " created." << std::endl;
}

void Agent::add_to_connections(Agent *new_agent) {
    this->connections.push_back(new_agent);
}

Agent::~Agent()=default;
