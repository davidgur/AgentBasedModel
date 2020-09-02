# Plot the infected population size
# for all simulations in a certain
# category (vaccination rate).
# Creates an average graph

# David Gurevich
# david (at) gurevich (dot) ca

import os
import sys
import pandas as pd
import numpy as np
import pickle as pl
import matplotlib.pyplot as plt

import itertools

from glob import glob
from scipy.signal import savgol_filter

# Step 1: Identify all population files
population_files = {}
folders = glob(sys.argv[1])

for folder in folders:
    population_files[folder[:-1]] = []

    for i, (par_dir, _, _) in enumerate(os.walk(os.getcwd() + '/' + folder)):
        if i != 0:
            population_files[folder[:-1]].append(par_dir + '/population_sizes.csv')

# Get the 'infected' data from every population file
infected_data = []

for _, population_file_list in population_files.items():
    for population_file in population_file_list:
        data = pd.read_csv(population_file)
        recovered_num = max([data['G9R'][data.index[-1]], data['G10R'][data.index[-1]], data['G11R'][data.index[-1]], data['G12R'][data.index[-1]] ])
        if recovered_num > 1:
            infected_data.append([sum(x) for x in zip(data['G9I'], data['G10I'], data['G11I'], data['G12I'])])

# Plot the at_home_data with low opacity
for data in infected_data:
    plt.plot(np.linspace(0, len(data) * 15 // (24 * 60), len(data)), data, alpha=0.1)

# Get the "average" at home plot
avg_data = savgol_filter([np.nanmedian(data_at_y) for data_at_y in itertools.zip_longest(*infected_data, fillvalue=0)], 89, 3)
plt.plot(np.linspace(0, len(avg_data) * 15 // (24*60), len(avg_data)), avg_data)
plt.xlabel("Time (days)")
plt.ylabel("# of people")

fig = plt.gcf()

# Pickle
plt.savefig(sys.argv[2] + ".pdf", format='pdf', dpi=1000)
