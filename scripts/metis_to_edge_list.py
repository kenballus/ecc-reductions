import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 metis_to_edge_list.py <metis_graph>", file=sys.stderr)
        sys.exit(1)

    lines = [line.split() for line in open(sys.argv[1]).readlines()]
    n = lines[0][0]
    e = lines[0][1]
    line_no = 1
    for line in lines[1:]:
        for node in line:
            print(line_no, node)
        line_no += 1

if __name__ == "__main__":
    main()
