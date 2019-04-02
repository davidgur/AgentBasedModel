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

void place_student_in_class(std::string *student_period, int grade, int period);

void assign_student_timetables(std::vector<Agent> &agent_vector, std::string grade_id);

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g);

template<typename Iter>
Iter random_element(Iter start, Iter end);


#endif //C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
