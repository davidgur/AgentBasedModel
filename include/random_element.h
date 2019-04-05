//
// Created by david on 4/4/2019.
//

#ifndef C_AGENTBASEDMODEL_RANDOM_ELEMENT_H
#define C_AGENTBASEDMODEL_RANDOM_ELEMENT_H

#include <random>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter random_element(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

#endif //C_AGENTBASEDMODEL_RANDOM_ELEMENT_H
