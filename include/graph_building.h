//
// Created by david on 27/03/19.
//

#ifndef C_AGENTBASEDMODEL_GRAPH_BUILDING_H
#define C_AGENTBASEDMODEL_GRAPH_BUILDING_H

#include "agent.h"

struct agent_vector_pair {
    std::vector<Agent> agent_vector_1;
    std::vector<Agent> agent_vector_2;
};

void watts_strogatz_in_vector(std::vector<Agent> &agent_vector);

void watts_strongatz_between_vectors(std::vector<Agent> &agent_vector_1, std::vector<Agent> &agent_vector_2);

#endif //C_AGENTBASEDMODEL_GRAPH_BUILDING_H
