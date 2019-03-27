/*
 * Agent Based Model (ABM) of secondary school.
 *
 *
 * TODO: Figure out a way to create a network between all the "nodes"
 */


#include <iostream>
#include <vector>
#include <random>

#include "agent.h"

const int Grade9_Population  = 292;
const int Grade10_Population = 356;
const int Grade11_Population = 372;
const int Grade12_Population = 334;

std::vector<Agent> grade9_agents;
std::vector<Agent> grade10_agents;
std::vector<Agent> grade11_agents;
std::vector<Agent> grade12_agents;

void populate_agent_vectors() {
    // Grade 9
    for (auto i = 0; i < Grade9_Population; i++) {
        Agent agent(i, 0);
        grade9_agents.push_back(agent);
    }

    // Grade 10
    for (auto i = 0; i < Grade10_Population; i++) {
        Agent agent(i, 1);
        grade10_agents.push_back(agent);
    }

    // Grade 11
    for (auto i = 0; i < Grade11_Population; i++) {
        Agent agent(i, 2);
        grade11_agents.push_back(agent);
    }

    // Grade 12
    for (auto i = 0; i < Grade12_Population; i++) {
        Agent agent(i, 3);
        grade12_agents.push_back(agent);
    }
}

void watts_strogatz_in_vector(std::vector<Agent> agent_vector) {
    const double beta = 0.9;
    const int avg_conn = 5;

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Connect each agent to it's previous and next neighbour. This is a ring lattice
    for (size_t i = 1; i < agent_vector.size() - 1; i++) {
        agent_vector.at(i).add_to_connections(&agent_vector.at(i-1));
        agent_vector.at(i-1).add_to_connections(&agent_vector.at(i));

        agent_vector.at(i).add_to_connections(&agent_vector.at(i+1));
        agent_vector.at(i+1).add_to_connections(&agent_vector.at(i));

        std::cout << "Created links for agent " << agent_vector[i].id << std::endl;
    }

    agent_vector[0].add_to_connections(&agent_vector[agent_vector.size()]);
    agent_vector[agent_vector.size()].add_to_connections(&agent_vector[0]);

    // Create some random connections
    for (auto &source : agent_vector) {
        for (auto &target : agent_vector) {
            bool generate_link = distribution(generator) < beta;
            if (generate_link) {
                source.add_to_connections(&target);
                target.add_to_connections(&source);
                std::cout << "Created link between " << source.id << " and " << target.id << std::endl;
            }
        }
    }
}

int main() {
    populate_agent_vectors();

    watts_strogatz_in_vector(grade9_agents);
    watts_strogatz_in_vector(grade10_agents);
    watts_strogatz_in_vector(grade11_agents);
    watts_strogatz_in_vector(grade12_agents);

    return 0;
}