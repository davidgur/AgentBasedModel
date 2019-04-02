//
// Created by david on 4/1/2019.
//

#include <iostream>
#include <fstream>

#include "../include/simulation.h"
#include "../include/graph_building.h"
#include "../include/configure_agents.h"

#define PRE_CLASS     500   // From 0 to 500 minutes, students are at home
#define PERIOD1_START 520
#define PERIOD1_END   600
#define PERIOD2_START 605
#define PERIOD2_END   680
#define PERIOD3_START 685
#define PERIOD3_END   760
#define PERIOD4_START 675
#define PERIOD4_END   840
#define PERIOD5_START 845
#define PERIOD5_END   920
#define POST_CLASS    940   // From 940 minutes until (24 * 60), students are at home

Simulation::Simulation() {
    this->day_counter = 0;
    this->day_state = 0;
    this->day_limit = 30;
    this->minute_counter = 0;
    this->current_period = -1;

    this->start_time = std::chrono::high_resolution_clock::now();
    this->last_day = std::chrono::high_resolution_clock::now();
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

        bool is_weekend = this->week[this->day_counter % 7] == "Sat" or week[this->day_counter % 7] == "Sun";

        // DAY STATE 0: AGENTS ARE AT HOME
        if (this->day_state == 0 or is_weekend) {
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
            Simulation::resolve_classroom_for_all();
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

void Simulation::export_agent_data(const std::vector<Agent> &agent_vector, std::string file_name) {
    std::ofstream export_file;
    export_file.open("export/" + file_name);
    for (auto const &student : agent_vector) {
        export_file << "[AGENT " << student.grade << student.id << "]" << std::endl;
        export_file << "\t" << student.p1 << std::endl;
        export_file << "\t" << student.p2 << std::endl;
        export_file << "\t" << student.p3 << std::endl;
        export_file << "\t" << student.p4 << std::endl;
        export_file << "\t" << student.p5 << std::endl;

        export_file << "\t->connections" << std::endl;
        for (auto &connection : student.connections) {
            export_file << "\t" << connection->grade << connection->id << std::endl;
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

    Simulation::log("Exporting all agent data.");
    Simulation::export_agent_data(this->grade9_agents, "grade9.txt");
    Simulation::export_agent_data(this->grade10_agents, "grade10.txt");
    Simulation::export_agent_data(this->grade11_agents, "grade11.txt");
    Simulation::export_agent_data(this->grade12_agents, "grade12.txt");
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

void Simulation::individual_disease_progression(const std::vector<Agent> &agent_vector) {
    for (auto agent : agent_vector) {
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

void Simulation::resolve_classroom_for_all() {
    Simulation::resolve_classroom(this->grade9_agents);
    Simulation::resolve_classroom(this->grade10_agents);
    Simulation::resolve_classroom(this->grade11_agents);
    Simulation::resolve_classroom(this->grade12_agents);
}

void Simulation::process_washroom_needs(const std::vector<Agent> &agent_vector) {
    for (auto agent : agent_vector) {
        agent.process_washroom_needs();
    }
}

void Simulation::interaction_among_friends(const std::vector<Agent> &agent_vector) {
    for (auto agent : agent_vector) {
        agent.interact_with_friend_random();
    }
}

void Simulation::resolve_classroom(const std::vector<Agent> &agent_vector) {
    for (auto agent : agent_vector)
        agent.resolve_classroom();
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
        return 0;
    else if (between(PERIOD2_START, PERIOD2_END))
        return 1;
    else if (between(PERIOD3_START, PERIOD3_END))
        return 2;
    else if (between(PERIOD4_START, PERIOD4_END))
        return 3;
    else if (between(PERIOD5_START, PERIOD5_END))
        return 4;
    else
        return -1;
}

void Simulation::set_day_limit(unsigned int day_limit) {
    this->day_limit = day_limit;
}

void Simulation::determine_classroom_population() {
    for (auto &agent : this->grade9_agents) {
        classrooms[agent.p1][0].push_back(&agent);
        classrooms[agent.p2][1].push_back(&agent);
        classrooms[agent.p3][2].push_back(&agent);
        classrooms[agent.p4][3].push_back(&agent);
        classrooms[agent.p5][4].push_back(&agent);
    }

    for (auto &agent : this->grade10_agents) {
        classrooms[agent.p1][0].push_back(&agent);
        classrooms[agent.p2][1].push_back(&agent);
        classrooms[agent.p3][2].push_back(&agent);
        classrooms[agent.p4][3].push_back(&agent);
        classrooms[agent.p5][4].push_back(&agent);
    }

    for (auto &agent : this->grade11_agents) {
        classrooms[agent.p1][0].push_back(&agent);
        classrooms[agent.p2][1].push_back(&agent);
        classrooms[agent.p3][2].push_back(&agent);
        classrooms[agent.p4][3].push_back(&agent);
        classrooms[agent.p5][4].push_back(&agent);
    }

    for (auto &agent : this->grade12_agents) {
        classrooms[agent.p1][0].push_back(&agent);
        classrooms[agent.p2][1].push_back(&agent);
        classrooms[agent.p3][2].push_back(&agent);
        classrooms[agent.p4][3].push_back(&agent);
        classrooms[agent.p5][4].push_back(&agent);
    }
}