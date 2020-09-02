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
a = 0.02 # Alpha

S_fig = plt.figure(figsize=(20, 10), dpi=1000)
V_fig = plt.figure(figsize=(20, 10), dpi=1000)
E_fig = plt.figure(figsize=(20, 10), dpi=1000)
I_fig = plt.figure(figsize=(20, 10), dpi=1000)
R_fig = plt.figure(figsize=(20, 10), dpi=1000)

SVEIR_fig = plt.figure(figsize=(20, 10), dpi=1000)

grade9_ax_s  = S_fig.add_subplot(221)
grade10_ax_s = S_fig.add_subplot(222)
grade11_ax_s = S_fig.add_subplot(223)
grade12_ax_s = S_fig.add_subplot(224)

grade9_ax_v  = V_fig.add_subplot(221)
grade10_ax_v = V_fig.add_subplot(222)
grade11_ax_v = V_fig.add_subplot(223)
grade12_ax_v = V_fig.add_subplot(224)

grade9_ax_e  = E_fig.add_subplot(221)
grade10_ax_e = E_fig.add_subplot(222)
grade11_ax_e = E_fig.add_subplot(223)
grade12_ax_e = E_fig.add_subplot(224)

grade9_ax_i  = I_fig.add_subplot(221)
grade10_ax_i = I_fig.add_subplot(222)
grade11_ax_i = I_fig.add_subplot(223)
grade12_ax_i = I_fig.add_subplot(224)

grade9_ax_r  = R_fig.add_subplot(221)
grade10_ax_r = R_fig.add_subplot(222)
grade11_ax_r = R_fig.add_subplot(223)
grade12_ax_r = R_fig.add_subplot(224)

grade9_ax_SVEIR = SVEIR_fig.add_subplot(221)
grade10_ax_SVEIR = SVEIR_fig.add_subplot(222)
grade11_ax_SVEIR = SVEIR_fig.add_subplot(223)
grade12_ax_SVEIR = SVEIR_fig.add_subplot(224)

subplots_s = [grade9_ax_s, grade10_ax_s, grade11_ax_s, grade12_ax_s]
subplots_v = [grade9_ax_v, grade10_ax_v, grade11_ax_v, grade12_ax_v]
subplots_e = [grade9_ax_e, grade10_ax_e, grade11_ax_e, grade12_ax_e]
subplots_i = [grade9_ax_i, grade10_ax_i, grade11_ax_i, grade12_ax_i]
subplots_r = [grade9_ax_r, grade10_ax_r, grade11_ax_r, grade12_ax_r]
subplots_SVEIR = [grade9_ax_SVEIR, grade10_ax_SVEIR, grade11_ax_SVEIR, grade12_ax_SVEIR]

