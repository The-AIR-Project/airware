import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import numpy as np

def ang_to_vol(ang):
    return 9.11458e-5*ang**3 - 2.315848e-2*ang**2 + 4.178385*ang - 143.66629

def vol_to_ang(vol):
    return 4.89427e-7*vol**3 - 8.40105e-4*vol**2 + 0.64294*vol + 28.072327

def vol_to_ang2(vol, theta):
    return theta[0] * np.power(vol, 1/3.0) + theta[1] * np.sqrt(vol) + theta[2] * vol + theta[3]


raw_ang = np.array([90, 90, 90, 90, 110, 110, 110, 110, 130, 130, 130, 130, 150, 150, 150, 150, 170, 170, 170, 170, 190, 190, 190, 190, 210, 210, 210, 210])
raw_vol = np.array([110, 110, 110, 110, 160, 160, 160, 160, 205, 210, 205, 210, 270, 270, 270, 265, 340, 340, 345, 345, 445, 440, 445, 445, 550, 555, 555, 555])
ang = np.array([90, 110, 130, 150, 170, 190, 210])
vol = np.array([110, 160, 207.5, 268.75, 342.5, 443.75, 553.75])
devs = np.zeros(len(raw_vol))
devs_minmax = []
dmin, dmax = 0, 0
counter = -1
for i in range(len(raw_vol)):
    if i % 4 == 0:
        if i != 0:
            devs_minmax.append([dmin, dmax])
        counter += 1
        dmin, dmax = 0, 0

    devs[i] = raw_vol[i] - vol[counter]
    if devs[i] > dmax:
        dmax = devs[i]
    elif devs[i] < dmin:
        dmin = devs[i]
devs_minmax.append([dmin, dmax])
val_ang = np.array([105, 120, 135, 165, 180, 195])
val_vol = np.array([145, 180, 220, 325, 390, 465])
phi = np.zeros((len(vol), 4))
phi[:, 0] = np.power(vol, 1/3.0)
phi[:, 1] = np.power(vol, 1/2.0)
phi[:, 2] = vol
phi[:, 3] = 1
theta = np.linalg.inv(phi.T @ phi) @ phi.T @ ang
pred_vol = ang_to_vol(val_ang)
pred_ang = vol_to_ang(val_vol)
pred_ang2 = vol_to_ang2(val_vol, theta)
ang_samples = np.arange(80, 220, 0.5)
vol_samples = np.arange(100, 565, 0.5)
trendline = [ang_samples, ang_to_vol(ang_samples)]
inv_trend = [vol_samples, vol_to_ang(vol_samples)]
inv_trend2 = [vol_samples, vol_to_ang2(vol_samples, theta)]

# plt.hist(raw_vol, 240, (100, 600))
# plt.title('Volume Measurement Distributions')
# plt.xlabel('Volume (mL)')
# plt.ylabel('Frequency')
# plt.text(110, 4, '90'+u'\N{DEGREE SIGN}')
# plt.text(160, 4, '110'+u'\N{DEGREE SIGN}')
# plt.text(210, 2, '130'+u'\N{DEGREE SIGN}')
# plt.text(270, 3, '150'+u'\N{DEGREE SIGN}')
# plt.text(345, 2, '170'+u'\N{DEGREE SIGN}')
# plt.text(445, 3, '190'+u'\N{DEGREE SIGN}')
# plt.text(555, 3, '210'+u'\N{DEGREE SIGN}')
plt.figure()
plt.title(f'Output Volume vs Motor Command')# - RMSE: {round(np.sqrt(np.mean(np.square(val_vol - pred_vol))), 3)} mL')
print(np.mean(np.abs(val_vol - pred_vol) / val_vol))
print(val_vol - pred_vol)
plt.xlabel('Motor Command')
plt.ylabel('Volume of Air (mL)')
plt.plot(*trendline, '--', label='Curve of Best Fit')
plt.scatter(ang, vol, label='Training Set')
# plt.scatter(val_ang, val_vol, label='Validation Set')
plt.text(77, 425, 'y = 9.115e-5$x^{3}$ - 2.316e-2$x^{2}$ + 4.178x - 143.666')
plt.legend()
plt.figure()
plt.title(f'Motor Command vs Output Volume')# - RMSE: {round(np.sqrt(np.mean(np.square(val_ang - pred_ang2))), 3)}' + u'\N{DEGREE SIGN}')
print(np.mean(np.abs(val_ang - pred_ang2) / val_ang))
plt.xlabel('Desired Volume (mL)')
plt.ylabel('Motor Command')
plt.plot(*inv_trend2, '--', label="Curve of Best Fit")
plt.text(200, 90, f'{round(theta[0], 3)}$\sqrt[3]{{x}}$ + {round(theta[1], 3)}$\sqrt{{x}}$ - {round(abs(theta[2]), 3)}x + {round(abs(theta[3]), 3)}')
plt.scatter(vol, ang, label='Training Set')
trend2_ests = theta[0] * np.power(vol, 1/3.0) + theta[1] * np.power(vol, 1/2.0) + theta[2] * vol + theta[3]
SS_reg = np.sum((trend2_ests - np.mean(ang))**2)
SS_tot = np.sum((ang - np.mean(ang))**2)
print(f'R^2 = {SS_reg / SS_tot}')
# plt.scatter(val_vol, val_ang, label='Validation Set')
# plt.text(77, 425, 'y = 9.115e-5$x^{3}$ - 2.316e-2$x^{2}$ + 4.178x - 143.666')
plt.legend()
plt.figure()
plt.title('Volume Deviations')
plt.xlabel('Motor Command')
plt.ylabel('Deviation from Mean (mL)')
plt.scatter(raw_ang, devs)
ax = plt.gca()
plt.plot([80, 220], [0, 0], '--')
for i in range(len(ang)):
    l = mlines.Line2D([ang[i],ang[i]], [devs_minmax[i][0],devs_minmax[i][1]])
    ax.add_line(l)
plt.ylim((-5, 5))
plt.show()
