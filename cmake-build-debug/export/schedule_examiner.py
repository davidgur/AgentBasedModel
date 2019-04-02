from pprint import pprint
from collections import Counter

with open("grade9.txt", 'r') as agent_file:
    f = agent_file.readlines()

f = [line.strip() for line in f]
f = [line for line in f if line[0] != "["]

agent_data = []
for i in range(0, len(f), 5):
    agent_data.append(f[i:i+5])

period1 = dict(Counter([agent[0] for agent in agent_data]))
period2 = dict(Counter([agent[1] for agent in agent_data]))
period3 = dict(Counter([agent[2] for agent in agent_data]))
period4 = dict(Counter([agent[3] for agent in agent_data]))
period5 = dict(Counter([agent[4] for agent in agent_data]))

print("Period 1")
pprint(period1)
print()

print("Period 2")
pprint(period2)
print()

print("Period 3")
pprint(period3)
print()

print("Period 4")
pprint(period4)
print()

print("Period 5")
pprint(period5)
print()
