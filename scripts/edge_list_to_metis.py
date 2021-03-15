import sys
import math

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 edge_list_to_metis.py <metis_graph>", file=sys.stderr)
        sys.exit(1)

    adj_list = {}
    lines = [line.split() for line in open(sys.argv[1]).readlines()]
    edges = list(filter(lambda line: line[0] != "#", lines))
    edges = [[int(endpt) for endpt in edge] for edge in edges]
    n = 0
    e = 0
    max_node = 0
    min_node = math.inf
    for edge in edges:
        for node in (edge[0], edge[1]):
            if node not in adj_list:
                adj_list[node] = set()
                n += 1
                if node > max_node:
                    max_node = node
                if node < min_node:
                    min_node = node

        if edge[1] not in adj_list[edge[0]]:
            adj_list[edge[0]].add(edge[1])
            adj_list[edge[1]].add(edge[0])
            e += 1

    for node in list(range(1, max_node)) + list(range(1, min_node)):
        if node not in adj_list:
            adj_list[node] = set()

    if min_node == 0:
        for node in adj_list:
            adj_list[node] = list(map(lambda x: x + 1, adj_list[node]))

    print(n, e)
    for k in sorted(adj_list):
        print(*sorted(adj_list[k]))

if __name__ == "__main__":
    main()
