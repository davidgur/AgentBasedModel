# Program to determine the class sizes
# David Gurevich (david at gurevich dot ca)

import os
import sys

import numpy as np

from pprint import pprint

# Open grade 9 files:
SIM_FOLDER = sys.argv[1] + "1/"

with open(SIM_FOLDER + "grade9.txt", "r") as grade9f:
    AGENT_DATA = []
    AGENT_DATA_START_INDEX = []
    CLASS_SIZE_LIST = [{}, {}, {}, {}, {}]
    GRADE_9_LINES = [line.strip() for line in grade9f.readlines()]
    for i, line in enumerate(GRADE_9_LINES):
        if line[0] == '[':
            AGENT_DATA_START_INDEX.append(i)

    for i in range(len(AGENT_DATA_START_INDEX) - 1):
        AGENT_DATA.append(GRADE_9_LINES[AGENT_DATA_START_INDEX[i]:AGENT_DATA_START_INDEX[i+1]])

    for agent in AGENT_DATA:
        # PERIOD 1
        CLASS_SIZE_LIST[0][agent[1]] = CLASS_SIZE_LIST[0].get(agent[1], 0) + 1
        # PERIOD 2
        CLASS_SIZE_LIST[1][agent[2]] = CLASS_SIZE_LIST[1].get(agent[2], 0) + 1
        # PERIOD 3
        CLASS_SIZE_LIST[2][agent[3]] = CLASS_SIZE_LIST[2].get(agent[3], 0) + 1
        # PERIOD 4
        CLASS_SIZE_LIST[3][agent[4]] = CLASS_SIZE_LIST[3].get(agent[4], 0) + 1
        # PERIOD 5
        CLASS_SIZE_LIST[4][agent[5]] = CLASS_SIZE_LIST[4].get(agent[5], 0) + 1

    # Average class size:
    class_avgs = []
    for period_dict in CLASS_SIZE_LIST:
        class_pops = []
        for key, val in period_dict.items():
            if key != "LUNCH": class_pops.append(val)
        avg_class_size = np.mean(class_pops)
        class_avgs.append(avg_class_size)

    print(class_avgs)
    print("AVERAGE CLASS SIZE:", np.nanmean(class_avgs))
