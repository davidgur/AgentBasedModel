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

