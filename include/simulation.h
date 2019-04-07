/**
 * @file simulation.h
 *
 * @brief Header file outlining Simulation class and associated types
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */
#pragma once

#include <chrono>
#include <map>
#include <array>
#include <random>
#include <fstream>

#include "agent.h"

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

#define FRIENDS_PROBABILITY 1
#define CLASS_PROBABILITY 1

typedef std::map<std::string, std::array<std::vector<Agent *>, 5>> classroom_population;

#ifndef C_AGENTBASEDMODEL_SIMULATION_H
#define C_AGENTBASEDMODEL_SIMULATION_H

class Simulation {
public:
    static const int GRADE_9_POPULATION = 292;
    static const int GRADE_10_POPULATION = 356;
    static const int GRADE_11_POPULATION = 372;
    static const int GRADE_12_POPULATION = 334;

    unsigned int day_counter;
    unsigned int day_limit;
    int current_period;

    std::array<std::string, 7> week = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

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
    std::uniform_real_distribution<double> dist;

    classroom_population classrooms;

    std::ofstream population_out;

    void initialize_simulation();

    void start_simulation();

    void individual_disease_progression_for_all();

    void interaction_among_friends_for_all();

    void process_washroom_needs_for_all();

    void resolve_classroom_for_all(int current_period);

    void print_population_sizes();

    void clear_existing_data();

    void individual_disease_progression(std::vector<Agent> &agent_vector);

    static void process_washroom_needs(std::vector<Agent> &agent_vector);

    void interaction_among_friends(std::vector<Agent> &agent_vector);

    void resolve_classroom(std::vector<Agent> &agent_vector, int current_period);

    static void export_agent_data(std::vector<Agent> &agent_vector, std::string file_name);

    void log(std::string to_print);

    void set_day_limit(unsigned int day_limit);

    void determine_classroom_population();

    void pick_random_sick();

    unsigned short determine_day_state();

    short determine_period();

    std::array<int, 5> get_population_sizes(std::vector<Agent> &agent_vector);

    Simulation();

    ~Simulation();

private:
    void populate_agent_vector();
};


#endif //C_AGENTBASEDMODEL_SIMULATION_H
