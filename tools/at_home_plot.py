# Plots the number of students that are at home at
# any given point in time. Uses shaded error bars
# to display the values. Only simulations with an 
# outbreak are used for the plot

import os
import pandas as pd
import matplotlib.pyplot as plt

from glob import glob

# Step 1: Identify all population files
population_files = {}
folders = glob('*/')

for folder in folders:
    population_files[folder[:-1]] = []

    for i, (par_dir, _, _) in enumerate(os.walk(os.getcwd() + '/' + folder)):
        if i != 0:
            population_files[folder[:-1]].append(par_dir + '/population_sizes.csv')

# Get the 'at-home' data from every population file 
at_home_data = []

for _, population_file_list in population_files.items():
    for population_file in population_file_list:
        data = pd.read_csv(population_file)
        recovered_num = max([data['G9R'][data.index[-1]], data['G10R'][data.index[-1]], data['G11R'][data.index[-1]], data['G12R'][data.index[-1]] ])
        if recovered_num > 1:
            at_home_data.append([sum(x) for x in zip(data['G9A'], data['G10A'], data['G11A'], data['G12A'])])

# Get smallest at_home data
for data in at_home_data:
    plt.plot(range(len(data) / 24 * 15), data)
plt.show()
