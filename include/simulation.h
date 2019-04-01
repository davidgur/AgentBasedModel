//
// Created by david on 4/1/2019.
//

#ifndef C_AGENTBASEDMODEL_SIMULATION_H
#define C_AGENTBASEDMODEL_SIMULATION_H


#include <chrono>
#include "agent.h"

class Simulation {
public:
    static const int GRADE_9_POPULATION = 292;
    static const int GRADE_10_POPULATION = 356;
    static const int GRADE_11_POPULATION = 372;
    static const int GRADE_12_POPULATION = 334;

    unsigned int day_counter;
    unsigned int day_limit;

    unsigned short day_state;

    // Second counter counts the minutes in the day
    unsigned int minute_counter;

    std::vector<Agent> grade9_agents;
    std::vector<Agent> grade10_agents;
    std::vector<Agent> grade11_agents;
    std::vector<Agent> grade12_agents;

    //std::vector<Classroom> classrooms;

    void initialize_simulation();

    void start_simulation();

    void individual_disease_progression(std::vector<Agent> agent_vector);

    void individual_disease_progression_for_all();

    void interaction_among_friends_for_all();

    void process_washroom_needs_for_all();

    void resolve_classroom_for_all();

    void process_washroom_needs(std::vector<Agent> const &agent_vector);

    void interaction_among_friends(std::vector<Agent> const &agent_vector);

    void resolve_classroom(std::vector<Agent> const &agent_vector);

    void export_agent_data(std::vector<Agent> agent_vector, std::string file_name);

    void log(std::string to_print);

    void set_day_limit(unsigned int day_limit);

    unsigned short determine_day_state();

    Simulation();

    ~Simulation();

private:
    void populate_agent_vector();
};


#endif //C_AGENTBASEDMODEL_SIMULATION_H
