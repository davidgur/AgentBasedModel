import os
import pandas as pd

from glob import glob
from pprint import pprint

population_files = {}
vacc_folders = glob('*/')

for vacc_folder in vacc_folders:
    population_files[vacc_folder[:-1]] = []

for vacc_folder in vacc_folders:
    for i, (par_dir, _, _) in enumerate(os.walk(os.getcwd() + "/" + vacc_folder)):
        if i != 0:
            population_files[vacc_folder[:-1]].append(par_dir+"/population_sizes.csv")

for sim_id, population_file_list in population_files.items():
    outbreak_simulations_1 = 0
    outbreak_simulations_2 = 0
    outbreak_simulations_5 = 0
    outbreak_simulations_10 = 0
    for population_file in population_file_list:
        data = pd.read_csv(population_file)
        recovered_num = max([data['G9R'][data.index[-1]], data['G10R'][data.index[-1]], data['G11R'][data.index[-1]], data['G12R'][data.index[-1]] ])
        if recovered_num > 1:
            outbreak_simulations_1 += 1
        if recovered_num > 2:
            outbreak_simulations_2 += 1
        if recovered_num > 5:
            outbreak_simulations_5 += 1
        if recovered_num > 10:
            outbreak_simulations_10 += 1

    outbreak_simulations_1 /= len(population_file_list)
    outbreak_simulations_2 /= len(population_file_list)
    outbreak_simulations_5 /= len(population_file_list)
    outbreak_simulations_10 /= len(population_file_list)
    print(sim_id[4:], outbreak_simulations_1, outbreak_simulations_2, outbreak_simulations_5, outbreak_simulations_10, sep=",")