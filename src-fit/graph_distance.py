#!/usr/bin/env python3
from common import serializeDiffs2, serializeDiffsSNP, serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
import sys
import numpy as np
from scipy.optimize import curve_fit
from numpy.polynomial.polynomial import polyfit, polyval
from fit_polynomial import fit_polynomial
import matplotlib.pyplot as plt

def scatter(diffs):
    x,y,_,_ = preprocessDiffsIntoFreqDist(diffs)
    coefs = fit_polynomial(x,y)
    plt.scatter(np.log10(x),np.log10(y), s=2, alpha=0.2)
    xfit = np.arange(0, np.log10(max(x)), 0.01)
    yfit = polyval(xfit, coefs)
    plt.plot(xfit,yfit)
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
