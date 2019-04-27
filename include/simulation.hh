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

#ifndef AGENTBASEDMODEL_INCLUDE_SIMULATION_HH
#define AGENTBASEDMODEL_INCLUDE_SIMULATION_HH

#include "agent.hh"
#include "graph_building.hh"
#include "configure_agents.hh"
#include "random_element.hh"

#include <iostream>
#include <chrono>
#include <map>
#include <algorithm>
#include <array>
#include <vector>
#include <fstream>
#include <thread>
#include <sys/stat.h>

// Population sizes (http://www.yrdsb.ca/AboutUs/DirectorsAnnualReport/Documents/2018-19-Sec.pdf)
const int kGrade9Population = 292;
const int kGrade10Population = 356;
const int kGrade11Population = 372;
const int kGrade12Population = 334;

// Period times
const int kPreClass = 500;
const int kPeriod1Start = 520;
const int kPeriod1End = 600;
const int kPeriod2Start = 605;
const int kPeriod2End = 680;
const int kPeriod3Start = 685;
const int kPeriod3End = 760;
const int kPeriod4Start = 765;
const int kPeriod4End = 840;
const int kPeriod5Start = 845;
const int kPeriod5End = 920;
const int kPostClass = 940;

// Interaction probabilities
const double kFriendsInteractionProbability = 1.5 / 20.0;
const double kClassInteractionProbability = 6.0 / 75.0;

// Washroom
const double kConcentrationDecayRate = 0.62578;

// ODE Mode
const bool kODEMode = false;

// Days of the week
const std::array<std::string, 7> kWeek = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

class Simulation {
public:
    // Constructor / Destructor
    Simulation(int sim_id, double vacc_rate);
    ~Simulation()=default;

    // Critical simulation information
    int sim_id;
    double vacc_rate;

    // Classroom management
    std::map<std::string, std::array<std::vector<Agent*>, 5>> classrooms;

    // School Washrooms
    std::vector<double> school_washrooms = {0, 0, 0, 0, 0, 0};

    // Public Functions
    std::thread start_simulation_thread();

private:
    // Time-keeping
    unsigned int day_counter;
    long long minute_counter = 0;

    unsigned short day_state;
    unsigned short current_period;

    // Export
    std::string export_folder;
    std::ofstream population_out;

    // Agent vectors
    std::vector<Agent> grade9_agents;
    std::vector<Agent> grade10_agents;
    std::vector<Agent> grade11_agents;
    std::vector<Agent> grade12_agents;

    // Clock
    std::chrono::time_point<std::chrono::high_resolution_clock> last_day;

    // Random Number Generation
    std::mt19937 mt;
    std::bernoulli_distribution friends_distribution;
    std::bernoulli_distribution classroom_distribution;
    std::uniform_int_distribution<int> random_sick_grade;

    // Private Functions
    void initialize_simulation();
    void run_simulation();
    void populate_agent_vector();

    void individual_disease_progression_for_all();
    void interaction_among_friends_for_all();
    void process_washroom_needs_for_all();
    void resolve_classroom_for_all();

    void individual_disease_progression(std::vector<Agent> &agent_vector);
    void interaction_among_friends(std::vector<Agent> &agent_vector);
    void process_washroom_needs(std::vector<Agent> &agent_vector);
    void resolve_classroom(std::vector<Agent> &agent_vector);

    void log();
    void prep_output_file();
    void print_population_sizes();
    void export_agent_data(std::vector<Agent> &agent_vector, std::string file_name);
    void determine_classroom_population();
    std::vector<int> get_population_sizes(std::vector<Agent> &agent_vector);

    void create_vaccinated();
    void pick_random_sick();
    void decay_washroom_concentration();
    void clean_washrooms();
    bool check_for_steady();

    unsigned short determine_day_state();
    unsigned short determine_period();
};

#endif // AGENTBASEDMODEL_INCLUDE_SIMULATION_HH