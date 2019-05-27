/* Modelling the spread of measles within secondary schools
 *
 * Copyright (c) 2019 David Gurevich
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to
 * deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom
 * the Software is furnised to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: david@gurevich.ca (David Gurevich)
 */

#include "../include/agent.hh"
#include "../include/random_element.hh"

#include <iostream>
#include <random>
#include <iterator>

Agent::Agent(int id, int grade, bool ode_mode) {
    this->id = id;
    this->grade = grade;
    this->ode_mode = ode_mode;
    this->secondary_infections = 0;

    // SVEIR info; everyone starts out susceptible
    this->susceptible = true;
    this->vaccinated = false;
    this->exposed = false;
    this->infected = false;
    this->recovered = false;

    // School info
    this->at_home = false;

    this->p1 = "";
    this->p2 = "";
    this->p3 = "";
    this->p4 = "";
    this->p5 = "";

    // Random number generation
    this->mt = std::mt19937(std::random_device{}());
    
    this->going_to_washroom = std::bernoulli_distribution(kWashroomProbability);
    this->washroom_choice = std::uniform_int_distribution<int>(0, 6);

    this->stoch_range = std::normal_distribution<>(0, kMinutesPerDay);
    this->stoch_range_ode = std::exponential_distribution<double>(1.0 / 720.0); // mean (0, 24 * 60)
    this->infection_prob = std::bernoulli_distribution(kTransmissionRate);
    this->infection_prob_washroom = std::bernoulli_distribution(kWashroomInfectionRate);

    // Absence days
    this->symptoms_absence_minutes = this->generate_truncated_normal_distribution_value() + (4 * kMinutesPerDay);
}

void Agent::add_to_connections(Agent* new_agent) {
    this->connections.push_back(new_agent);
}

void Agent::generate_lunch_friends(class_map& classrooms) {
    // First, look for any agents that are connections that have the same lunch
    for (auto& agent : this->connections) {
        if (agent->lunch_period == this->lunch_period){
            this->lunch_friends.push_back(agent);
            agent->lunch_friends.push_back(this);
        }
    }
}

void Agent::individual_disease_progression() {
    // Cases:
    //  - Agent is susceptible --> Do nothing
    //  - Agent is exposed but hasn't met threshold --> Increase counter
    //  - Agent is exposed and has met threshold --> convert to infected
    //  - Agent is infected but hasn't met the threshold to go home --> increase counter
    //  - Agent is infected and has met the threshold to go home --> increase counter, agent is at home
    //  - Agent is infected and has met the threshold to recover --> agent isn't at home, agent recovers

    if (this->exposed and this->exposed_minute_count < (kExposedDayCount * kMinutesPerDay)) {
        this->exposed_minute_count++;
    }

    else if (this->exposed and this->exposed_minute_count == (kExposedDayCount * kMinutesPerDay)) {
        this->infected = true;
        this->exposed = false;
        if (this->ode_mode)
            this->infected_minute_count = stoch_range_ode(mt);
        else
            this->infected_minute_count = this->generate_truncated_normal_distribution_value();
    }

    else if (this->infected and this->infected_minute_count == (kSymptomsAbsenceDays * kMinutesPerDay)) {
        this->at_home = true;
        this->infected_minute_count++;
    }

    else if (this->infected and this->infected_minute_count < (kInfectedDayCount * kMinutesPerDay)) {
        this->infected_minute_count++;
    }

    else if (this->infected and this->infected_minute_count == (kInfectedDayCount * kMinutesPerDay)) {
        this->at_home = false;
        this->recovered = true;
        this->infected = false;
    }

}

void Agent::process_washroom_needs(std::vector<double>& school_washrooms) {
    // Is the student going to the washroom?
    if (this->going_to_washroom(this->mt) and !this->at_home) {
        // Pick a washroom:
        int washroom_destination = this->washroom_choice(this->mt);


        // If agent is susceptible, then they might get infected from the washroom
        bool infect_from_wr = (rand() % 100) < (kWashroomInfectionRate * kPulmonaryVentilation * kAverageWashroomTime * school_washrooms[washroom_destination] * 100);
        if (infect_from_wr and this->susceptible)
            this->get_infected();


        // If the agent is infected, then they will further infect the washroom
        else if (this->infected) {
            school_washrooms[washroom_destination] += kWashroomConcentrationIncrease;
        }
    }
}

void Agent::interact_with_friend_random(int sim_time) {
    this->interact(**random_element(this->connections.begin(), this->connections.end()), sim_time);
}

void Agent::resolve_classroom(int current_period, class_map& classrooms, int sim_time) {
    switch (current_period) {
        case 1:
            this->interact(**random_element(classrooms[this->p1][0].begin(), classrooms[this->p1][0].end()), sim_time);
            break;
        case 2:
            this->interact(**random_element(classrooms[this->p2][1].begin(), classrooms[this->p2][1].end()), sim_time);
            break;
        case 3:
            this->interact(**random_element(this->lunch_friends.begin(), this->lunch_friends.end()), sim_time);
            break;
        case 4:
            this->interact(**random_element(classrooms[this->p4][3].begin(), classrooms[this->p4][3].end()), sim_time);
            break;
        case 5:
            this->interact(**random_element(classrooms[this->p5][4].begin(), classrooms[this->p5][4].end()), sim_time);
            break;
        default:
            break;
    }
}

void Agent::interact(Agent& other_agent, int sim_time) {
    bool should_infect = this->infection_prob(this->mt);

    // If the other agent is infectious
    if (other_agent.infected and this->susceptible and should_infect) {
        this->get_infected();
        this->time_of_infection = sim_time;
        other_agent.secondary_infections += 1;
    }
    // If this agent is infectious
    else if (other_agent.susceptible and this->infected and should_infect) {
        other_agent.get_infected();
        other_agent.time_of_infection = sim_time;
        this->secondary_infections += 1;
    }
}

int Agent::generate_truncated_normal_distribution_value() {
    int return_value = 0;

    for (;;) {
        return_value = this->stoch_range(mt);
        if (-kMinutesPerDay <= return_value or return_value <= kMinutesPerDay)
            break;
    }

    return static_cast<int>(return_value);
}

void Agent::get_infected() {
    this->susceptible = false;
    this->vaccinated = false;
    this->exposed = true;
    this->infected = false;
    this->recovered = false;

    if (this->ode_mode)
        this->exposed_minute_count = stoch_range_ode(mt);
    else
        this->exposed_minute_count = this->generate_truncated_normal_distribution_value();
}
