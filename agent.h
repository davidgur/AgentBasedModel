//
// Created by david on 3/27/2019.
//

#ifndef C_AGENTBASEDMODEL_AGENT_H
#define C_AGENTBASEDMODEL_AGENT_H


#include <vector>

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

    std::vector<Agent*> connections;
    std::vector<int>   courses;

    void add_to_connections(Agent *new_agent);
};


#endif //C_AGENTBASEDMODEL_AGENT_H
