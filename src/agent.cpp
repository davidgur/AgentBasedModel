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

    this->mt = std::mt19937(std::random_device{}());
    this->going_to_wr = std::discrete_distribution<bool>{1 - PROBABILITY_OF_WASHROOM, PROBABILITY_OF_WASHROOM};
    this->washroom = std::uniform_int_distribution<int>(0, 6);
    this->stoch_range = std::uniform_int_distribution<int>(-48 * 60, 48 * 60);
    this->infection_prob_no_vacc = std::discrete_distribution<bool>{1 - PROBABILITY_OF_INFECTION,
                                                                    PROBABILITY_OF_INFECTION};
    this->infection_prob_yes_vacc = std::discrete_distribution<bool>(1 - PROBABILITY_OF_INFECTION_VACC,
                                                                     PROBABILITY_OF_INFECTION_VACC);
}

void Agent::add_to_connections(Agent *new_agent) {
    this->connections.push_back(new_agent);
}

void Agent::individual_disease_progression() {
    if (this->exposed && this->exposed_minute_count < (EXPOSED_DAY_COUNT * MINUTES_PER_DAY)) {
        this->exposed_minute_count++;

    } else if (this->exposed && this->exposed_minute_count == (EXPOSED_DAY_COUNT * MINUTES_PER_DAY)) {

        this->exposed = false;
        this->exposed_minute_count = 0;
        this->infected = true;
        this->infected_minute_count = this->stoch_range(mt);

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
    bool should_infect = this->infection_prob_no_vacc(mt);
    bool should_infect_vacc = this->infection_prob_yes_vacc(mt);

    // Check if the other agent is infectious
    if (other_agent.infected && this->susceptible && should_infect) {
        this->susceptible = false;
        this->exposed = true;
        this->exposed_minute_count = this->stoch_range(mt);    // Random value to add some stochasticity
    } else if (other_agent.infected && this->vaccinated && should_infect_vacc) {
        this->vaccinated = false;
        this->exposed = true;
        this->exposed_minute_count = this->stoch_range(mt);
    } else if (other_agent.susceptible && this->infected && should_infect) {
        other_agent.susceptible = false;
        other_agent.exposed = true;
        other_agent.exposed_minute_count = this->stoch_range(mt); // Random value to add some stochasticity
    } else if (other_agent.vaccinated && this->infected && should_infect_vacc) {
        other_agent.vaccinated = false;
        other_agent.exposed = true;
        other_agent.exposed_minute_count = this->stoch_range(mt);

    }
}

void Agent::process_washroom_needs(std::vector<double> &school_washrooms) {
    if (this->going_to_wr(mt) && !this->at_home) {
        int wr_choice = this->washroom(mt);

        // If the student is infectious, they should infect the washroom a bit
        if (this->infected) {
            school_washrooms[wr_choice] += CONCENTRATION_INCREASE;
        } else if (this->susceptible) {
            std::discrete_distribution<bool> infect_from_wr{
                    1 - (school_washrooms[wr_choice] * PULMINARY_VENTILATION * AVERAGE_WR_TIME),
                    (school_washrooms[wr_choice] * PULMINARY_VENTILATION * AVERAGE_WR_TIME)};
            bool infect_from_washroom = infect_from_wr(mt);
            if (infect_from_washroom) {
                this->susceptible = false;
                this->exposed = true;
                this->exposed_minute_count = stoch_range(mt);
            }
        }
    }
}

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
