#!/usr/bin/env python3
from common import serializeDiffs2, serializeDiffsSNP, serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
import sys
import numpy as np
from scipy.optimize import curve_fit
from numpy.polynomial.polynomial import polyfit, polyval
from fit_polynomial import fit_polynomial
import matplotlib.pyplot as plt

def scatter(diffs, deg, show=True, sampleID=0):
    x,y,_,_ = preprocessDiffsIntoFreqDist(diffs)
    coefs = fit_polynomial(x,y, deg)
    plt.scatter(x,y, s=10, alpha=0.4, marker="x", color='r')
    if show:
        plt.show()
    plt.clf()
    plt.scatter(np.log10(x),np.log10(y), s=10, alpha=0.4, marker="x", color='r')
    if show:
        plt.show()
        plt.scatter(np.log10(x),np.log10(y), s=10, alpha=0.4, marker="x", color='r')
    xfit = np.arange(0, np.log10(max(x)), 0.01)
    yfit = polyval(xfit, coefs)
    plt.plot(xfit,yfit)
    if show:
        plt.show()
    else:
        imgName = "sample{}_deg{}.png".format(sampleID, deg)
        print("Image {} saved.".format(imgName))
        plt.savefig(imgName, dpi=500)
        plt.clf()

def main():
    if len(sys.argv) != 4:
        print("Usage: python3 graph_distance.py <pickle SNP pos file> <sample# (-1 for all samples)> <poly degree>")
        sys.exit(1)

    diffs = serializeVCFSNP(sys.argv[1], int(sys.argv[2]))

    if int(sys.argv[2]) < 0:
        print("{} samples read".format(len(diffs)))
        for sampleID, sampleDiffs in diffs.items():
            scatter(sampleDiffs, int(sys.argv[3]), show=False, sampleID=sampleID)
    else:
        print("{} deltas read".format(len(diffs)))
        scatter(diffs, int(sys.argv[3]))
main()
