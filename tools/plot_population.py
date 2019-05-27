# Creates an average plot for a given folder
# based on the population sizes at any given
# time point.

# David Gurevich
# david (at) gurevich (dot) ca

import os
import sys
import itertools
import pandas as pd
import pickle as pl
import numpy as np
import random
import matplotlib.pyplot as plt

from glob import glob
from scipy.ndimage.filters import gaussian_filter1d as gauss
from matplotlib.lines import Line2D

# (1) Identity all population files
population_files = {}
folders = glob(sys.argv[1])

for folder in folders:
    population_files[folder[:-1]] = []

    for i, (par_dir, _, _) in enumerate(os.walk(os.getcwd() + '/' + folder)):
        if i != 0:
            population_files[folder[:-1]].append(par_dir + '/population_sizes.csv')

# (2) Extract data from all population files
by_grade_data = {9: {'S':[], 'V':[], 'E':[], 'I':[], 'R':[]}, 
                10: {'S':[], 'V':[], 'E':[], 'I':[], 'R':[]}, 
                11: {'S':[], 'V':[], 'E':[], 'I':[], 'R':[]}, 
                12: {'S':[], 'V':[], 'E':[], 'I':[], 'R':[]}}

for _, population_file_list in population_files.items():
    for population_file in population_file_list:
        data = pd.read_csv(population_file)

        by_grade_data[9]['S'].append(data['G9S'])
        by_grade_data[9]['V'].append(data['G9V'])
        by_grade_data[9]['E'].append(data['G9E'])
        by_grade_data[9]['I'].append(data['G9I'])
        by_grade_data[9]['R'].append(data['G9R'])

        by_grade_data[10]['S'].append(data['G10S'])
        by_grade_data[10]['V'].append(data['G10V'])
        by_grade_data[10]['E'].append(data['G10E'])
        by_grade_data[10]['I'].append(data['G10I'])
        by_grade_data[10]['R'].append(data['G10R'])

        by_grade_data[11]['S'].append(data['G11S'])
        by_grade_data[11]['V'].append(data['G11V'])
        by_grade_data[11]['E'].append(data['G11E'])
        by_grade_data[11]['I'].append(data['G11I'])
        by_grade_data[11]['R'].append(data['G11R'])

        by_grade_data[12]['S'].append(data['G12S'])
        by_grade_data[12]['V'].append(data['G12V'])
        by_grade_data[12]['E'].append(data['G12E'])
        by_grade_data[12]['I'].append(data['G12I'])
        by_grade_data[12]['R'].append(data['G12R'])

# (3) Plotting
vacc_rate = 100 * (by_grade_data[9]['V'][0][0] / (by_grade_data[9]['S'][0][0] + by_grade_data[9]['V'][0][0] + 1))
a = 0.1 # Alpha

fig = plt.figure(figsize=(20,10), dpi=100)
grade9_ax  = fig.add_subplot(221)
grade10_ax = fig.add_subplot(222)
grade11_ax = fig.add_subplot(223)
grade12_ax = fig.add_subplot(224)

subplots = [grade9_ax, grade10_ax, grade11_ax, grade12_ax]

for grade in range(9, 13):
    # Pick 15 random simulations to pick from:
    s_selection = random.sample(by_grade_data[grade]['S'], 15)
    v_selection = random.sample(by_grade_data[grade]['V'], 15)
    e_selection = random.sample(by_grade_data[grade]['E'], 15)
    i_selection = random.sample(by_grade_data[grade]['I'], 15)
    r_selection = random.sample(by_grade_data[grade]['R'], 15)

    for data in s_selection: 
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots[grade - 9].plot(x_axis, data, alpha=a, color='b')
    for data in v_selection:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots[grade - 9].plot(x_axis, data, alpha=a, color='r')
    for data in e_selection:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots[grade - 9].plot(x_axis, data, alpha=a, color='y')
    for data in i_selection:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots[grade - 9].plot(x_axis, data, alpha=a, color='m')
    for data in r_selection:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots[grade - 9].plot(x_axis, data, alpha=a, color='g')
    
    sigma = 4

    avg_last_s = np.mean([list(s_selection[x])[-1] for x in range(len(s_selection))])
    avg_last_v = np.mean([list(v_selection[x])[-1] for x in range(len(v_selection))])
    avg_last_e = np.mean([list(e_selection[x])[-1] for x in range(len(e_selection))])
    avg_last_i = np.mean([list(i_selection[x])[-1] for x in range(len(i_selection))])
    avg_last_r = np.mean([list(r_selection[x])[-1] for x in range(len(r_selection))])

    avg_s = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['S'], fillvalue=avg_last_s)]
    avg_v = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['V'], fillvalue=avg_last_v)]
    avg_e = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['E'], fillvalue=avg_last_e)]
    avg_i = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['I'], fillvalue=avg_last_i)]
    avg_r = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['R'], fillvalue=avg_last_r)]

    num_of_days = max([len(avg_s), len(avg_v), len(avg_e), len(avg_i), len(avg_r)]) * 15 // (24 * 60)
    x_axis = np.linspace(0, num_of_days, max([len(avg_s), len(avg_v), len(avg_e), len(avg_i), len(avg_r)]))

    subplots[grade - 9].plot(x_axis, avg_s, color='b')
    subplots[grade - 9].plot(x_axis, avg_v, color='r')
    subplots[grade - 9].plot(x_axis, avg_e, color='y')
    subplots[grade - 9].plot(x_axis, avg_i, color='m')
    subplots[grade - 9].plot(x_axis, avg_r, color='g')

    legend_lines = [Line2D([0], [0], color='b'),
                    Line2D([0], [0], color='r'),
                    Line2D([0], [0], color='y'),
                    Line2D([0], [0], color='m'),
                    Line2D([0], [0], color='g')]
    subplots[grade - 9].legend(legend_lines, ['Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered'])

    subplots[grade - 9].set_ylabel('# of people')
    subplots[grade - 9].set_xlabel('Time (days)')
    subplots[grade - 9].set_title('Grade ' + str(grade))

fig.suptitle(
    'Compartmentalization of students in different grades within a secondary school during a measles outbreak\n (Vaccination rate: ' + str(round(vacc_rate, 2)) + '%)')


# Pickle
pl.dump(fig, open(sys.argv[2], 'wb+'))
