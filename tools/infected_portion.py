# Determining the percentage of the popuation
# that ends up infected vs the population
# of susceptible agents.

# David Gurevich
# david (at) gurevich (dot) ca

import os
import sys

import pandas as pd
import numpy as np

from glob import glob

# Get all the population files
population_files = {}
folders = glob(sys.argv[1])

for folder in folders:
    population_files[folder[:-1]] = []

    for i, (par_dir, _, _) in enumerate(os.walk(os.getcwd() + '/' + folder)):
        if i != 0:
            population_files[folder[:-1]].append(par_dir + '/population_sizes.csv')

# Get the susceptible data and the infected data
susceptible_data = []

for _, population_file_list in population_files.items():
    for population_file in population_file_list:
        data = pd.read_csv(population_file)
        recovered_num = max([data['G9R'][data.index[-1]], data['G10R'][data.index[-1]], data['G11R'][data.index[-1]], data['G12R'][data.index[-1]] ])
        if recovered_num > 1:
            susceptible_data.append([sum(x) for x in zip(data['G9S'], data['G10S'], data['G11S'], data['G12S'])])

# Get initial susceptible population vs final susceptible population
proportions = []

for data in susceptible_data:
    initial_s = data[0]
    final_s = data[-1]

    proportions.append(final_s / initial_s)

print('Average proportion of infected vs susceptible:', 1 - np.mean(proportions))