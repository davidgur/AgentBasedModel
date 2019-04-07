/**
 * @file simulation.cpp
 *
 * @brief File containing all functions associated with Simulation class
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 *
 * TODO: - Do environmental transmission (maybe 1/100 of regular transmission?)
 *
 */
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>

#include "../include/simulation.h"
#include "../include/graph_building.h"
#include "../include/configure_agents.h"
#include "../include/random_element.h"

Simulation::Simulation() {
    this->day_counter = 0;
    this->day_state = 0;
    this->day_limit = 30;
    this->minute_counter = 0;
    this->current_period = -1;

    this->start_time = std::chrono::high_resolution_clock::now();
    this->last_day = std::chrono::high_resolution_clock::now();

    this->mt = std::mt19937(rd());
    this->dist = std::uniform_real_distribution<double>(0.0, 1.0);
}

Simulation::~Simulation() = default;

void Simulation::start_simulation() {
    /*  (void) --> (void)
     *  This is the function that runs the simulation based on the existing
     *  parameters in the 'Simulation' object.
     *
     *  A loop runs until the specified day limit is reached.
     *  NOTE: Simulation starts on a Monday.
     *        This model assumes that students don't go to school on Sat and Sun
     */
    Simulation::log("Beginning Simulation Loop now!");
    Simulation::log("Day " + std::to_string(this->day_counter) +
                    " (" + this->week[this->day_counter % 7] + ")\t" +
                    "[Time elapsed since last day (ms): ~]");
    this->last_day = std::chrono::high_resolution_clock::now();

    // MAIN SIMULATION LOOP
    while (this->day_counter < this->day_limit) {
        // Figure out what the day_state and what period it is during the day
        // (School days are divided into periods)
        this->day_state = Simulation::determine_day_state();
        this->current_period = Simulation::determine_period();

        bool is_weekend = this->week[this->day_counter % 7] == "Sat" || this->week[this->day_counter % 7] == "Sun";

        // DAY STATE 0: AGENTS ARE AT HOME
        if (this->day_state == 0 || is_weekend) {
            // only do Individual Disease Progression
            Simulation::individual_disease_progression_for_all();
        }
            // DAY STATE 1: BEFORE/AFTER CLASS
        else if (this->day_state == 1) {
            // Individual Disease Progression, Social Among Friends, Washroom
            Simulation::individual_disease_progression_for_all();
            Simulation::interaction_among_friends_for_all();
	    Simulation::process_washroom_needs_for_all();
        }
            // DAY STATE 2: IN CLASS
        else if (this->day_state == 2) {
            // Individual Disease Progression, Social Among Classmates, Washroom, any special class considerations
            Simulation::individual_disease_progression_for_all();
            Simulation::resolve_classroom_for_all(this->current_period);
            Simulation::process_washroom_needs_for_all();
        }
            // DAY STATE 3: IN HALL
        else if (this->day_state == 3) {
            Simulation::individual_disease_progression_for_all();
            Simulation::interaction_among_friends_for_all();
            Simulation::process_washroom_needs_for_all();
        }

        // Increment minute counter
        this->minute_counter++;
        this->print_population_sizes();
        if (this->minute_counter == (24 * 60)) {
            this->day_counter++;
            this->minute_counter = 0;
            this->current_period = 0;
            Simulation::log("Day " + std::to_string(this->day_counter) +
                            " (" + this->week[this->day_counter % 7] + ")\t" +
                            "[Time elapsed since last day (ms): " +
                            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>
                                                   (std::chrono::high_resolution_clock::now() -
                                                    this->last_day).count()) +
                            "]");
            this->last_day = std::chrono::high_resolution_clock::now();
        }
    }
    Simulation::log("Simulation complete!");
}

void Simulation::log(std::string to_print) {
    auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::high_resolution_clock::now() - this->start_time);
    std::cout << "[" << current_time.count() << "]\t\t";
    std::cout << to_print << std::endl;
}

void Simulation::export_agent_data(std::vector<Agent> &agent_vector, std::string file_name) {
    std::ofstream export_file;
    export_file.open("export/" + file_name);
    for (auto &student : agent_vector) {
        export_file << "[AGENT " << student.grade << student.id << "]" << "\n";
        export_file << "\t" << student.p1 << "\n";
        export_file << "\t" << student.p2 << "\n";
        export_file << "\t" << student.p3 << "\n";
        export_file << "\t" << student.p4 << "\n";
        export_file << "\t" << student.p5 << "\n";

        export_file << "\t->connections" << "\n";
        for (auto &connection : student.connections) {
            export_file << "\t" << connection->grade << connection->id << "\n";
        }
    }
}

