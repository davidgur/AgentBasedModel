//
// Created by david on 3/27/2019.
//

#include <iostream>
#include "../include/agent.h"

Agent::Agent() {
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

    this->exposed_day_count = 0;
    this->infected_day_count = 0;

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
    if (this->exposed and this->exposed_day_count < 12) {
        this->exposed_day_count++;
    } else if (this->exposed and this->exposed_day_count == 12) {
        this->exposed = false;
        this->exposed_day_count = 0;

        this->infected = true;
    } else if (this->infected and this->infected_day_count < 5) {
        this->infected_day_count++;
    } else if (this->infected and this->infected_day_count == 5) {
        this->infected = false;
        this->recovered = true;
        this->infected_day_count = 0;
    }
}

void Agent::process_washroom_needs() {}

void Agent::interact_with_friend_random() {}

void Agent::resolve_classroom() {}

Agent::~Agent()=default;
