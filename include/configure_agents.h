/**
 * @file graph_building.h
 *
 * @brief Header file outlining all functions associated with configure_agents.cpp
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#ifndef C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
#define C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H

#include "agent.h"
#include "json.hpp"

void assign_student_timetables(std::vector<Agent> &agent_vector,
                               std::vector<nlohmann::json> &classes,
                               std::string grade_id);


#endif //C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
