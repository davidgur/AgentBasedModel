# Determining the R0 for a simulation

# David Gurevich
# david (at) gurevich (dot) ca

import os
import sys

import pandas as pd
import numpy as np

from glob import glob

# Get all the secondary infection files
r0_files = {}
folders = glob(sys.argv[1])

for folder in folders:
    r0_files[folder[:-1]] = []

    for i, (par_dir, _, _) in enumerate(os.walk(os.getcwd() + '/' + folder)):
        if i != 0:
            r0_files[folder[:-1]].append(par_dir + '/secondary_infections.csv')

# Get the R0 data
R0_data = []

for _, r0_file_list in r0_files.items():
    for r0_file in r0_file_list:
        data = pd.read_csv(r0_file)
        data.sort_values(by='SECONDARY_INFECTIONS') 
        R0_data.append(data['SECONDARY_INFECTIONS'].head())

# Print average
average_r0 = np.mean([np.mean(x) for x in R0_data])
print(average_r0)