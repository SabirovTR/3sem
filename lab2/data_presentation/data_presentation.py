import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 1000, 1)
DiscretState = np.arange(0, 1000, 1.0)
SegmentState = np.arange(0, 1000, 1.0)
SetState = np.arange(0, 1000, 1.0)

for i in range(1000):
    DiscretState[i] = float(input())
    SegmentState[i] = float(input())
    SetState[i] = float(input())

fig, ax = plt.subplots()
ax.scatter(x, DiscretState, 2, label = 'DiscretState')
ax.scatter(x, SegmentState, 2, label = 'SegmentState')
ax.scatter(x, SetState, 2, label = 'SetState')
ax.legend()
ax.set_xlabel('Верхняя граница, 10^2')
ax.set_ylabel('Вероятность')
plt.show()