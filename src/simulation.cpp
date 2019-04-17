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

#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <sys/stat.h>

#include "../include/simulation.h"
#include "../include/graph_building.h"
#include "../include/configure_agents.h"
#include "../include/random_element.h"

Simulation::Simulation(int sim_id, double vacc_rate) {
	this->day_counter = 0;
	this->day_state = 0;
	this->day_limit = 30;
	this->minute_counter = 0;
	this->current_period = -1;

	this->sim_id = sim_id;
	this->vacc_rate = vacc_rate;
	this->export_folder = "export/SIM_" + std::to_string(this->vacc_rate) + "/" + std::to_string(this->sim_id) + "/";

	this->start_time = std::chrono::high_resolution_clock::now();
	this->last_day = std::chrono::high_resolution_clock::now();

	this->mt = std::mt19937(rd());
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
	this->log("[" + std::to_string(this->sim_id) + "] Day " + std::to_string(this->day_counter) +
		" (" + this->week[this->day_counter % 7] + ")\t" +
		"[~]");
	this->last_day = std::chrono::high_resolution_clock::now();

	// MAIN SIMULATION LOOP
	while (this->day_counter < this->day_limit) {
		// Figure out what the day_state and what period it is during the day
		// (School days are divided into periods)
		this->day_state = this->determine_day_state();
		this->current_period = this->determine_period();

		bool is_weekend = this->week[this->day_counter % 7] == "Sat" || this->week[this->day_counter % 7] == "Sun";

		// DAY STATE 0: AGENTS ARE AT HOME
		if (this->day_state == 0 || is_weekend) {
			this->individual_disease_progression_for_all();
		}
			// DAY STATE 1: BEFORE/AFTER CLASS
		else if (this->day_state == 1) {
			this->individual_disease_progression_for_all();
			this->interaction_among_friends_for_all();
			this->process_washroom_needs_for_all();
		}
			// DAY STATE 2: IN CLASS
		else if (this->day_state == 2) {
			this->individual_disease_progression_for_all();
			this->resolve_classroom_for_all(this->current_period);
			this->process_washroom_needs_for_all();
		}
			// DAY STATE 3: IN HALL
		else if (this->day_state == 3) {
			this->individual_disease_progression_for_all();
			this->interaction_among_friends_for_all();
			this->process_washroom_needs_for_all();
		}

		// Increment minute counter
		this->minute_counter++;
		this->decay_washroom_concentration();
		this->print_population_sizes();
		if (this->minute_counter == (24 * 60)) {
			this->day_counter++;
			this->clean_washrooms();
			this->minute_counter = 0;
			this->current_period = 0;

			if (day_counter % 7 == 0) {
				if (this->check_for_steady()) {
					this->log("[" + std::to_string(this->sim_id) + "] Simulation reached steady state!");
					break;
				}
			}

			// Log what day it is
			std::string day_counter_str = std::to_string(this->day_counter);
			std::string day_of_week = this->week[this->day_counter % 7];
			std::string ms_since_yesterday = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>
				                                                (std::chrono::high_resolution_clock::now() -
					                                                this->last_day).count());

			this->log(
				"[" + std::to_string(this->sim_id) + "] Day " + day_counter_str + " (" + day_of_week + ")\t" + "[" +
					ms_since_yesterday + "]");
			this->last_day = std::chrono::high_resolution_clock::now();
		}
	}
	this->log("[" + std::to_string(this->sim_id) + "] Simulation complete!");
	this->population_out.close();
}

std::thread Simulation::start_simulation_thread() {
	return std::thread([this] { this->start_simulation(); });
}

void Simulation::log(const std::string to_print) {
	auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - this->start_time);
	std::cout << "[" << current_time.count() << "]\t\t";
	std::cout << to_print << std::endl;
}

void Simulation::export_agent_data(std::vector<Agent> &agent_vector, const std::string file_name) {
	std::ofstream export_file;
	export_file.open(this->export_folder + file_name);
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
	std::string parent_folder = "export/SIM_" + std::to_string(this->vacc_rate);

	mkdir(this->export_folder.c_str(), 0777);
	this->population_out.open(this->export_folder + "population_sizes.csv", std::ios::app);
	this->prep_output_file();

	this->populate_agent_vector();

	watts_strogatz_in_vector(this->grade9_agents);
	watts_strogatz_in_vector(this->grade10_agents);
	watts_strogatz_in_vector(this->grade11_agents);
	watts_strogatz_in_vector(this->grade12_agents);

	random_connections_between_grades(this->grade9_agents, this->grade10_agents);
	random_connections_between_grades(this->grade10_agents, this->grade11_agents);
	random_connections_between_grades(this->grade11_agents, this->grade12_agents);

	assign_student_timetables(this->grade9_agents, 0);
	assign_student_timetables(this->grade10_agents, 1);
	assign_student_timetables(this->grade11_agents, 2);
	assign_student_timetables(this->grade12_agents, 3);

	this->determine_classroom_population();
	this->pick_random_sick();

	this->export_agent_data(this->grade9_agents, "grade9.txt");
	this->export_agent_data(this->grade10_agents, "grade10.txt");
	this->export_agent_data(this->grade11_agents, "grade11.txt");
	this->export_agent_data(this->grade12_agents, "grade12.txt");
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
	this->individual_disease_progression(this->grade9_agents);
	this->individual_disease_progression(this->grade10_agents);
	this->individual_disease_progression(this->grade11_agents);
	this->individual_disease_progression(this->grade12_agents);
}

