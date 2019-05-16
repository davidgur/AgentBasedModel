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

int main(int argc, char *argv[]) {
	int num_of_simulations_per_rate = std::stoi(argv[1]);
	int sick_grade = std::stoi(argv[2]);

	std::cout << argv[0] << std::endl;
	std::cout << argv[1] << std::endl;
	std::cout << argv[2] << std::endl;
	std::cout << argv[3] << std::endl;

	int num_of_vacc_rates = argc - 1;

	std::vector<double> vacc_rates(num_of_vacc_rates);

	// Add all vaccination rates to the vector
	int vacc_rate_arg_count;
	for (vacc_rate_arg_count = 0; vacc_rate_arg_count < num_of_vacc_rates; vacc_rate_arg_count++)
		vacc_rates[vacc_rate_arg_count] = std::stod(argv[2 + vacc_rate_arg_count]);

	// Every vaccination rate has some number of simulations.
	// Here, we iterate over all of the different vaccination
	// rates, and then we will run independent simulations
	// inside this loop.
	for (auto &vacc_rate : vacc_rates) {
		std::vector<std::thread> simulation_threads;
		std::vector<Simulation *> simulation_objects;

		// Remove any existing data.
		system(("rm -rf export/SIM_" + std::to_string(vacc_rate)).c_str());

		// Create new directory for data.
		mkdir(("export/SIM_" + std::to_string(vacc_rate)).c_str(), 0777);

		// Message to indicate new simulation
		std::cout << "===============" << std::endl;
		std::cout << "Starting " << num_of_simulations_per_rate << " simulations with "
		          << vacc_rate << " vaccination rate." << std::endl;
		std::cout << "===============" << std::endl;

		// Start making threads.
		for (int sim_id = 1; sim_id <= num_of_simulations_per_rate; sim_id++) {
			simulation_objects.push_back(new Simulation(sim_id, vacc_rate, sick_grade));
		}

		for (auto &simulation : simulation_objects) {
			simulation_threads.push_back(simulation->start_simulation_thread());
		}

		// Join all threads before proceeding with next "job"
		for (auto &thread : simulation_threads)
			thread.join();

		for (auto &simulation : simulation_objects)
			delete simulation;
		// Clear the vector
		simulation_objects.clear();
		simulation_threads.clear();
	}

	return 0;
}