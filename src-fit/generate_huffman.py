import huffman
import sys
from common import serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
from numpy.polynomial.polynomial import polyval
from fit_polynomial import fit_polynomial
import numpy as np

def generate_freq_codebook(counter):
    # print(counter)
    print("freq distance count: {}".format(len(counter)))
    return huffman.codebook(counter.items())

def generate_polynomial_codebook(coeffs, n, maxDistance, originalCounter):
    ctr = Counter()
    for distance, _ in originalCounter.items():
        freq = 10**polyval(np.log10(distance), coeffs)
        ctr[distance] = int(freq* n)
    print("poly distance count: {}".format(len(originalCounter)))
    return huffman.codebook(ctr.items())

def compute_cost(book, ctr):
    res = 0
    for distance, count in ctr.items():
        res += len(book[distance]) * count
    return res

def main():
    if len(sys.argv) != 4:
        print("Usage: ./generate_huffman.py <pickle SNP pos file> <sample #> <polynomial degree>")
        sys.exit(1)

    diffs = serializeVCFSNP(sys.argv[1], int(sys.argv[2]))
    x,y,totalCount,ctr = preprocessDiffsIntoFreqDist(diffs)
    bookFreq = generate_freq_codebook(ctr)
    coeffs = fit_polynomial(x,y, int(sys.argv[3]))
    bookPoly = generate_polynomial_codebook(coeffs, totalCount, max(x), ctr)
    costFreq = compute_cost(bookFreq, ctr)
    costPoly = compute_cost(bookPoly, ctr)

    print(costFreq)
    print(costPoly)
    print(costPoly / costFreq)
main()
