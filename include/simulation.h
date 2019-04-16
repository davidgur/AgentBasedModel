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

#ifndef C_AGENTBASEDMODEL_SIMULATION_H
#define C_AGENTBASEDMODEL_SIMULATION_H

#include <chrono>
#include <map>
#include <array>
#include <random>
#include <fstream>
#include <thread>

#include "agent.h"
#include "json.hpp"

#define PRE_CLASS     500   // From 0 to 500 minutes, students are at home
#define PERIOD1_START 520
#define PERIOD1_END   600
#define PERIOD2_START 605
#define PERIOD2_END   680
#define PERIOD3_START 685
#define PERIOD3_END   760
#define PERIOD4_START 675
#define PERIOD4_END   840
#define PERIOD5_START 845
#define PERIOD5_END   920
#define POST_CLASS    940   // From 940 minutes until (24 * 60), students are at home

#define FRIENDS_PROBABILITY 5.75 / 20.0
#define CLASS_PROBABILITY 5.75 / 75.0
#define CONCENTRATION_DECAY_RATE 0.62578

typedef std::map<std::string, std::array<std::vector<Agent *>, 5>> classroom_population;

class Simulation {
public:
    static const int GRADE_9_POPULATION = 292;
    static const int GRADE_10_POPULATION = 356;
    static const int GRADE_11_POPULATION = 372;
    static const int GRADE_12_POPULATION = 334;

    int sim_id;
    double vacc_rate;
    std::string export_folder;

    unsigned int day_counter;
    unsigned int day_limit;
    int current_period;

    std::array<std::string, 7> week = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    std::vector<double> school_washrooms = {0, 0, 0, 0, 0, 0};
    std::vector<nlohmann::json> classes;

    unsigned short day_state;
    long minute_counter;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_day;

    std::vector<Agent> grade9_agents;
    std::vector<Agent> grade10_agents;
    std::vector<Agent> grade11_agents;
    std::vector<Agent> grade12_agents;

    std::random_device rd;
    std::mt19937 mt;

    classroom_population classrooms;

    std::ofstream population_out;

    void initialize_simulation();

    std::thread start_simulation_thread();

    void start_simulation();

    void individual_disease_progression_for_all();

    void interaction_among_friends_for_all();

    void process_washroom_needs_for_all();

    void resolve_classroom_for_all(int current_period);

    void print_population_sizes();

    void individual_disease_progression(std::vector<Agent> &agent_vector);

    void process_washroom_needs(std::vector<Agent> &agent_vector);

    void interaction_among_friends(std::vector<Agent> &agent_vector);

    void resolve_classroom(std::vector<Agent> &agent_vector, int current_period);

    void export_agent_data(std::vector<Agent> &agent_vector, std::string file_name);

    void log(std::string to_print);

    void set_day_limit(unsigned int day_limit);

    void determine_classroom_population();

    void prep_output_file();

    void pick_random_sick();

    void decay_washroom_concentration();

    void clean_washrooms();

    bool check_for_steady();

    void create_vaccinated();

    unsigned short determine_day_state();

    short determine_period();

    std::array<int, 5> get_population_sizes(std::vector<Agent> &agent_vector);

    std::vector<nlohmann::json> load_classes();

    Simulation(int sim_id, double vacc_rate);

    ~Simulation();

private:
    void populate_agent_vector();
};


#endif //C_AGENTBASEDMODEL_SIMULATION_H
