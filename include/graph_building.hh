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

#ifndef AGENTBASEDMODEL_INCLUDE_GRAPH_BUILDING_HH
#define AGENTBASEDMODEL_INCLUDE_GRAPH_BUILDING_HH

#include "agent.hh"

#include <random>
#include <algorithm>

const int kAverageNumOfFriends = 5.0;
const double kProbabilityOfFriendsOutsideOfGrade = 1.0 / 5.0;

void watts_strogatz_in_vector(std::vector<Agent> &agent_vector);
void random_connections_between_grades(std::vector<Agent> &agent_vector_1, std::vector<Agent> &agent_vector_2);

#endif // AGENTBASEDMODEL_INCLUDE_GRAPH_BUILDING_HH