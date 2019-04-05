/**
 * @file simulation.cpp
 *
 * @brief File containing all functions associated with Simulation class
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 *
 * TODO: - Track period of the day
 *       - Create transmission dynamic in class
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
            Simulation::interaction_among_friends_for_all();
            //Simulation::resolve_classroom_for_all();
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

    Simulation::log("Picking random sick person");
    Simulation::pick_random_sick();

    Simulation::log("Exporting all agent data.");
    Simulation::export_agent_data(this->grade9_agents, "grade9.txt");
    Simulation::export_agent_data(this->grade10_agents, "grade10.txt");
    Simulation::export_agent_data(this->grade11_agents, "grade11.txt");
    Simulation::export_agent_data(this->grade12_agents, "grade12.txt");

    Simulation::log("Preparing export file.");
    Simulation::clear_existing_data();
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

void Simulation::resolve_classroom_for_all() {
    Simulation::resolve_classroom(this->grade9_agents);
    Simulation::resolve_classroom(this->grade10_agents);
    Simulation::resolve_classroom(this->grade11_agents);
    Simulation::resolve_classroom(this->grade12_agents);
}

std::vector<int> Simulation::get_grade9_population_sizes() {
    int grade9_susceptible = std::count_if(this->grade9_agents.begin(), this->grade9_agents.end(),
                                           [](auto &agent) { return agent.susceptible == true; });
    int grade9_vaccinated = std::count_if(this->grade9_agents.begin(), this->grade9_agents.end(),
                                          [](auto &agent) { return agent.vaccinated == true; });
    int grade9_exposed = std::count_if(this->grade9_agents.begin(), this->grade9_agents.end(),
                                       [](auto &agent) { return agent.exposed == true; });
    int grade9_infected = std::count_if(this->grade9_agents.begin(), this->grade9_agents.end(),
                                        [](auto &agent) { return agent.infected == true; });
    int grade9_recovered = std::count_if(this->grade9_agents.begin(), this->grade9_agents.end(),
                                         [](auto &agent) { return agent.recovered == true; });

    return std::vector<int>{grade9_susceptible, grade9_vaccinated, grade9_exposed, grade9_infected, grade9_recovered};
}

std::vector<int> Simulation::get_grade10_population_sizes() {
    int grade10_susceptible = std::count_if(this->grade10_agents.begin(), this->grade10_agents.end(),
                                            [](auto &agent) { return agent.susceptible == true; });
    int grade10_vaccinated = std::count_if(this->grade10_agents.begin(), this->grade10_agents.end(),
                                           [](auto &agent) { return agent.vaccinated == true; });
    int grade10_exposed = std::count_if(this->grade10_agents.begin(), this->grade10_agents.end(),
                                        [](auto &agent) { return agent.exposed == true; });
    int grade10_infected = std::count_if(this->grade10_agents.begin(), this->grade10_agents.end(),
                                         [](auto &agent) { return agent.infected == true; });
    int grade10_recovered = std::count_if(this->grade10_agents.begin(), this->grade10_agents.end(),
                                          [](auto &agent) { return agent.recovered == true; });

    return std::vector<int>{grade10_susceptible, grade10_vaccinated, grade10_exposed, grade10_infected,
                            grade10_recovered};
}

std::vector<int> Simulation::get_grade11_population_sizes() {
    int grade11_susceptible = std::count_if(this->grade11_agents.begin(), this->grade11_agents.end(),
                                            [](auto &agent) { return agent.susceptible == true; });
    int grade11_vaccinated = std::count_if(this->grade11_agents.begin(), this->grade11_agents.end(),
                                           [](auto &agent) { return agent.vaccinated == true; });
    int grade11_exposed = std::count_if(this->grade11_agents.begin(), this->grade11_agents.end(),
                                        [](auto &agent) { return agent.exposed == true; });
    int grade11_infected = std::count_if(this->grade11_agents.begin(), this->grade11_agents.end(),
                                         [](auto &agent) { return agent.infected == true; });
    int grade11_recovered = std::count_if(this->grade11_agents.begin(), this->grade11_agents.end(),
                                          [](auto &agent) { return agent.recovered == true; });

    return std::vector<int>{grade11_susceptible, grade11_vaccinated, grade11_exposed, grade11_infected,
                            grade11_recovered};
}

std::vector<int> Simulation::get_grade12_population_sizes() {
    int grade12_susceptible = std::count_if(this->grade12_agents.begin(), this->grade12_agents.end(),
                                            [](auto &agent) { return agent.susceptible == true; });
    int grade12_vaccinated = std::count_if(this->grade12_agents.begin(), this->grade12_agents.end(),
                                           [](auto &agent) { return agent.vaccinated == true; });
    int grade12_exposed = std::count_if(this->grade12_agents.begin(), this->grade12_agents.end(),
                                        [](auto &agent) { return agent.exposed == true; });
    int grade12_infected = std::count_if(this->grade12_agents.begin(), this->grade12_agents.end(),
                                         [](auto &agent) { return agent.infected == true; });
    int grade12_recovered = std::count_if(this->grade12_agents.begin(), this->grade12_agents.end(),
                                          [](auto &agent) { return agent.recovered == true; });

    return std::vector<int>{grade12_susceptible, grade12_vaccinated, grade12_exposed, grade12_infected,
                            grade12_recovered};
}

void Simulation::clear_existing_data() {
    std::ofstream out;
    out.open("export/population_sizes.csv", std::ofstream::out | std::ofstream::trunc);
    out.close();
}

void Simulation::print_population_sizes() {
    std::vector<int> grade9_population_sizes = this->get_grade9_population_sizes();
    std::vector<int> grade10_population_sizes = this->get_grade10_population_sizes();
    std::vector<int> grade11_population_sizes = this->get_grade11_population_sizes();
    std::vector<int> grade12_population_sizes = this->get_grade12_population_sizes();

    std::ofstream out;

    // Append to file
    out.open("export/population_sizes.csv", std::ios::app);

    // Grade 9
    out << grade9_population_sizes[0] << "," <<
        grade9_population_sizes[1] << "," <<
        grade9_population_sizes[2] << "," <<
        grade9_population_sizes[3] << "," <<
        grade9_population_sizes[4] << ",";

    // Grade 10
    out << grade10_population_sizes[0] << "," <<
        grade10_population_sizes[1] << "," <<
        grade10_population_sizes[2] << "," <<
        grade10_population_sizes[3] << "," <<
        grade10_population_sizes[4] << ",";

    // Grade 11
    out << grade11_population_sizes[0] << "," <<
        grade11_population_sizes[1] << "," <<
        grade11_population_sizes[2] << "," <<
        grade11_population_sizes[3] << "," <<
        grade11_population_sizes[4] << ",";

    // Grade 12
    out << grade12_population_sizes[0] << "," <<
        grade12_population_sizes[1] << "," <<
        grade12_population_sizes[2] << "," <<
        grade12_population_sizes[3] << "," <<
        grade12_population_sizes[4] << std::endl;
}

void Simulation::process_washroom_needs(std::vector<Agent> &agent_vector) {
    for (auto &agent : agent_vector) {
        agent.process_washroom_needs();
    }
}

void Simulation::interaction_among_friends(std::vector<Agent> &agent_vector) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (auto &agent : agent_vector) {
        bool is_interacting_with_friend = dist(mt) < (20.0 / 75.0);
        if (is_interacting_with_friend)
            agent.interact_with_friend_random();
    }
}

void Simulation::resolve_classroom(std::vector<Agent> &agent_vector) {
    for (auto &agent : agent_vector)
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

void Simulation::pick_random_sick() {
    Agent *sick_agent = &(*random_element(this->grade9_agents.begin(), this->grade9_agents.end()));
    sick_agent->exposed = true;
    sick_agent->susceptible = false;
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