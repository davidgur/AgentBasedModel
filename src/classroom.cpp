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

#include "../include/classroom.hh"

Classroom::Classroom(int id, int grade, int period) {
    this->id = id;
    this->grade = grade;
    this->period = period;
}

int Classroom::population() {
    return this->classroom_population.size();
}

void Classroom::add_to_class(Agent* new_agent) {
    if (new_agent->grade == this->grade)
        this->classroom_population.push_back(new_agent);
}

std::string Classroom::get_name() {
    return "C" + std::to_string(this->grade) + "_" + std::to_string(this->period) + "_" + std::to_string(this->id);
}