"""
@file plot_population.py

@brief Utility to plot output file of simulation

@author David Gurevich
Contact: david(at)gurevich.ca
"""

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Get all csv values and organize them
population_df = pd.read_csv('population_sizes.csv', delimiter=',')

G9S = population_df['G9S'].tolist()
G9V = population_df['G9V'].tolist()
G9E = population_df['G9E'].tolist()
G9I = population_df['G9I'].tolist()
G9R = population_df['G9R'].tolist()

G10S = population_df['G10S'].tolist()
G10V = population_df['G10V'].tolist()
G10E = population_df['G10E'].tolist()
G10I = population_df['G10I'].tolist()
G10R = population_df['G10R'].tolist()

G11S = population_df['G11S'].tolist()
G11V = population_df['G11V'].tolist()
G11E = population_df['G11E'].tolist()
G11I = population_df['G11I'].tolist()
G11R = population_df['G11R'].tolist()

G12S = population_df['G12S'].tolist()
G12V = population_df['G12V'].tolist()
G12E = population_df['G12E'].tolist()
G12I = population_df['G12I'].tolist()
G12R = population_df['G12R'].tolist()

G9_SVEIR = [G9S, G9V, G9E, G9I, G9R]
G10_SVEIR = [G10S, G10V, G10E, G10I, G10R]
G11_SVEIR = [G11S, G11V, G11E, G11I, G11R]
G12_SVEIR = [G12S, G12V, G12E, G12I, G12R]

vacc_rate = 100 * (G12V[0] / (G12S[0] + G12V[0] + G12E[0] + G12I[0] + G12R[0]))
num_of_days = len(G12R) // (24 * 60)
x_axis = np.linspace(0, num_of_days, len(G12R))
# Plotting the data
fig = plt.figure()

grade9_ax = fig.add_subplot(221)
grade10_ax = fig.add_subplot(222)
grade11_ax = fig.add_subplot(223)
grade12_ax = fig.add_subplot(224)

for data in G9_SVEIR:
    grade9_ax.plot(x_axis, np.array(data))
    grade9_ax.legend(['Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered'], loc=7)

for data in G10_SVEIR:
    grade10_ax.plot(x_axis, np.array(data))
    grade10_ax.legend(['Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered'], loc=7)

for data in G11_SVEIR:
    grade11_ax.plot(x_axis, np.array(data))
    grade11_ax.legend(['Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered'], loc=7)

for data in G12_SVEIR:
    grade12_ax.plot(x_axis, np.array(data))
    grade12_ax.legend(['Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered'], loc=7)

grade9_ax.set_ylabel("# of people")
grade10_ax.set_ylabel("# of people")
grade11_ax.set_ylabel("# of people")
grade12_ax.set_ylabel("# of people")

grade9_ax.set_xlabel("Time (days)")
grade10_ax.set_xlabel("Time (days)")
grade11_ax.set_xlabel("Time (days)")
grade12_ax.set_xlabel("Time (days)")

grade9_ax.set_title("Grade 9")
grade10_ax.set_title("Grade 10")
grade11_ax.set_title("Grade 11")
grade12_ax.set_title("Grade 12")

fig.suptitle(
    'Compartmentalization of students in different grades within a secondary school during a measles outbreak\n (Vaccination rate: ' + str(
        round(vacc_rate, 2)) + '%)')
mpl.rcParams['figure.figsize'] = [10.0, 8.0]
mpl.rcParams['figure.dpi'] = 100
mpl.rcParams['savefig.dpi'] = 120
mpl.rcParams['image.cmap'] = 'jet'

plt.show()
