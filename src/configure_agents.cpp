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

#include <random>

void assign_student_timetables(std::vector<Agent> &agent_vector, int grade) {
	std::vector<std::vector<std::vector<std::string>>> student_courses = {
		{   // Grade 9
			{"304", "115", "223", "345", "227", "226", "211", "108", "340", "312", "GYMA", "319", "145", "334"}, // p1
			{"304", "210", "GYMA", "227", "150", "320", "226", "340", "GYMB", "319", "205", "115"}, // p2
			{"111", "150", "112", "210", "226", "146", "346", "320"}, // p4
			{"GYMA", "345", "304", "211", "LIBSEM", "GYMB", "320", "214", "227", "205"} // p5
		},
		{   // Grade 10
			{"337", "316", "146", "327", "314", "111", "208", "338", "326", "303", "107", "213"}, // p1
			{"329", "337", "312", "106", "333", "112", "305", "345", "316", "303", "327", "211", "145", "310", "213",
			 "336"}, // p2
			{"337", "GYMB", "WEIGH", "227", "223", "213", "303", "312", "302"}, // p4
			{"107", "150", "112", "106", "333", "317", "WEIGH", "303", "226"} // p5
		},
		{   // Grade 11
			{"328", "310", "150", "112", "305", "214", "302", "106", "GYMB", "317", "336"}, // p1
			{"111", "208", "317", "214", "346", "107", "334", "302", "336", "334", "302"}, // p2
			{"345", "338", "333", "211", "214", "145", "220", "317"}, // p4
			{"328", "310", "314", "326", "111", "305", "223", "146", "337",} // p5
		},
		{   // Grade 12
			{"329", "301", "210", "333", "WEIGH", "346", "320", "220"}, // p1
			{"326", "338", "328", "223", "WEIGH", "301", "146"}, // p2
			{"310", "314", "106", "301", "316", "327", "340", "328", "336", "WEIGH"}, // p4
			{"338", "208", "316", "327", "346", "301", "150", "312"} // p5
		}
	};

    for (auto &student : agent_vector) {
        // Assign the student to some classes
	    student.p1 = *random_element(student_courses[grade][0].begin(), student_courses[grade][0].end());
	    student.p2 = *random_element(student_courses[grade][1].begin(), student_courses[grade][1].end());
	    // student.p3 = *random_element(student_courses[grade][2].begin(), student_courses[grade][2].end());
	    student.p4 = *random_element(student_courses[grade][2].begin(), student_courses[grade][2].end());
	    student.p5 = *random_element(student_courses[grade][3].begin(), student_courses[grade][3].end());

        student.p3 = kLunchVar;

        student.lunch_period = 2;
    }

}

void assign_lunch_friends(std::vector<Agent>& agent_vector, std::map<std::string, std::array<std::vector<Agent*>, 5>>& classrooms) {
	for (auto& agent : agent_vector)
		agent.generate_lunch_friends(classrooms);
}