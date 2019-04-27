/* Modelling the spread of measles within secondary schools
 *
 * Copyright (c) 2019 David Gurevich
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to
 * deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom
 * the Software is furnised to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: david@gurevich.ca (David Gurevich)
 */

#include "../include/graph_building.hh"

void watts_strogatz_in_vector(std::vector<Agent> &agent_vector) {
    const double beta = (kAverageNumOfFriends - 2.0) / agent_vector.size();

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::discrete_distribution<bool> beta_gen{1 - beta, beta};

    // Connect each agent to it's previous and next neighbour. This is a ring lattice
    for (size_t i = 1; i < agent_vector.size() - 1; i++) {
        agent_vector[i].add_to_connections(&agent_vector[i - 1]);
        agent_vector[i].add_to_connections(&agent_vector[i + 1]);
    }

    agent_vector[0].add_to_connections(&agent_vector.back());
    agent_vector[0].add_to_connections(&agent_vector[1]);

    agent_vector[agent_vector.size() - 1].add_to_connections(&agent_vector[0]);

    // Create some random connections
    for (auto &source : agent_vector) {
        for (auto &target : agent_vector) {
            bool target_in_source = std::find(source.connections.begin(), source.connections.end(), &target) !=
                                    source.connections.end();
            bool source_is_target = &source == &target;

            bool generate_link = beta_gen(generator) && !source_is_target && !target_in_source;
            if (generate_link) {
                source.add_to_connections(&target);
                target.add_to_connections(&source);
            }
        }
    }
}

void random_connections_between_grades(std::vector<Agent> &agent_vector_1, std::vector<Agent> &agent_vector_2) {
    const double beta = kProbabilityOfFriendsOutsideOfGrade / (((double) agent_vector_1.size() + (double) agent_vector_2.size()) / 2);

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::discrete_distribution<bool> beta_gen{1 - beta, beta};

    for (auto &source : agent_vector_1) {
        for (auto &target : agent_vector_2) {
            bool target_in_source = std::find(source.connections.begin(), source.connections.end(), &target) !=
                                    source.connections.end();

            bool generate_link = beta_gen(generator) && !target_in_source;
            if (generate_link) {
                source.add_to_connections(&target);
                target.add_to_connections(&source);
            }
        }
    }
}