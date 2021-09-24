import numpy as np
import matplotlib.pyplot as plt

n = 1000
x = np.arange(0, n, 1.0)
DiscretState = np.arange(0, n, 1.0)
SegmentState = np.arange(0, n, 1.0)
SetState = np.arange(0, n, 1.0)
UnionState = np.arange(0, n, 1.0)
UnionStateudss = np.arange(0, n, 1.0)
InterStateids = np.arange(0, n, 1.0)
InterStateisss = np.arange(0, n, 1.0)

for i in range(n):
    DiscretState[i] = float(input())
    SegmentState[i] = float(input())
    SetState[i] = float(input())
    UnionState[i] = float(input())
    UnionStateudss[i] = float(input())
    InterStateids[i] = float(input())
    InterStateisss[i] = float(input())


fig, ax = plt.subplots()
ax.scatter(x, DiscretState, 6, label = 'DiscretState')
ax.scatter(x, SegmentState, 6, label = 'SegmentState')
ax.scatter(x, SetState, 6, label = 'SetState')
ax.scatter(x, UnionState, 6, label = 'UnionState(d, s, ss)')
ax.scatter(x, UnionStateudss, 6, label = 'UnionState(d, ss)')
ax.scatter(x, InterStateids, 6, label = 'InterState(d, s)')
ax.scatter(x, InterStateisss, 6, label = 'InterState(s, ss)')
ax.legend()
ax.set_xlabel('')
ax.set_ylabel('Вероятность')
plt.show()