for grade in range(9, 13):
    for data in by_grade_data[grade]['S']:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots_s[grade - 9].plot(x_axis, data, alpha=a, color='b')
        subplots_SVEIR[grade - 9].plot(x_axis, data, alpha=a, color='b')
    for data in by_grade_data[grade]['V']:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots_v[grade - 9].plot(x_axis, data, alpha=a, color='r')
        subplots_SVEIR[grade - 9].plot(x_axis, data, alpha=a, color='r')
    for data in by_grade_data[grade]['E']:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots_e[grade - 9].plot(x_axis, data, alpha=a, color='y')
        subplots_SVEIR[grade - 9].plot(x_axis, data, alpha=a, color='y')
    for data in by_grade_data[grade]['I']:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots_i[grade - 9].plot(x_axis, data, alpha=a, color='m')
        subplots_SVEIR[grade - 9].plot(x_axis, data, alpha=a, color='m')
    for data in by_grade_data[grade]['R']:
        num_of_days = len(data) * 15 // (24 * 60)
        x_axis = np.linspace(0, num_of_days, len(data))
        subplots_r[grade - 9].plot(x_axis, data, alpha=a, color='g')
        subplots_SVEIR[grade - 9].plot(x_axis, data, alpha=a, color='g')

    sigma = 4

    avg_last_s = np.mean([list(by_grade_data[grade]['S'][x])[-1] for x in range(len(by_grade_data[grade]['S']))])
    avg_last_v = np.mean([list(by_grade_data[grade]['V'][x])[-1] for x in range(len(by_grade_data[grade]['V']))])
    avg_last_e = np.mean([list(by_grade_data[grade]['E'][x])[-1] for x in range(len(by_grade_data[grade]['E']))])
    avg_last_i = np.mean([list(by_grade_data[grade]['I'][x])[-1] for x in range(len(by_grade_data[grade]['I']))])
    avg_last_r = np.mean([list(by_grade_data[grade]['R'][x])[-1] for x in range(len(by_grade_data[grade]['R']))])

    avg_s = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['S'], fillvalue=avg_last_s)]
    avg_v = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['V'], fillvalue=avg_last_v)]
    avg_e = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['E'], fillvalue=avg_last_e)]
    avg_i = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['I'], fillvalue=avg_last_i)]
    avg_r = [np.nanmean(y) for y in itertools.zip_longest(*by_grade_data[grade]['R'], fillvalue=avg_last_r)]

    num_of_days = max([len(avg_s), len(avg_v), len(avg_e), len(avg_i), len(avg_r)]) * 15 // (24 * 60)
    x_axis = np.linspace(0, num_of_days, max([len(avg_s), len(avg_v), len(avg_e), len(avg_i), len(avg_r)]))

    subplots_s[grade - 9].plot(x_axis, avg_s, color='b', linewidth=3.0)
    subplots_v[grade - 9].plot(x_axis, avg_v, color='r', linewidth=3.0)
    subplots_e[grade - 9].plot(x_axis, avg_e, color='y', linewidth=3.0)
    subplots_i[grade - 9].plot(x_axis, avg_i, color='m', linewidth=3.0)
    subplots_r[grade - 9].plot(x_axis, avg_r, color='g', linewidth=3.0)

    subplots_SVEIR[grade - 9].plot(x_axis, avg_s, color='b', linewidth=3.0)
    subplots_SVEIR[grade - 9].plot(x_axis, avg_v, color='r', linewidth=3.0)
    subplots_SVEIR[grade - 9].plot(x_axis, avg_e, color='y', linewidth=3.0)
    subplots_SVEIR[grade - 9].plot(x_axis, avg_i, color='m', linewidth=3.0)
    subplots_SVEIR[grade - 9].plot(x_axis, avg_r, color='g', linewidth=3.0)

    legend_lines = [Line2D([0], [0], color='b'),
                    Line2D([0], [0], color='r'),
                    Line2D([0], [0], color='y'),
                    Line2D([0], [0], color='m'),
                    Line2D([0], [0], color='g')]

    subplots_s[grade - 9].set_ylabel('# of people')
    subplots_s[grade - 9].set_xlabel('Time (days)')
    subplots_s[grade - 9].set_title('Grade ' + str(grade) + ' Susceptible population')

    subplots_v[grade - 9].set_ylabel('# of people')
    subplots_v[grade - 9].set_xlabel('Time (days)')
    subplots_v[grade - 9].set_title('Grade ' + str(grade) + ' Vaccinated population')

    subplots_e[grade - 9].set_ylabel('# of people')
    subplots_e[grade - 9].set_xlabel('Time (days)')
    subplots_e[grade - 9].set_title('Grade ' + str(grade) + ' Exposed population')

    subplots_i[grade - 9].set_ylabel('# of people')
    subplots_i[grade - 9].set_xlabel('Time (days)')
    subplots_i[grade - 9].set_title('Grade ' + str(grade) + ' Infected population')

    subplots_r[grade - 9].set_ylabel('# of people')
    subplots_r[grade - 9].set_xlabel('Time (days)')
    subplots_r[grade - 9].set_title('Grade ' + str(grade) + ' Recovered population')

    legend_lines = [Line2D([0], [0], color='b'),
                    Line2D([0], [0], color='r'),
                    Line2D([0], [0], color='y'),
                    Line2D([0], [0], color='m'),
                    Line2D([0], [0], color='g')]
    subplots_SVEIR[grade - 9].legend(legend_lines, ['Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered'])

    subplots_SVEIR[grade - 9].set_ylabel('# of people')
    subplots_SVEIR[grade - 9].set_xlabel('Time (days)')
    subplots_SVEIR[grade - 9].set_title('Grade ' + str(grade))

# Pickle
S_fig.savefig(sys.argv[2] + "S.pdf", format='pdf', dpi=1000)
V_fig.savefig(sys.argv[2] + "V.pdf", format='pdf', dpi=1000)
E_fig.savefig(sys.argv[2] + "E.pdf", format='pdf', dpi=1000)
I_fig.savefig(sys.argv[2] + "I.pdf", format='pdf', dpi=1000)
R_fig.savefig(sys.argv[2] + "R.pdf", format='pdf', dpi=1000)
SVEIR_fig.savefig(sys.argv[2] + "SVEIR.pdf", format='pdf', dpi=1000)
