import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

fig, ax = plt.subplots()
df = pd.read_csv("final.csv")

#y1 = df["None Lowest Score"]
#y1 = df["Trivial Lowest Score"]
#y1 = df["Vertex Lowest Score"]
#y1 = df["Edge Lowest Score"]
#y1 = df["Full Lowest Score"]
#y2 = df["Full First"]
#y1 = df["Vertex Time"]
y1 = df["Edge Time"]
y2 = df["Full Time"]


ax.set_yscale('log', base=2)
plt.xlabel('\nTestcase (Ordered by Difficulty)', fontsize=14)
plt.xticks([])
plt.yticks([2**i for i in range(40)])
plt.ylabel('Recursive Calls', fontsize=14)
plt.grid(True)

width = 0.4
plt.bar(range(len(y1)), y1, width=width)
plt.bar(np.arange(len(y2)) + width, y2, width=width)
#plt.bar(np.arange(len(y3)) + 2 * width, y3, width=width)

plt.show()
