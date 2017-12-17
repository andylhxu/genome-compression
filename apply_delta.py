#!/usr/bin/env python3

from Bio import SeqIO,Seq,SeqRecord
from Bio.Alphabet import DNAAlphabet
import sys

def readSequence(name):
    seqs = SeqIO.parse(name, 'fasta')
    seq = ""
    for i, record in enumerate(seqs):
        assert(i < 1) # make suree there's only one sequence per file
        seq = record.seq
    return seq

def serializeDiffs(diffFileName, chrName):
    diffs = []
    lastDelPos = 0
    lastDelLength = 0
    with open(diffFileName, "r") as f:
        for line in f:
            delta = line.split(',')
            posOfDelta = int(delta[2])
            if delta[1] != chrName:
                continue
            if delta[0] == "0": #SNP
                diffs.append((0, posOfDelta - 1, delta[3][2]))
            elif delta[0] == "1": # deletion
                lengthToRemove = len(delta[3].split("/")[0])
                contentToRemove = delta[3].split("/")[0]
                if (posOfDelta - 1 - lastDelPos < lastDelLength):
                    continue
                diffs.append((1, posOfDelta - 1, lengthToRemove, contentToRemove))
                lastDelPos = posOfDelta - 1
                lastDelLength = lengthToRemove
            else: # insertion
                assert(delta[0] == "2")
                stringToInsert = delta[3].split("/")[1]
                diffs.append((2, posOfDelta - 1, stringToInsert))
    diffs.sort(key=lambda x:x[1])
    return diffs

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

def run(chrName, refDirectory, deltaFile):
    # seq = readSequence('chrY-small.fa')
    seq = readSequence('{}/{}.fa'.format(refDirectory, chrName))
    print("Read reference sequence of length: {}".format(len(seq)))
    diffs = serializeDiffs(deltaFile, chrName)
    # diffs = serializeDiffs("smalldel.txt")
    print("Found {} deltas".format(len(diffs)))
    newSeq = applyDiffOn(seq, diffs)
    print("Constructed new sequence length: {}".format(len(newSeq)))
    print("Diff between new and reference: {}".format(len(newSeq) - len(seq)))


def main():
    if len(sys.argv) != 3:
        print("Usage: ./apply_diff.py <chrX ref directory> <delta file>")
        sys.exit()
    chrs = [str(i) for i in range(1, 23)]
    chrs += ["X", "Y", "M"]
    for chrNum in chrs:
        name = "chr{}".format(chrNum)
        print("==BEGIN {}===".format(name))
        run(name, sys.argv[1], sys.argv[2])
        print("==END {}===".format(name))
        print("")
main()
