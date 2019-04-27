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

#include "../include/simulation.hh"

// Constructor
Simulation::Simulation(int sim_id, double vacc_rate) {
    this->sim_id = sim_id;
    this->vacc_rate = vacc_rate;
    this->export_folder = "export/SIM_" + std::to_string(this->vacc_rate) + "/" + std::to_string(this->sim_id) + "/";

    this->day_counter = 0;
    this->day_state = 0;
    this->minute_counter = 0;
    this->current_period = -1;

    this->last_day = std::chrono::high_resolution_clock::now();

    this->mt = std::mt19937(std::random_device{}());
    this->friends_distribution = std::bernoulli_distribution(kFriendsInteractionProbability);
    this->classroom_distribution = std::bernoulli_distribution(kClassInteractionProbability);
    this->random_sick_grade = std::uniform_int_distribution<int>(0, 4);

    this->grade9_agents.reserve(kGrade9Population);
    this->grade10_agents.reserve(kGrade10Population);
    this->grade11_agents.reserve(kGrade11Population);
    this->grade12_agents.reserve(kGrade12Population);
}

// Create a thread to run the simulation
std::thread Simulation::start_simulation_thread() {
    this->initialize_simulation();
    return std::thread([this]{this->run_simulation();});
}

// Loop that runs the simulation
void Simulation::run_simulation() {
    
    // Get ready for the first day
    this->log();
    this->last_day = std::chrono::high_resolution_clock::now();

    // Main simulation loop
    while (true) {
        // Figure out what the day state is and what period it is during the day
        this->day_state = this->determine_day_state();
        this->current_period = this->determine_period();

        // Is it the weekend?
        bool is_weekend = day_counter % 7 == 5 or day_counter % 7 == 6;

        // Day State 0: Agents are at home
        if (this->day_state == 0 or is_weekend)
            this->individual_disease_progression_for_all();
        
        // Day State 1: Before / After class
        else if (this->day_state == 1) {
            this->individual_disease_progression_for_all();
            this->interaction_among_friends_for_all();
            this->process_washroom_needs_for_all(); 		// TODO: FIX PROBLEM HERE
        }

        // Day State 2: In Class
        else if (this->day_state == 2) {
            this->individual_disease_progression_for_all();
            this->resolve_classroom_for_all();
            this->process_washroom_needs_for_all();
        }

        // Day State 3: In Hall
        else if (this->day_state == 3) {
            this->individual_disease_progression_for_all();
            this->interaction_among_friends_for_all();
            this->process_washroom_needs_for_all();
        }

        // Things to do every minute:
        this->minute_counter++;
        this->decay_washroom_concentration();

        // Things to do every 15 minutes:
        if (this->minute_counter % 15 == 0)
            this->print_population_sizes();
        
        // Things to do every day:
        if (this->minute_counter == kMinutesPerDay) {
		    this->day_counter++;
            this->minute_counter = 0;
            
            this->clean_washrooms();

            // Check if the simulation reached a steady state
            if (this->check_for_steady()) break;

            this->log();
            this->last_day = std::chrono::high_resolution_clock::now();
        }
    }
    this->population_out.close();
    std::cout << "Simulation " << this->sim_id << " completed successfully!\n";
}

// Initialize all the values for the simulation
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

    assign_lunch_friends(this->grade9_agents, this->classrooms);
    assign_lunch_friends(this->grade10_agents, this->classrooms);
    assign_lunch_friends(this->grade11_agents, this->classrooms);
    assign_lunch_friends(this->grade12_agents, this->classrooms);

    this->create_vaccinated();
    this->pick_random_sick();

	this->export_agent_data(this->grade9_agents, "grade9.txt");
	this->export_agent_data(this->grade10_agents, "grade10.txt");
	this->export_agent_data(this->grade11_agents, "grade11.txt");
	this->export_agent_data(this->grade12_agents, "grade12.txt");
}

void Simulation::populate_agent_vector() {
    for (int i = 0; i < kGrade9Population; i++) {
        Agent agent(i, 0, kODEMode);
        this->grade9_agents.push_back(agent);
    }

    for (int i = 0; i < kGrade10Population; i++) {
        Agent agent(i, 1, kODEMode);
        this->grade10_agents.push_back(agent);
    }

    for (int i = 0; i < kGrade11Population; i++) {
        Agent agent(i, 2, kODEMode);
        this->grade11_agents.push_back(agent);
    }

    for (int i = 0; i < kGrade12Population; i++) {
        Agent agent(i, 3, kODEMode);
        this->grade12_agents.push_back(agent);
    }
}

// =============================
// = AGENT FUNCTIONS (FOR ALL) =
// =============================

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

void Simulation::resolve_classroom_for_all() {
    this->resolve_classroom(this->grade9_agents);
    this->resolve_classroom(this->grade10_agents);
    this->resolve_classroom(this->grade11_agents);
    this->resolve_classroom(this->grade12_agents);
}

// ===================
// = AGENT FUNCTIONS =
// ===================

void Simulation::individual_disease_progression(std::vector<Agent>& agent_vector) {
    for (auto& agent : agent_vector) {
        agent.individual_disease_progression();
    }
}

