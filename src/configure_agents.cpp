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

#include "../include/configure_agents.hh"
#include "../include/classroom.hh"

#include <random>

void assign_student_timetables(std::vector<Agent> &agent_vector, int grade) {
	// How many classrooms do we need?
	int num_of_classes = agent_vector.size() / kAverageClassroomSize;

	// Make some classes
	std::vector<std::vector<Classroom>> student_courses(5);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < num_of_classes; j++) {
			student_courses[i].push_back(Classroom(j, grade, i));
		}
	}
    for (auto &student : agent_vector) {
        // Assign the student to some classes
	    student.p1 = random_element(student_courses[0].begin(), student_courses[0].end())->get_name();
	    student.p2 = random_element(student_courses[1].begin(), student_courses[1].end())->get_name();
	    student.p3 = random_element(student_courses[2].begin(), student_courses[2].end())->get_name();
	    student.p4 = random_element(student_courses[3].begin(), student_courses[3].end())->get_name();
	    student.p5 = random_element(student_courses[4].begin(), student_courses[4].end())->get_name();

        // Give the student a lunch
        //      If the student is in grade 9 or 11, give them a lunch in P3
		//		If the student is in grade 10 or 12, give them a lunch in P4
        int lunch_period;

	    if (grade == 0 or grade == 2)
            lunch_period = 3;
        else
            lunch_period = 4;

        if (lunch_period == 1)
            student.p1 = kLunchVar;
        else if (lunch_period == 2)
            student.p2 = kLunchVar;
        else if (lunch_period == 3)
            student.p3 = kLunchVar;
        else if (lunch_period == 4)
            student.p4 = kLunchVar;
        else if (lunch_period == 5)
            student.p5 = kLunchVar;

        student.lunch_period = lunch_period - 1;
    }

}

void assign_lunch_friends(std::vector<Agent>& agent_vector, std::map<std::string, std::array<std::vector<Agent*>, 5>>& classrooms) {
	for (auto& agent : agent_vector)
		agent.generate_lunch_friends(classrooms);
}