void Simulation::initialize_simulation() {
    Simulation::log("Initializing the simulation now!");

    Simulation::log("Populating all agent vectors.");
    Simulation::populate_agent_vector();

    Simulation::log("Creating scale-free network for each grade.");
    watts_strogatz_in_vector(this->grade9_agents);
    watts_strogatz_in_vector(this->grade10_agents);
    watts_strogatz_in_vector(this->grade11_agents);
    watts_strogatz_in_vector(this->grade12_agents);

    Simulation::log("Creating student links between different grades.");
    random_connections_between_grades(this->grade9_agents, this->grade10_agents);
    random_connections_between_grades(this->grade10_agents, this->grade11_agents);
    random_connections_between_grades(this->grade11_agents, this->grade12_agents);

    Simulation::log("Assigning all students timetables");
    assign_student_timetables(this->grade9_agents, "grade9");
    assign_student_timetables(this->grade10_agents, "grade10");
    assign_student_timetables(this->grade11_agents, "grade11");
    assign_student_timetables(this->grade12_agents, "grade12");

    Simulation::log("Determining classroom populations");
    Simulation::determine_classroom_population();

    Simulation::log("Picking random sick person");
    Simulation::pick_random_sick();

    Simulation::log("Exporting all agent data.");
    Simulation::export_agent_data(this->grade9_agents, "grade9.txt");
    Simulation::export_agent_data(this->grade10_agents, "grade10.txt");
    Simulation::export_agent_data(this->grade11_agents, "grade11.txt");
    Simulation::export_agent_data(this->grade12_agents, "grade12.txt");

    Simulation::log("Preparing export file.");
    Simulation::clear_existing_data();

    this->population_out.open("export/population_sizes.csv", std::ios::app);
}

void Simulation::populate_agent_vector() {
    for (int i = 0; i < GRADE_9_POPULATION; i++) {
        Agent agent(i, 0);
        this->grade9_agents.push_back(agent);
    }

    for (int i = 0; i < GRADE_10_POPULATION; i++) {
        Agent agent(i, 1);
        this->grade10_agents.push_back(agent);
    }

    for (int i = 0; i < GRADE_11_POPULATION; i++) {
        Agent agent(i, 2);
        this->grade11_agents.push_back(agent);
    }

    for (int i = 0; i < GRADE_12_POPULATION; i++) {
        Agent agent(i, 3);
        this->grade12_agents.push_back(agent);
    }
}

void Simulation::individual_disease_progression(std::vector<Agent> &agent_vector) {
    for (auto &agent : agent_vector) {
        agent.individual_disease_progression();
    }
}

void Simulation::individual_disease_progression_for_all() {
    Simulation::individual_disease_progression(this->grade9_agents);
    Simulation::individual_disease_progression(this->grade10_agents);
    Simulation::individual_disease_progression(this->grade11_agents);
    Simulation::individual_disease_progression(this->grade12_agents);
}

void Simulation::interaction_among_friends_for_all() {
    Simulation::interaction_among_friends(this->grade9_agents);
    Simulation::interaction_among_friends(this->grade10_agents);
    Simulation::interaction_among_friends(this->grade11_agents);
    Simulation::interaction_among_friends(this->grade12_agents);
}

void Simulation::process_washroom_needs_for_all() {
    Simulation::process_washroom_needs(this->grade9_agents);
    Simulation::process_washroom_needs(this->grade10_agents);
    Simulation::process_washroom_needs(this->grade11_agents);
    Simulation::process_washroom_needs(this->grade12_agents);
}

void Simulation::resolve_classroom_for_all(int current_period) {
    Simulation::resolve_classroom(this->grade9_agents, current_period);
    Simulation::resolve_classroom(this->grade10_agents, current_period);
    Simulation::resolve_classroom(this->grade11_agents, current_period);
    Simulation::resolve_classroom(this->grade12_agents, current_period);
}

std::array<int, 5> Simulation::get_population_sizes(std::vector<Agent> &agent_vector) {
    int susceptible = 0, vaccinated = 0, exposed = 0, infected = 0, recovered = 0;
    for (auto &agent : agent_vector) {
        if (agent.susceptible) susceptible++;
        else if (agent.vaccinated) vaccinated++;
        else if (agent.exposed) exposed++;
        else if (agent.infected) infected++;
        else if (agent.recovered) recovered++;
    }
    return std::array<int, 5>{susceptible, vaccinated, exposed, infected, recovered};
}

void Simulation::clear_existing_data() {
    std::ofstream out;
    out.open("export/population_sizes.csv", std::ofstream::out | std::ofstream::trunc);
    out.close();
}

void Simulation::print_population_sizes() {
    std::array<int, 5> grade9_population_sizes = get_population_sizes(this->grade9_agents);
    std::array<int, 5> grade10_population_sizes = get_population_sizes(this->grade10_agents);
    std::array<int, 5> grade11_population_sizes = get_population_sizes(this->grade11_agents);
    std::array<int, 5> grade12_population_sizes = get_population_sizes(this->grade12_agents);

    // Grade 9
    this->population_out << grade9_population_sizes[0] << "," <<
                         grade9_population_sizes[1] << "," <<
                         grade9_population_sizes[2] << "," <<
                         grade9_population_sizes[3] << "," <<
                         grade9_population_sizes[4] << ",";

    // Grade 10
    this->population_out << grade10_population_sizes[0] << "," <<
                         grade10_population_sizes[1] << "," <<
                         grade10_population_sizes[2] << "," <<
                         grade10_population_sizes[3] << "," <<
                         grade10_population_sizes[4] << ",";

    // Grade 11
    this->population_out << grade11_population_sizes[0] << "," <<
                         grade11_population_sizes[1] << "," <<
                         grade11_population_sizes[2] << "," <<
                         grade11_population_sizes[3] << "," <<
                         grade11_population_sizes[4] << ",";

    // Grade 12
    this->population_out << grade12_population_sizes[0] << "," <<
                         grade12_population_sizes[1] << "," <<
                         grade12_population_sizes[2] << "," <<
                         grade12_population_sizes[3] << "," <<
                         grade12_population_sizes[4] << "\n";
}

