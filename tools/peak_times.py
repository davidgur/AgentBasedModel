# Determining the time at which there is
# the highest infected population

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

# Get the infected data
infected_data = []

for _, population_file_list in population_files.items():
    for population_file in population_file_list:
        data = pd.read_csv(population_file)
        recovered_num = max([data['G9R'][data.index[-1]], data['G10R'][data.index[-1]], data['G11R'][data.index[-1]], data['G12R'][data.index[-1]] ])
        if recovered_num > 1:
            infected_data.append([sum(x) for x in zip(data['G9I'], data['G10I'], data['G11I'], data['G12I'])])

# Get the peak times
peak_times = []

for data in infected_data:
    peak_times.append(15 * data.index(max(data)))

print("\n\tMin peak time (days):", min(peak_times) // (24 * 60))
print("\tAverage peak time (days):", np.mean(peak_times) // (24*60))
print("\tMax peak time (days):", max(peak_times) // (24 * 60))
print("\tSTD (days):", np.std(peak_times) // (24*60))
