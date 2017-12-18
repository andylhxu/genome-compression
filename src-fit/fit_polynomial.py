from common import serializeDiffs2, serializeDiffsSNP, serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
import sys
import numpy as np
from scipy.optimize import curve_fit
from numpy.polynomial.polynomial import polyfit, polyval

def fit_polynomial(x,y):
    coefs = fit2(np.log10(x),np.log10(y))
    return coefs

def fit2(x,y):
    coeffs = polyfit(x,y, 20)
    print(coeffs)
    return coeffs
