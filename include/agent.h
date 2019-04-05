/**
 * @file agent.h
 *
 * @brief Header file outlining Agent class
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#ifndef C_AGENTBASEDMODEL_AGENT_H
#define C_AGENTBASEDMODEL_AGENT_H

#define MINUTES_PER_DAY (24*60)

#define EXPOSED_DAY_COUNT 12
#define INFECTED_DAY_COUNT 5

#define PROBABILITY_OF_INFECTION 0.5217

#include <vector>
#include <string>
#include <array>
#include <map>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g);

template<typename Iter>
Iter random_element(Iter start, Iter end);

class Agent {
public:
    Agent();
    Agent(int id, int grade);
    ~Agent();

    int id;
    int grade;

    long long exposed_minute_count;
    long long infected_minute_count;

    // SVEIR
    bool susceptible;
    bool vaccinated;
    bool exposed;
    bool infected;
    bool recovered;

    double susceptibility;
    double transmisability;


    std::vector<Agent*> connections;

    std::string p1;
    std::string p2;
    std::string p3;
    std::string p4;
    std::string p5;

    void add_to_connections(Agent *new_agent);

    void individual_disease_progression();

    void process_washroom_needs();

    void interact_with_friend_random();

    void resolve_classroom(int current_period, std::map<std::string, std::array<std::vector<Agent *>, 5>> &classrooms);

    void interact(Agent &other_agent);
};


#endif //C_AGENTBASEDMODEL_AGENT_H
