#!/usr/bin/env python3

from Bio import SeqIO,Seq,SeqRecord
from Bio.Alphabet import DNAAlphabet
import sys

from common import serializeDiffs

def readSequence(name):
    seqs = SeqIO.parse(name, 'fasta')
    seq = ""
    for i, record in enumerate(seqs):
        assert(i < 1) # make suree there's only one sequence per file
        seq = record.seq
    return seq

def applyDiffOn(seq, diffs):
    newSeqList = []
    nextPosToPush = 0
    for diff in diffs:
        deltaType = diff[0]
        posOfDelta=int(diff[1])
        contentOfDelta = diff[2]
        if deltaType == 0:
            newSeqList.append(str(seq[nextPosToPush:posOfDelta]))
            newSeqList.append(contentOfDelta) # the new SNP
            nextPosToPush = posOfDelta + 1
        elif deltaType == 2: # insertion
            newSeqList.append(str(seq[nextPosToPush:posOfDelta + 1]))
            newSeqList.append(contentOfDelta) # the new sequence
            nextPosToPush = posOfDelta + 1
        else: # deletion
            assert(deltaType == 1)
            newSeqList.append(str(seq[nextPosToPush:posOfDelta]))
            assert(diff[3] == seq[posOfDelta: posOfDelta + contentOfDelta])# make sure we are deleting the same thing
            nextPosToPush = posOfDelta + contentOfDelta
    newSeqList.append(str(seq[nextPosToPush:]))
    newSeq = "".join(newSeqList)
    print("Total {} deltas applied.".format(len(diffs)))
    return newSeq

def output(seq, outputDirectory, chrName):
    with open('{}/{}.txt'.format(outputDirectory, chrName), "w") as f:
        f.write(seq)

def run(chrName, refDirectory, deltaFile, outputDirectory):
    # seq = readSequence('chrY-small.fa')
    seq = readSequence('{}/{}.fa'.format(refDirectory, chrName))
    print("Read reference sequence of length: {}".format(len(seq)))
    diffs = serializeDiffs(deltaFile, chrName)
    # diffs = serializeDiffs("smalldel.txt")
    print("Found {} deltas".format(len(diffs)))
    newSeq = applyDiffOn(seq, diffs)
    diffLen = len(newSeq) - len(seq)
    print("Constructed new sequence length: {}({:.3f}%)".format(len(newSeq), 100 * diffLen / len(seq)))
    newSeqWithoutN = newSeq.replace("N", "")
    newSeqWithoutN = newSeqWithoutN.replace("\n", "")
    print("Removed {} N's (unknown bases) and spaces".format(len(newSeq) - len(newSeqWithoutN)))
    output(newSeqWithoutN, outputDirectory, chrName)

def main():
    if len(sys.argv) != 4:
        print("Usage: ./apply_diff.py <chrX ref directory> <delta file> <output directory>")
        sys.exit()
    chrs = [str(i) for i in range(1, 23)]
    chrs += ["X", "Y", "M"]
    for chrNum in chrs:
        name = "chr{}".format(chrNum)
        print("==BEGIN {}===".format(name))
        run(name, sys.argv[1], sys.argv[2], sys.argv[3])
        print("==END {}===".format(name))
        print("")
main()
