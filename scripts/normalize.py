import sys
import re

d = {}
counter = 0
for line in open(sys.argv[1]).readlines():
    line = re.split(",| |\t", line.strip())
    if line[0][0] in ("%", "#"):
        continue
    v1 = int(line[0])
    v2 = int(line[1])
    if v1 not in d:
        d[v1] = counter
        counter+= 1
    if v2 not in d:
        d[v2] = counter
        counter += 1
    print(d[v1], d[v2])
