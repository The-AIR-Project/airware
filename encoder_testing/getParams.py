import math as m

def get_params(os, t_p, kp):
    zeta = m.sqrt((m.log(os) ** 2) / (m.log(os) ** 2 + m.pi ** 2))
    wn = m.pi / (t_p * m.sqrt(1 - zeta ** 2))
    tau = 1 / (2 * zeta * wn)
    k1 = wn ** 2 * tau / kp
    return (k1, tau)

overshoots = [0.472, 0.426, 0.482, 0.442, 0.478, 0.432]
peak_times = [0.559, 0.554, 0.545, 0.546, 0.552, 0.559]
kps = [1, 1, 1, 1, 1, 1]
k1 = 0
tau = 0
for i in range(len(kps)):
    res = get_params(overshoots[i], peak_times[i], kps[i])
    k1 += res[0]
    tau += res[1]

print(f'K1 = {k1 / len(kps)}, tau = {tau / len(kps)}')
