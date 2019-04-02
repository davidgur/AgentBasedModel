/**
 * @file graph_building.cpp
 *
 * @brief Functions that are used in the creation of a graph from a vector of Agents
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include <random>
#include <algorithm>
#include "../include/graph_building.h"


void watts_strogatz_in_vector(std::vector<Agent> &agent_vector) {
    const double beta = 4.0 / agent_vector.size();

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Connect each agent to it's previous and next neighbour. This is a ring lattice
    for (size_t i = 1; i < agent_vector.size() - 1; i++) {
        agent_vector[i].add_to_connections(&agent_vector[i - 1]);
        agent_vector[i].add_to_connections(&agent_vector[i + 1]);
    }

    agent_vector[0].add_to_connections(&agent_vector[agent_vector.size()]);
    agent_vector[agent_vector.size() - 1].add_to_connections(&agent_vector[0]);

    // Create some random connections
    for (auto &source : agent_vector) {
        for (auto &target : agent_vector) {
            bool target_in_source = std::find(source.connections.begin(), source.connections.end(), &target) !=
                                    source.connections.end();
            bool source_is_target = &source == &target;

            bool generate_link = (distribution(generator) < beta) && !source_is_target && !target_in_source;
            if (generate_link) {
                source.add_to_connections(&target);
                target.add_to_connections(&source);
            }
        }
    }
}

void random_connections_between_grades(std::vector<Agent> &agent_vector_1, std::vector<Agent> &agent_vector_2) {
    const double beta = 0.2 / (((double) agent_vector_1.size() + (double) agent_vector_2.size()) / 2);

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    for (auto &source : agent_vector_1) {
        for (auto &target : agent_vector_2) {
            bool target_in_source = std::find(source.connections.begin(), source.connections.end(), &target) !=
                                    source.connections.end();

            bool generate_link = (distribution(generator) < beta) && !target_in_source;
            if (generate_link) {
                source.add_to_connections(&target);
                target.add_to_connections(&source);
            }
        }
    }
}
