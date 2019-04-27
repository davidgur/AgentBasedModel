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

#ifndef AGENTBASEDMODEL_INCLUDE_AGENT_HH
#define AGENTBASEDMODEL_INCLUDE_AGENT_HH

#include <vector>
#include <random>
#include <string>
#include <array>
#include <map>

// CONSTANTS (as related to agents)
const int kMinutesPerDay = 24 * 60;
const int kExposedDayCount = 11;    // } [1]
const int kInfectedDayCount = 6;    // } [1]
const int kSymptomsAbsenceDays = 4; // } [1]
const int kAverageWashroomTime = 3;
const int kMinLunchFriends = 3;

const double kTransmissionRate = 0.52; // Calculated based on R0
const double kWashroomInfectionRate = kTransmissionRate / 100.0;
const double kWashroomProbability = 0.0004; // Own calculations
const double kWashroomConcentrationIncrease = 28.7878179;
const double kPulmonaryVentilation = 0.00566;

const std::string kLunchVar = "LUNCH";


class Agent {
  public:
    // Constructor / Destructor
    Agent(int id, int grade, bool ode_mode);
    ~Agent()=default;

    // Agent identification and critical information
    int id;
    int grade;
    int lunch_period;

    bool ode_mode = false;

    // SVEIR information
    bool susceptible = false;
    bool vaccinated = false;
    bool exposed = false;
    bool infected = false;
    bool recovered = false;

    // Friends information
    std::vector<Agent*> connections;
    std::vector<Agent*> lunch_friends;

    // School information
    std::string p1, p2, p3, p4, p5;
    bool at_home = false;

    // Functions (public)
    void add_to_connections(Agent* new_agent);
    void generate_lunch_friends(std::map<std::string, std::array<std::vector<Agent*>, 5>>& classrooms);
    void individual_disease_progression();
    void process_washroom_needs(std::vector<double>& school_washrooms);
    void interact_with_friend_random();
    void resolve_classroom(int current_period, std::map<std::string, std::array<std::vector<Agent*>, 5>>& classrooms);
    void get_infected();

  private:
    // Disease Progression
    long long exposed_minute_count = 0;
    long long infected_minute_count = 0;

    // Random number generation
    std::mt19937 mt;
      // Washroom RNG
    std::bernoulli_distribution going_to_washroom;
    std::uniform_int_distribution<int> washroom_choice;
      // Infection RNG
    std::bernoulli_distribution infection_prob;
    std::bernoulli_distribution infection_prob_washroom;
    std::normal_distribution<> stoch_range;
    std::exponential_distribution<> stoch_range_ode;
    
    // Functions (private)
    void interact(Agent& other_agent);

};

#endif

/*
[1] Heffernan, J. M., & Keeling, M. J. (2008). 
    An in-host model of acute infection: Measles as a case study. Theoretical population biology, 73(1), 134-147.
  

*/