import matplotlib.pyplot as plt
import networkx as nx

from matplotlib.lines import Line2D

G = nx.Graph()
color_map = []

# Grade 9
with open("grade9.txt", 'r') as grade9_file:
    grade9_lines = grade9_file.readlines()

grade9_new_agent = [i for i in range(len(grade9_lines)) if grade9_lines[i][0] == "["]
grade9_connection = [i for i in range(len(grade9_lines)) if grade9_lines[i].strip()[0] == "-"]

for i in range(len(grade9_new_agent) - 1):
    # Get agent name:
    node_name = grade9_lines[grade9_new_agent[i]]
    node_name = node_name.replace("[", "")
    node_name = node_name.replace("]", "")
    node_name = node_name.split()[1]
    G.add_node(node_name)
    color_map.append("red")
    # Get connections:
    connections = grade9_lines[grade9_connection[i] + 1:grade9_new_agent[i + 1]]
    for connection in connections:
        connection = connection.strip()
        G.add_edge(node_name, connection, weight=(100 if node_name[0]==connection[0] else 0))

color_map.append("red")

# Grade 10
with open("grade10.txt", 'r') as grade10_file:
    grade10_lines = grade10_file.readlines()

grade10_new_agent = [i for i in range(len(grade10_lines)) if grade10_lines[i][0] == "["]
grade10_connection = [i for i in range(len(grade10_lines)) if grade10_lines[i].strip()[0] == "-"]

grade10_nodes = []
grade10_edges = []
for i in range(len(grade10_new_agent) - 1):
    # Get agent name:
    node_name = grade10_lines[grade10_new_agent[i]]
    node_name = node_name.replace("[", "")
    node_name = node_name.replace("]", "")
    node_name = node_name.split()[1]
    G.add_node(node_name)
    color_map.append("green")
    # Get connections:
    connections = grade10_lines[grade10_connection[i] + 1:grade10_new_agent[i + 1]]
    for connection in connections:
        connection = connection.strip()
        G.add_edge(node_name, connection, weight=(100 if node_name[0]==connection[0] else 0))

color_map.append("green")

# Grade 11
with open("grade11.txt", 'r') as grade11_file:
    grade11_lines = grade11_file.readlines()

grade11_new_agent = [i for i in range(len(grade11_lines)) if grade11_lines[i][0] == "["]
grade11_connection = [i for i in range(len(grade11_lines)) if grade11_lines[i].strip()[0] == "-"]

for i in range(len(grade11_new_agent) - 1):
    # Get agent name:
    node_name = grade11_lines[grade11_new_agent[i]]
    node_name = node_name.replace("[", "")
    node_name = node_name.replace("]", "")
    node_name = node_name.split()[1]
    G.add_node(node_name)
    color_map.append("blue")
    # Get connections:
    connections = grade11_lines[grade11_connection[i] + 1:grade11_new_agent[i + 1]]
    for connection in connections:
        connection = connection.strip()
        G.add_edge(node_name, connection, weight=(100 if node_name[0]==connection[0] else 0))

color_map.append("blue")

# Grade 12
with open("grade12.txt", 'r') as grade12_file:
    grade12_lines = grade12_file.readlines()

grade12_new_agent = [i for i in range(len(grade12_lines)) if grade12_lines[i][0] == "["]
grade12_connection = [i for i in range(len(grade12_lines)) if grade12_lines[i].strip()[0] == "-"]
for i in range(len(grade12_new_agent) - 1):
    # Get agent name:
    node_name = grade12_lines[grade12_new_agent[i]]
    node_name = node_name.replace("[", "")
    node_name = node_name.replace("]", "")
    node_name = node_name.split()[1]
    G.add_node(node_name)
    color_map.append("magenta")
    # Get connections:
    connections = grade12_lines[grade12_connection[i] + 1:grade12_new_agent[i + 1]]
    for connection in connections:
        connection = connection.strip()
        G.add_edge(node_name, connection, weight=(100 if node_name[0]==connection[0] else 0))

color_map.append("magenta")

pos = nx.spectral_layout(G)

options = {
    'node_size': 35,
    'node_color': color_map,
    'line_color': 'white',
    'linewidths': 0,
    'width': 0.1,
    'with_labels': False
}

# Legend elements
legend_elements = [Line2D([0], [0], marker='o', color='red', label="Grade 9"),
                   Line2D([0], [0], marker='o', color='green', label="Grade 10"),
                   Line2D([0], [0], marker='o', color='blue', label="Grade 11"),
                   Line2D([0], [0], marker='o', color='magenta', label="Grade 12")]

nx.draw_spring(G, **options)


plt.legend(handles=legend_elements)
plt.show()
