//
// Created by david on 4/1/2019.
//

#ifndef C_AGENTBASEDMODEL_SIMULATION_H
#define C_AGENTBASEDMODEL_SIMULATION_H


#include <chrono>
#include <map>
#include <array>
#include "agent.h"

/*
 * classroom_population example:
 * "303": {
 *  {Agents who are in the class in period1},
 *  {Agents who are in the class in period2},
 *  {Agents who are in the class in period3},
 *  {Agents who are in the class in period4},
 *  {Agents who are in the class in period5},
 * }
 */


typedef std::map<std::string, std::array<std::vector<Agent *>, 5>> classroom_population;

class Simulation {
public:
    static const int GRADE_9_POPULATION = 292;
    static const int GRADE_10_POPULATION = 356;
    static const int GRADE_11_POPULATION = 372;
    static const int GRADE_12_POPULATION = 334;

    unsigned int day_counter;
    unsigned int day_limit;
    int current_period;

    unsigned short day_state;
    unsigned int minute_counter;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    std::vector<Agent> grade9_agents;
    std::vector<Agent> grade10_agents;
    std::vector<Agent> grade11_agents;
    std::vector<Agent> grade12_agents;

    // Classroom
    classroom_population classrooms;

    void initialize_simulation();

    void start_simulation();

    void individual_disease_progression_for_all();

    void interaction_among_friends_for_all();

    void process_washroom_needs_for_all();

    void resolve_classroom_for_all();

    static void individual_disease_progression(const std::vector<Agent> &agent_vector);

    static void process_washroom_needs(const std::vector<Agent> &agent_vector);

    static void interaction_among_friends(const std::vector<Agent> &agent_vector);

    static void resolve_classroom(const std::vector<Agent> &agent_vector);

    static void export_agent_data(const std::vector<Agent> &agent_vector, std::string file_name);

    void log(std::string to_print);

    void set_day_limit(unsigned int day_limit);

    void determine_classroom_population();

    unsigned short determine_day_state();

    short determine_period();

    Simulation();

    ~Simulation();

private:
    void populate_agent_vector();
};


#endif //C_AGENTBASEDMODEL_SIMULATION_H