void Simulation::interaction_among_friends_for_all() {
	this->interaction_among_friends(this->grade9_agents);
	this->interaction_among_friends(this->grade10_agents);
	this->interaction_among_friends(this->grade11_agents);
	this->interaction_among_friends(this->grade12_agents);
}

void Simulation::process_washroom_needs_for_all() {
	this->process_washroom_needs(this->grade9_agents);
	this->process_washroom_needs(this->grade10_agents);
	this->process_washroom_needs(this->grade11_agents);
	this->process_washroom_needs(this->grade12_agents);
}

void Simulation::resolve_classroom_for_all(int current_period) {
	this->resolve_classroom(this->grade9_agents, current_period);
	this->resolve_classroom(this->grade10_agents, current_period);
	this->resolve_classroom(this->grade11_agents, current_period);
	this->resolve_classroom(this->grade12_agents, current_period);
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

void Simulation::prep_output_file() {
	std::ofstream out;
	out.open(this->export_folder + "population_sizes.csv", std::ofstream::out | std::ofstream::trunc);
	out << "G9S,G9V,G9E,G9I,G9R,G10S,G10V,G10E,G10I,G10R,G11S,G11V,G11E,G11I,G11R,G12S,G12V,G12E,G12I,G12R\n";
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
		agent.process_washroom_needs(this->school_washrooms);
	}
}

void Simulation::interaction_among_friends(std::vector<Agent> &agent_vector) {
	std::discrete_distribution<bool> friends_distribution{1 - FRIENDS_PROBABILITY, FRIENDS_PROBABILITY};

	for (auto &agent : agent_vector) {
		bool interact_with_friend = friends_distribution(this->mt);
		if (interact_with_friend && !agent.at_home)
			agent.interact_with_friend_random();
	}
}

void Simulation::resolve_classroom(std::vector<Agent> &agent_vector, int current_period) {
	std::discrete_distribution<bool> class_distribution(1 - CLASS_PROBABILITY, CLASS_PROBABILITY);

	for (auto &agent : agent_vector) {
		bool interact_in_class = class_distribution(this->mt);
		if (interact_in_class && !agent.at_home)
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
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 4);

	Agent *sick_agent = nullptr;

	switch (dist(mt)) {
		case 0: sick_agent = &(*random_element(this->grade9_agents.begin(), this->grade9_agents.end()));
			break;
		case 1: sick_agent = &(*random_element(this->grade10_agents.begin(), this->grade10_agents.end()));
			break;
		case 2: sick_agent = &(*random_element(this->grade11_agents.begin(), this->grade11_agents.end()));
			break;
		case 3: sick_agent = &(*random_element(this->grade12_agents.begin(), this->grade12_agents.end()));
			break;
		default: sick_agent = &(*random_element(this->grade9_agents.begin(), this->grade9_agents.end()));
	}

	sick_agent->infected = true;
	sick_agent->susceptible = false;
	sick_agent->vaccinated = false;

}

void Simulation::create_vaccinated() {
	int grade9_vacc_population = (int) std::round(GRADE_9_POPULATION * this->vacc_rate);
	int grade10_vacc_population = (int) std::round(GRADE_10_POPULATION * this->vacc_rate);
	int grade11_vacc_population = (int) std::round(GRADE_11_POPULATION * this->vacc_rate);
	int grade12_vacc_population = (int) std::round(GRADE_12_POPULATION * this->vacc_rate);

	// grade 9
	for (int i = 0; i < grade9_vacc_population; i++) {
		Agent *student = nullptr;
		while (true) {
			student = &(*random_element(grade9_agents.begin(), grade9_agents.end()));
			if (!student->vaccinated)
				break;
		}
		student->vaccinated = true;
		student->susceptible = false;
	}

	// grade 10
	for (int i = 0; i < grade10_vacc_population; i++) {
		Agent *student = nullptr;
		while (true) {
			student = &(*random_element(grade10_agents.begin(), grade10_agents.end()));
			if (!student->vaccinated)
				break;
		}
		student->vaccinated = true;
		student->susceptible = false;
	}

	// grade 11
	for (int i = 0; i < grade11_vacc_population; i++) {
		Agent *student = nullptr;
		while (true) {
			student = &(*random_element(grade11_agents.begin(), grade11_agents.end()));
			if (!student->vaccinated)
				break;
		}
		student->vaccinated = true;
		student->susceptible = false;
	}

	// grade 12
	for (int i = 0; i < grade12_vacc_population; i++) {
		Agent *student = nullptr;
		while (true) {
			student = &(*random_element(grade12_agents.begin(), grade12_agents.end()));
			if (!student->vaccinated)
				break;
		}
		student->vaccinated = true;
		student->susceptible = false;
	}

}

void Simulation::decay_washroom_concentration() {
	for (auto &washroom : this->school_washrooms) {
		washroom *= CONCENTRATION_DECAY_RATE;
	}
}

void Simulation::clean_washrooms() {
	for (auto &washroom : this->school_washrooms) {
		washroom = 0;
	}
}

bool Simulation::check_for_steady() {
	auto not_sick = [](Agent &agent) { return agent.exposed || agent.infected; };

	bool grade9_steady = std::count_if(this->grade9_agents.begin(), this->grade9_agents.end(), not_sick) == 0;
	bool grade10_steady = std::count_if(this->grade10_agents.begin(), this->grade10_agents.end(), not_sick) == 0;
	bool grade11_steady = std::count_if(this->grade11_agents.begin(), this->grade11_agents.end(), not_sick) == 0;
	bool grade12_steady = std::count_if(this->grade12_agents.begin(), this->grade12_agents.end(), not_sick) == 0;

	return grade9_steady && grade10_steady && grade11_steady && grade12_steady;
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