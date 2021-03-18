from sys import argv
import networkx as nx
import matplotlib.pyplot as plt
import re

def main():
    if len(argv) != 2:
        print("Usage: python3 visualize_snap.py <graph_filename>")
        exit(1)

    G = nx.Graph()
    color = "black"
    for edge in open(argv[1], "r").readlines():
        edge = edge.strip()
        if edge[0] == "#":
            if re.match(r"^color=", edge[1:]):
                color = edge[len("color=") + 1:]
            continue

        edge = edge.split()

        if len(edge) != 2:
            print("Error: Edge", edge, "contains more than two vertices.")
            return

        G.add_edge(int(edge[0]), int(edge[1]), color=color)
        color = "black"

    nx.draw_networkx(G, font_color='white', edge_color=nx.get_edge_attributes(G, "color").values())
    plt.show()

if __name__ == "__main__":
    main()
