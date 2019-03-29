//
// Created by david on 27/03/19.
//

#ifndef C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
#define C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H

#include "agent.h"

void place_student_in_class(std::string *student_period, int grade, int period);

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g);

template<typename Iter>
Iter random_element(Iter start, Iter end);


#endif //C_AGENTBASEDMODEL_CONFIGURE_AGENTS_H
