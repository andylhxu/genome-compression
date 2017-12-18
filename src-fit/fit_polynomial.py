from common import serializeDiffs2, serializeDiffsSNP, serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
import sys
import numpy as np
from scipy.optimize import curve_fit
from numpy.polynomial.polynomial import polyfit, polyval

def fit_polynomial(x,y, degree):
    coefs = polyfit(np.log10(x),np.log10(y), degree)
    return coefs

