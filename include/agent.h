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

#ifndef C_AGENTBASEDMODEL_AGENT_H
#define C_AGENTBASEDMODEL_AGENT_H

#define MINUTES_PER_DAY (24*60)

#define EXPOSED_DAY_COUNT 12
#define INFECTED_DAY_COUNT 5

#define PROBABILITY_OF_INFECTION 0.52
#define PROBABILITY_OF_WASHROOM  0.0004

#define CONCENTRATION_INCREASE 28.7878179

#define DAYS_UNTIL_SYMPTOMS 4
#define PULMINARY_VENTILATION 0.00566
#define AVERAGE_WR_TIME 3

#include <vector>
#include <random>
#include <string>
#include <array>
#include <map>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator &g);

template<typename Iter>
Iter random_element(Iter start, Iter end);

class Agent {
public:
    Agent();

    Agent(int id, int grade);

    ~Agent();

    int id = 0;
    int grade = 0;

    long long exposed_minute_count = 0;
    long long infected_minute_count = 0;

    // SVEIR
    bool susceptible = false;
    bool vaccinated = false;
    bool exposed = false;
    bool infected = false;
    bool recovered = false;

    bool at_home = false;

    std::vector<Agent *> connections;

    std::string p1;
    std::string p2;
    std::string p3;
    std::string p4;
    std::string p5;

    std::mt19937 mt;
    std::discrete_distribution<bool> going_to_wr;
    std::uniform_int_distribution<int> washroom;
    std::normal_distribution<> stoch_range;
    std::discrete_distribution<bool> infection_prob;

    void add_to_connections(Agent *new_agent);

    void individual_disease_progression();

    void process_washroom_needs(std::vector<double> &school_washrooms);

    void interact_with_friend_random();

    void resolve_classroom(int current_period, std::map<std::string, std::array<std::vector<Agent *>, 5>> &classrooms);

    void interact(Agent &other_agent);
};


#endif //C_AGENTBASEDMODEL_AGENT_H
