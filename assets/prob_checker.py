import os
import pandas as pd

outbreak_simulations = 0

population_sizes_files = []

# Get all the subdirectories
for (dir_path, dir_names, filesnames) in os.walk(os.getcwd()):
    population_sizes_files.append(dir_path + "/population_sizes.csv")

for population_file in population_sizes_files[1:]:
    print(population_file)
    data = pd.read_csv(population_file)
    recovered_num = max(
        [data['G9R'].tolist()[-1], data['G10R'].tolist()[-1], data['G11R'].tolist()[-1], data['G12R'].tolist()[-1]])
    if recovered_num > 1:
        outbreak_simulations += 1
print(outbreak_simulations, "/", len(population_sizes_files) - 1)
