# schedule_maker.py
# March 27, 2019
# David Gurevich

# Super ugly program that parses a CSV file with all the courses that run at a secondary school.

import csv
import json

from pprint import pprint

with open('master_schedule.csv', 'r') as csvfile:
    data = list(csv.reader(csvfile))

    period1_classes = [row[0].strip() for row in data if row[0] != "" and row[0][0] != "Y" and row[0][3].isdigit()]
    period2_classes = [row[1].strip() for row in data if row[1] != "" and row[1][0] != "Y" and row[1][3].isdigit()]
    period3_classes = [row[2].strip() for row in data if row[2] != "" and row[2][0] != "Y" and row[2][3].isdigit()]
    period4_classes = [row[3].strip() for row in data if row[3] != "" and row[3][0] != "Y" and row[3][3].isdigit()]
    period5_classes = [row[4].strip() for row in data if row[4] != "" and row[4][0] != "Y" and row[4][3].isdigit()]

pprint(period1_classes)
pprint(period2_classes)
pprint(period3_classes)
pprint(period4_classes)
pprint(period5_classes)

period_1 = [[], [], [], []]
period_2 = [[], [], [], []]
period_3 = [[], [], [], []]
period_4 = [[], [], [], []]
period_5 = [[], [], [], []]

for c in period1_classes:
    period_1[int(c[3]) - 1].append(c.split()[1])

for c in period2_classes:
    period_2[int(c[3]) - 1].append(c.split()[1])

for c in period3_classes:
    period_3[int(c[3]) - 1].append(c.split()[1])

for c in period4_classes:
    period_4[int(c[3]) - 1].append(c.split()[1])

for c in period5_classes:
    period_5[int(c[3]) - 1].append(c.split()[1])

period1_json = {'grade9': period_1[0], 'grade10': period_1[1], 'grade11': period_1[2], 'grade12': period_1[3]}
period2_json = {'grade9': period_2[0], 'grade10': period_2[1], 'grade11': period_2[2], 'grade12': period_2[3]}
period3_json = {'grade9': period_3[0], 'grade10': period_3[1], 'grade11': period_3[2], 'grade12': period_3[3]}
period4_json = {'grade9': period_4[0], 'grade10': period_4[1], 'grade11': period_4[2], 'grade12': period_4[3]}
period5_json = {'grade9': period_5[0], 'grade10': period_5[1], 'grade11': period_5[2], 'grade12': period_5[3]}

with open('period1.json', 'w') as outfile:
    json.dump(period1_json, outfile)

with open('period2.json', 'w') as outfile:
    json.dump(period2_json, outfile)

with open('period3.json', 'w') as outfile:
    json.dump(period3_json, outfile)

with open('period4.json', 'w') as outfile:
    json.dump(period4_json, outfile)

with open('period5.json', 'w') as outfile:
    json.dump(period5_json, outfile)
