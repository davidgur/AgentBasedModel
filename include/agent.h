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


#include <vector>
#include <string>

class Agent {
public:
    Agent();
    Agent(int id, int grade);
    ~Agent();

    int id;
    int grade;

    int exposed_day_count;
    int infected_day_count;

    bool exposed;
    bool infected;
    bool recovered;

    bool vaccinated;

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

    void resolve_classroom();
};


#endif //C_AGENTBASEDMODEL_AGENT_H