void Simulation::interaction_among_friends(std::vector<Agent>& agent_vector) {
    for (auto& agent : agent_vector) {
        if (this->friends_distribution(mt) and !agent.at_home)
            agent.interact_with_friend_random();
    }
}

void Simulation::process_washroom_needs(std::vector<Agent>& agent_vector) {
    for (auto& agent : agent_vector) {
        agent.process_washroom_needs(this->school_washrooms);
    }
}

void Simulation::resolve_classroom(std::vector<Agent>& agent_vector) {
    for (auto& agent : agent_vector) {
        if (this->classroom_distribution(mt) and !agent.at_home)
        agent.resolve_classroom(this->current_period, this->classrooms);
    }
}

// ===========
// = LOGGING =
// ===========

void Simulation::log() {
    std::cout << "[" << this->sim_id << "]\t";
    std::cout << "Day " << this->day_counter;
    std::cout << " (" << kWeek[this->day_counter % 7] << ")\t";
    std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->last_day).count() << "]\n";
}

void Simulation::prep_output_file() {
    std::ofstream out;
    out.open(this->export_folder + "population_sizes.csv", std::ofstream::out | std::ofstream::trunc);
    out << "TIME,G9S,G9V,G9E,G9I,G9R,G10S,G10V,G10E,G10I,G10R,G11S,G11V,G11E,G11I,G11R,G12S,G12V,G12E,G12I,G12R\n";
    out.close();
}

void Simulation::print_population_sizes() {
	std::vector<int> grade9_population_sizes = get_population_sizes(this->grade9_agents);
	std::vector<int> grade10_population_sizes = get_population_sizes(this->grade10_agents);
	std::vector<int> grade11_population_sizes = get_population_sizes(this->grade11_agents);
	std::vector<int> grade12_population_sizes = get_population_sizes(this->grade12_agents); 

    this->population_out << (this->day_counter * kMinutesPerDay) + this->minute_counter << ",";

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

void Simulation::export_agent_data(std::vector<Agent>& agent_vector, std::string file_name) {
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

std::vector<int> Simulation::get_population_sizes(std::vector<Agent> &agent_vector) {
	int susceptible = 0, vaccinated = 0, exposed = 0, infected = 0, recovered = 0;
	for (auto& agent : agent_vector) {
		if (agent.susceptible) 
			susceptible++;
		else if (agent.vaccinated) 
			vaccinated++;
		else if (agent.exposed) 
			exposed++;
		else if (agent.infected) 
			infected++;
		else if (agent.recovered) 
			recovered++;
	}
	return std::vector<int>{susceptible, vaccinated, exposed, infected, recovered};
}

// ===========
// = DISEASE =
// ===========

void Simulation::create_vaccinated() {
	int grade9_vacc_population = (int) std::round(kGrade9Population * this->vacc_rate);
	int grade10_vacc_population = (int) std::round(kGrade10Population * this->vacc_rate);
	int grade11_vacc_population = (int) std::round(kGrade11Population * this->vacc_rate);
	int grade12_vacc_population = (int) std::round(kGrade12Population * this->vacc_rate);

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

void Simulation::pick_random_sick() {
    Agent *sick_agent = nullptr;

    switch (this->random_sick_grade(mt)) {
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

    sick_agent->get_infected();
    sick_agent->vaccinated = false;
}

void Simulation::decay_washroom_concentration() {
	for (auto &washroom : this->school_washrooms) {
		washroom *= kConcentrationDecayRate;
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

// ================
// = TIME-KEEPING =
// ================

unsigned short Simulation::determine_day_state() {
	auto between = [this](int lower, int upper) { return (this->minute_counter - lower) < (upper - lower); };

	if (between(0, kPreClass))
		return 0;
	else if (between(kPreClass, kPeriod1Start))
		return 1;
	else if (between(kPeriod1Start, kPeriod1End))
		return 2;
	else if (between(kPeriod1End, kPeriod2Start))
		return 3;
	else if (between(kPeriod2Start, kPeriod2End))
		return 1;
	else if (between(kPeriod2End, kPeriod3Start))
		return 3;
	else if (between(kPeriod3Start, kPeriod3End))
		return 2;
	else if (between(kPeriod3End, kPeriod4Start))
		return 3;
	else if (between(kPeriod4Start, kPeriod4End))
		return 2;
	else if (between(kPeriod1End, kPeriod5Start))
		return 3;
	else if (between(kPeriod5Start, kPeriod5End))
		return 2;
	else if (between(kPeriod5End, kPostClass))
		return 1;
	else if (between(kPostClass, kMinutesPerDay))
		return 0;
	else
		return 0;
}

unsigned short Simulation::determine_period() {
	auto between = [this](int lower, int upper) { return (this->minute_counter - lower) < (upper - lower); };

	if (between(kPeriod1Start, kPeriod1End))
		return 1;
	else if (between(kPeriod2Start, kPeriod2End))
		return 2;
	else if (between(kPeriod3Start, kPeriod3End))
		return 3;
	else if (between(kPeriod4Start, kPeriod4End))
		return 4;
	else if (between(kPeriod5Start, kPeriod5End))
		return 5;
	else
		return 0;
}

