#!/usr/bin/env python3
from common import serializeDiffs2, serializeDiffsSNP, serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
import sys
import numpy as np
from scipy.optimize import curve_fit
from numpy.polynomial.polynomial import polyfit, polyval

import matplotlib.pyplot as plt

def scatter(diffs):
    x,y = preprocessDiffsIntoFreqDist(diffs)
    # popt = fit(x,y)
    coefs = fit2(np.log10(x),np.log10(y))
    # plawcoefs = fit(np.log10(x),np.log10(y)) 
    plt.scatter(np.log10(x),np.log10(y), s=2, alpha=0.2)
    xfit = np.arange(0, 5, 0.01)
    # yfit = poly(xfit, popt[0], popt[1], popt[2], popt[3], popt[4], popt[5])
    yfit = polyval(xfit, coefs)
    # yfit = logistic(xfit, plawcoefs[0], plawcoefs[1], plawcoefs[2])
    plt.plot(xfit,yfit)
    plt.show()


def powerlaw(x, alpha, beta, gamma, d0):
    y = x**(alpha/gamma) + (x/d0)**(beta/gamma)
    print(x.shape)
    print(y.shape)
    return y**gamma

def logistic(x, a,b,c):
    return (a)/ (1 + np.exp(1)**(x-b)) - c

def fit(x,y):
    popt, pcov = curve_fit(logistic, x,y, bounds=(0,6))
    print(popt)
    return popt

def fit2(x,y):
    coeffs = polyfit(x,y, 6)
    print(coeffs)
    return coeffs

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
