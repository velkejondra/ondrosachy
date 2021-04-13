import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.colors import DivergingNorm

row = ["8", "7", "6", "5","4", "3", "2", "1"]
collumn= ["A","B","C","D","E","F","G","H"]

vals = np.array([
    [-50, -40, -30, -30, -30, -30, -40, -50],
    [-40, -20, 0,  5,   5,   0,  -20, -40],
    [-30, 5,   10,  15,  15,  10,  5,   -30],
    [-30, 0,   15,  20,  20,  15,  0,   -30],
    [-30, 5,   15,  20,  20,  15,  5,   -30],
    [-30, 0,   10,  15,  15,  10,  0,   -30],
    [-40, -20, 0,   0,   0,   0,   -20, -40],
    [-50, -40, -30, -30, -30, -30, -40, -50],
])
fig, ax = plt.subplots()
im = ax.imshow(vals, cmap="RdYlBu", norm=DivergingNorm(0, vmin=-50, vmax=20))
fig.colorbar(im)# We want to show all ticks...
ax.set_xticks(np.arange(len(row)))
ax.set_yticks(np.arange(len(collumn)))

ax.set_xticklabels(collumn)
ax.set_yticklabels(row)

plt.setp(ax.get_xticklabels(), ha="right",
         )

for i in range(len(row)):
    for j in range(len(collumn)):
        text = ax.text(j, i, vals[i, j],
                       ha="center", va="center", color="w")

ax.set_title("Heatmapa hodnot pozic pro koňe")
# fig.tight_layout()
# plt.show()
plt.savefig('/home/velkejondra/repos/chess_gitlab/Dokumentace/LaTeX_Template/images/heatmap.png', dpi=200)