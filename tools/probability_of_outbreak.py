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
    outbreak_simulations = 0
    for population_file in population_file_list:
        data = pd.read_csv(population_file)
        recovered_num = max([data['G9R'][data.index[-1]], data['G10R'][data.index[-1]], data['G11R'][data.index[-1]], data['G12R'][data.index[-1]] ])
        if recovered_num > 1:
            outbreak_simulations += 1
    print(sim_id[4:]+ ","+ str(outbreak_simulations / len(population_file_list)))