void Simulation::process_washroom_needs(std::vector<Agent> &agent_vector) {
    for (auto &agent : agent_vector) {
        agent.process_washroom_needs();
    }
}

void Simulation::interaction_among_friends(std::vector<Agent> &agent_vector) {
    for (auto &agent : agent_vector) {
        if (this->dist(mt) < FRIENDS_PROBABILITY && !agent.at_home)
            agent.interact_with_friend_random();
    }
}

void Simulation::resolve_classroom(std::vector<Agent> &agent_vector, int current_period) {
    for (auto &agent : agent_vector) {
        if (this->dist(mt) < CLASS_PROBABILITY && !agent.at_home)
            agent.resolve_classroom(current_period, this->classrooms);
    }
}

unsigned short Simulation::determine_day_state() {
    auto between = [this](int lower, int upper) { return (this->minute_counter - lower) < (upper - lower); };

    if (between(0, PRE_CLASS))
        return 0;
    else if (between(PRE_CLASS, PERIOD1_START))
        return 1;
    else if (between(PERIOD1_START, PERIOD1_END))
        return 2;
    else if (between(PERIOD1_END, PERIOD2_START))
        return 3;
    else if (between(PERIOD2_START, PERIOD2_END))
        return 1;
    else if (between(PERIOD2_END, PERIOD3_START))
        return 3;
    else if (between(PERIOD3_START, PERIOD3_END))
        return 2;
    else if (between(PERIOD3_END, PERIOD4_START))
        return 3;
    else if (between(PERIOD4_START, PERIOD4_END))
        return 2;
    else if (between(PERIOD4_END, PERIOD5_START))
        return 3;
    else if (between(PERIOD5_START, PERIOD5_END))
        return 2;
    else if (between(PERIOD5_END, POST_CLASS))
        return 1;
    else if (between(POST_CLASS, 24 * 60))
        return 0;
    else
        return 0;
}

short Simulation::determine_period() {
    auto between = [this](int lower, int upper) { return (this->minute_counter - lower) < (upper - lower); };

    if (between(PERIOD1_START, PERIOD1_END))
        return 1;
    else if (between(PERIOD2_START, PERIOD2_END))
        return 2;
    else if (between(PERIOD3_START, PERIOD3_END))
        return 3;
    else if (between(PERIOD4_START, PERIOD4_END))
        return 4;
    else if (between(PERIOD5_START, PERIOD5_END))
        return 5;
    else
        return 0;
}

void Simulation::set_day_limit(unsigned int day_limit) {
    this->day_limit = day_limit;
}

void Simulation::pick_random_sick() {
    for (int i = 0; i < 3; i++) {
        Agent *sick_agent = &(*random_element(this->grade9_agents.begin(), this->grade9_agents.end()));
        sick_agent->infected = true;
        sick_agent->susceptible = false;
    }
}

void Simulation::determine_classroom_population() {
    for (auto &agent : this->grade9_agents) {
        this->classrooms[agent.p1][0].push_back(&agent);
        this->classrooms[agent.p2][1].push_back(&agent);
        this->classrooms[agent.p3][2].push_back(&agent);
        this->classrooms[agent.p4][3].push_back(&agent);
        this->classrooms[agent.p5][4].push_back(&agent);
    }

    for (auto &agent : this->grade10_agents) {
        this->classrooms[agent.p1][0].push_back(&agent);
        this->classrooms[agent.p2][1].push_back(&agent);
        this->classrooms[agent.p3][2].push_back(&agent);
        this->classrooms[agent.p4][3].push_back(&agent);
        this->classrooms[agent.p5][4].push_back(&agent);
    }

    for (auto &agent : this->grade11_agents) {
        this->classrooms[agent.p1][0].push_back(&agent);
        this->classrooms[agent.p2][1].push_back(&agent);
        this->classrooms[agent.p3][2].push_back(&agent);
        this->classrooms[agent.p4][3].push_back(&agent);
        this->classrooms[agent.p5][4].push_back(&agent);
    }

    for (auto &agent : this->grade12_agents) {
        this->classrooms[agent.p1][0].push_back(&agent);
        this->classrooms[agent.p2][1].push_back(&agent);
        this->classrooms[agent.p3][2].push_back(&agent);
        this->classrooms[agent.p4][3].push_back(&agent);
        this->classrooms[agent.p5][4].push_back(&agent);
    }
}
