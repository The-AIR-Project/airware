import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def predict_current(v, p, th):
    return th[0] + th[1]*v + th[2]*v**2 + th[3]*p + th[4]*p**2 + th[5]*p*v #+ th[6]*p*v**2 + th[7]*p**2*v + th[8]*p**2*v**2

data = pd.read_excel("current_data.xlsx").as_matrix()

v = data[:, 0]
p = data[:, 1]
c = data[:, 2]

from mpl_toolkits.mplot3d.axes3d import Axes3D
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1, projection='3d')
ax.scatter(v, p, c)

X = np.row_stack((np.ones_like(v),
                 v,
                 v**2,
                 p,
                 p**2,
                 p*v))
B = (np.linalg.inv(X @ X.T) @ X @ c.T).T
print(B)

v_pr = np.arange(460, 551, 10)
p_pr = np.arange(1, 11, 1)
vs = []
ps = []
cs = np.zeros((len(p_pr), len(v_pr)))

for col in range(len(v_pr)):
    for row in range(len(p_pr)):
        vol = v_pr[col]
        pres = p_pr[row]
        vs.append(vol)
        ps.append(pres)
        cs[row, col] = predict_current(vol, pres, B)
V, P = np.meshgrid(v_pr, p_pr)
ax.plot_surface(V, P, cs, cmap='seismic', alpha=0.5)
plt.show()
