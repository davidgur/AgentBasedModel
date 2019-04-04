/**
 * @file graph_building.h
 *
 * @brief Header file outlining all functions associated with graph_building.cpp
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#ifndef C_AGENTBASEDMODEL_GRAPH_BUILDING_H
#define C_AGENTBASEDMODEL_GRAPH_BUILDING_H

#include "agent.h"

void watts_strogatz_in_vector(std::vector<Agent> &agent_vector);

void random_connections_between_grades(std::vector<Agent> &agent_vector_1, std::vector<Agent> &agent_vector_2);

#endif //C_AGENTBASEDMODEL_GRAPH_BUILDING_H
