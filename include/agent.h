//
// Created by david on 3/27/2019.
//

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

    bool exposed;
    bool infected;
    bool recovered;

    bool vaccinated;

    bool fast_tracking;

    std::vector<Agent*> connections;

    std::string p1;
    std::string p2;
    std::string p3;
    std::string p4;
    std::string p5;

    void add_to_connections(Agent *new_agent);
};


#endif //C_AGENTBASEDMODEL_AGENT_H
