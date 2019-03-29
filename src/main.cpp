/*
 * main.cpp
 *
 * Main file for executing simulation.
 *
 * ----------
 * Agent Based Model (ABM) for analyzing the spread of measles within secondary schools
 *
 * Author: David Gurevich
 * email: david@gurevich.ca
 *
 * ----------
 * TODO: Write python application to generate a timetable. If possible, port it to C++ later on
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "../include/agent.h"


// Below data is from the YRDSB Monthly Enrolment Report for Secondary Schools (Westmount Collegiate Institute)
// It can be found at: http://www.yrdsb.ca/AboutUs/DirectorsAnnualReport/Documents/2018-19-Sec.pdf
const int Grade9_Population = 292;
const int Grade10_Population = 356;
const int Grade11_Population = 372;
const int Grade12_Population = 334;

const int num_of_fast_trackers = 12;

std::vector<Agent> grade9_agents;
std::vector<Agent> grade10_agents;
std::vector<Agent> grade11_agents;
std::vector<Agent> grade12_agents;

auto start_time = std::chrono::high_resolution_clock::now();

void log(std::string to_print) {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count()
              << "]\t\t";
    std::cout << to_print << std::endl;
}

void export_data(std::vector<Agent> agent_vector, const std::string file_name) {
    std::ofstream export_file;
    export_file.open(file_name);
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

int main() {

    // Create a bunch of agents in each grade
    log("Populating agent vectors.");
    populate_agent_vectors();

    // Create a network among the people in each grade (scale free network)
    log("Creating scale free network for each grade");
    watts_strogatz_in_vector(grade9_agents);
    watts_strogatz_in_vector(grade10_agents);
    watts_strogatz_in_vector(grade11_agents);
    watts_strogatz_in_vector(grade12_agents);

    // Create some (very few) connections for people in different grades
    log("Creating student links between different grades");
    watts_strongatz_between_vectors(grade9_agents, grade10_agents);
    watts_strongatz_between_vectors(grade10_agents, grade11_agents);
    watts_strongatz_between_vectors(grade11_agents, grade12_agents);

    // Assigning all students a timetable in their grade
    log("Assigning students a timetable");
    for (auto &student : grade9_agents) {
        place_student_in_class(&student.p1, 0, 1);
        place_student_in_class(&student.p2, 0, 2);
        place_student_in_class(&student.p3, 0, 3);
        place_student_in_class(&student.p4, 0, 4);
        place_student_in_class(&student.p5, 0, 5);
    }

    for (auto &student : grade10_agents) {
        place_student_in_class(&student.p1, 1, 1);
        place_student_in_class(&student.p2, 1, 2);
        place_student_in_class(&student.p3, 1, 3);
        place_student_in_class(&student.p4, 1, 4);
        place_student_in_class(&student.p5, 1, 5);
    }

    for (auto &student : grade11_agents) {
        place_student_in_class(&student.p1, 2, 1);
        place_student_in_class(&student.p2, 2, 2);
        place_student_in_class(&student.p3, 2, 3);
        place_student_in_class(&student.p4, 2, 4);
        place_student_in_class(&student.p5, 2, 5);
    }

    for (auto &student : grade12_agents) {
        place_student_in_class(&student.p1, 3, 1);
        place_student_in_class(&student.p2, 3, 2);
        place_student_in_class(&student.p3, 3, 3);
        place_student_in_class(&student.p4, 3, 4);
        place_student_in_class(&student.p5, 3, 5);
    }

    log("Exporting current data to file.");
    export_data(grade9_agents, "grade9.txt");
    export_data(grade10_agents, "grade10.txt");
    export_data(grade11_agents, "grade11.txt");
    export_data(grade12_agents, "grade12.txt");
    log("Complete!");
    return 0;
}