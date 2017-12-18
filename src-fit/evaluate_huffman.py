import huffman
import sys
from common import serializeVCFSNP, preprocessDiffsIntoFreqDist
from collections import Counter
from numpy.polynomial.polynomial import polyval
from fit_polynomial import fit_polynomial
import numpy as np
from scipy.stats import entropy

def generate_freq_codebook(counter):
    # print(counter)
    # print("freq distance count: {}".format(len(counter)))
    return huffman.codebook(counter.items())

def generate_polynomial_codebook(coeffs, n, maxDistance, originalCounter):
    ctr = Counter()
    totalCount = 0
    for distance, _ in originalCounter.items():
        freq = 10**polyval(np.log10(distance), coeffs)
        ctr[distance] = int(freq* n)
    # print("poly distance count: {}".format(len(originalCounter)))
    return huffman.codebook(ctr.items()), ctr

def compute_cost(book, ctr):
    res = 0
    for distance, count in ctr.items():
        res += len(book[distance]) * count
    return res

def process(name, diffs):
    print("===Evaluate polyfit huffman on {}===".format(name))
    x,y,totalCount,ctr = preprocessDiffsIntoFreqDist(diffs)
    bookFreq  = generate_freq_codebook(ctr)
    coeffs = fit_polynomial(x,y, int(sys.argv[3]))
    bookPoly, polyCtr = generate_polynomial_codebook(coeffs, totalCount, max(x), ctr)
    costFreq = compute_cost(bookFreq, ctr)
    costPoly = compute_cost(bookPoly, ctr)
    costFreqTable = len(x) * (np.log(max(x))+np.log(max(y)*totalCount))
    print("Bits using freq table: {}".format(costFreq))
    print("Bits using polynomial: {}".format(costPoly))
    bitsSacrificed = costPoly - costFreq
    print("Bits sacrificed using poly rather than using freq table: {}".format(bitsSacrificed))
    print("Bits would have needed to send freq table: {}".format(costFreqTable))

    # kl divergence
    y_poly = np.array([e[1] for e in polyCtr.items()])
    y_poly = y_poly / np.linalg.norm(y_poly, ord=1)
    assert(len(y) == len(y_poly))
    kl = entropy(y_poly, y, base=2)
    print("KL Divergence: {}".format(kl))
    return costFreqTable - bitsSacrificed

def main():
    if len(sys.argv) != 4:
        print("Usage: ./evaluate_huffman.py <pickle SNP pos file> <sample # (-1 for all samples)> <polynomial degree>")
        sys.exit(1)

    diffs = serializeVCFSNP(sys.argv[1], int(sys.argv[2]))

    if int(sys.argv[2]) >= 0:
        process("Sample #{}".format(sys.argv[2]), diffs)
    else:
        bitsSavedList = []
        for sample, sampleDiffs in diffs.items():
            bitsSaved = process(sample, sampleDiffs)
            bitsSavedList.append(bitsSaved)
        print("Average bits saved: {}".format(np.average(bitsSavedList)))
main()
