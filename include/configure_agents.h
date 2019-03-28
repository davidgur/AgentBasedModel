//
// Created by david on 27/03/19.
//

#ifndef C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
#define C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H

#include "agent.h"

std::vector<Agent> place_students_in_classes(std::vector<Agent> agent_vector, int grade);

std::vector<Agent> create_fast_track(std::vector<Agent> agent_vector, int grade);


#endif //C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
