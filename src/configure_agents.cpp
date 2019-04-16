/**
 * @file configure_agents.cpp
 *
 * @brief Functions that are used to configure individual agents before the simulation starts
 *
 * @author David Gurevich
 * Contact: david(at)gurevich.ca
 */

#include "../include/configure_agents.h"
#include "../include/agent.h"
#include "../include/random_element.h"
#include "../include/json.hpp"


#include <random>
#include <fstream>

void assign_student_timetables(std::vector<Agent> &agent_vector, std::vector<nlohmann::json> &classes,
                               std::string grade_id) {
    for (auto &student : agent_vector) {
        // Assign the student to some classes
        student.p1 = *random_element(classes[0][grade_id].begin(), classes[0][grade_id].end());
        student.p2 = *random_element(classes[1][grade_id].begin(), classes[1][grade_id].end());
        student.p3 = *random_element(classes[2][grade_id].begin(), classes[2][grade_id].end());
        student.p4 = *random_element(classes[3][grade_id].begin(), classes[3][grade_id].end());
        student.p5 = *random_element(classes[4][grade_id].begin(), classes[4][grade_id].end());

        // Give the student a lunch
        //      If the student is in grade 9 or 10, give them a lunch in P3 or P4
        int lunch_period;
        std::vector<int> possible_lunch_periods;

        if (grade_id == "grade9" or grade_id == "grade10")
            possible_lunch_periods = {3, 4};
        else
            possible_lunch_periods = {1, 2, 3, 4, 5};

        // SUPER overkill way of generating a random number
        lunch_period = *random_element(possible_lunch_periods.begin(), possible_lunch_periods.end());

        if (lunch_period == 1)
            student.p1 = "LUNCH";
        else if (lunch_period == 2)
            student.p2 = "LUNCH";
        else if (lunch_period == 3)
            student.p3 = "LUNCH";
        else if (lunch_period == 4)
            student.p4 = "LUNCH";
        else if (lunch_period == 5)
            student.p5 = "LUNCH";
    }

}

