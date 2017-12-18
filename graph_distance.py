#!/usr/bin/env python3
from common import serializeDiffs2, serializeDiffsSNP, serializeVCFSNP
from collections import Counter
import sys
import numpy as np

import matplotlib.pyplot as plt

def graph(diffs):
    x = []
    ctr = Counter()
    for i in range(len(diffs) - 1):
        dist = diffs[i+1][1] - diffs[i][1]
        ctr[dist] += 1
        x.append(dist)
    print(ctr.most_common(10))

    plt.hist(x, bins=100,range=(0,1000))
    plt.show()

def scatter(diffs):
    ctr = Counter()
    totalCount = len(diffs)
    for i in range(len(diffs) - 1):
        # dist = diffs[i+1][1] - diffs[i][1]
        dist = diffs[i+1] - diffs[i]
        ctr[dist] += 1
    print(ctr.most_common(10))
    x = []
    y = []
    for item in ctr.items():
        if (item[0] == 0):
            continue
        x.append(item[0])
        y.append(item[1])

    plt.scatter(np.log10(x),np.log10(y), s=2, alpha=0.5)
    plt.show()

def main():
    if len(sys.argv) != 3 and len(sys.argv) != 2:
        print("Usage: ./graph_distance.py <delta file>")
        print("Usage: ./graph_distance.py <chr name> <delta file>")
        sys.exit(1)
    if len(sys.argv) == 2:
        # diffs = serializeDiffs2(sys.argv[1])
        diffs = serializeVCFSNP(sys.argv[1], 0)
        print("{} deltas read".format(len(diffs)))
        scatter(diffs)
    else:
        diffs = serializeDiffs(sys.argv[2], sys.argv[1])
        graph(diffs)
main()
