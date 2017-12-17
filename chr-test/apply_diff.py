from Bio import SeqIO,Seq,SeqRecord
from Bio.Alphabet import DNAAlphabet

def readSequence(name):
    seqs = SeqIO.parse(name, 'fasta')
    seq = ""
    for i, record in enumerate(seqs):
        assert(i < 1) # make suree there's only one sequence per file
        print(record)
        seq = record.seq
    return seq

def checkIntegrity(seq, diffFileName, checkNew=False):
    with open (diffFileName, "r") as f:
        for line in f:
            delta = line.split(',')
            if delta[0] == "0":
                posOfDelta=int(delta[2])
                if checkNew:
                    assert(delta[3][2] == seq[posOfDelta-1]) # make sure we are replacing the right thing
                else:
                    assert(delta[3][0] == seq[posOfDelta-1]) # make sure we are replacing the right thing

def serializeDiffs(diffFileName):
    diffs = []
    lastDelPos = 0
    lastDelLength = 0
    with open(diffFileName, "r") as f:
        for line in f:
            delta = line.split(',')
            posOfDelta = int(delta[2])
            if delta[0] == "0": #SNP
                diffs.append((0, posOfDelta - 1, delta[3][2]))
            elif delta[0] == "1": # deletion
                lengthToRemove = len(delta[3].split("/")[0])
                contentToRemove = delta[3].split("/")[0]
                diffs.append((1, posOfDelta - 1, lengthToRemove, contentToRemove))
                lastDelPos = posOfDelta - 1
                lastDelLength = lengthToRemove
            else: # insertion
                assert(delta[0] == "2")
                stringToInsert = delta[3].split("/")[1]
                diffs.append((2, posOfDelta - 1, stringToInsert))
    diffs.sort(key=lambda x:x[1])
    return diffs
def applyDiff(seq, diffs):
    totalInsertion = 0
    totalDeletion = 0
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
            continue
            newSeqList.append(str(seq[nextPosToPush:posOfDelta + 1]))
            newSeqList.append(contentOfDelta) # the new sequence
            nextPosToPush = posOfDelta + 1
            totalInsertion += len(contentOfDelta)
        else: # deletion
            assert(deltaType == 1)
            newSeqList.append(str(seq[nextPosToPush:posOfDelta + 1]))
            nextPosToPush = posOfDelta + contentOfDelta + 1
            # assert(skipped == diff[3])
            totalDeletion += contentOfDelta
    newSeqList.append(str(seq[nextPosToPush:]))
    newSeq = "".join(newSeqList)
    print("inserted: {}, removed: {}".format(totalInsertion, totalDeletion))
    return newSeq

def main():
    # seq = readSequence('chrY-small.fa')
    seq = readSequence('chrY.fa')
    print("Read sequence of length: {}".format(len(seq)))
    # checkIntegrity(seq, "chrY-JWB-unified-file.txt")
    diffs = serializeDiffs("./chrY-JWB-unified-file.txt")
    # diffs = serializeDiffs("smalldel.txt")
    newSeq = applyDiff(seq, diffs)
    print("newSeq length: {}".format(len(newSeq)))
    print("change: {}".format(len(newSeq) - len(seq)))
    # checkIntegrity(newSeq, "chrY-JWB-unified-file.txt", checkNew=True)
main()
