import pickle

def serializeDiffs(diffFileName, chrName=None):
    diffs = []
    lastDelPos = 0
    lastDelLength = 0
    with open(diffFileName, "r") as f:
        for line in f:
            delta = line.split(',')
            posOfDelta = int(delta[2])
            if chrName != None and delta[1] != chrName:
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

def serializeDiffs2(diffFileName):
    diffs = []
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
            else: # insertion
                assert(delta[0] == "2")
                stringToInsert = delta[3].split("/")[1]
                diffs.append((2, posOfDelta - 1, stringToInsert))
    diffs.sort(key=lambda x:x[1])
    return diffs

def serializeDiffsSNP(diffFileName):
    diffs = []
    with open(diffFileName, "r") as f:
        for line in f:
            delta = line.split(',')
            posOfDelta = int(delta[2])
            if delta[0] == "0": #SNP
                diffs.append((0, posOfDelta - 1, delta[3][2]))
    diffs.sort(key=lambda x:x[1])
    return diffs

def serializeVCFSNP(pickleFileName, humanNum):
    with open(pickleFileName, "rb") as f:
        samplesDiffs = pickle.load(f)
        sampleName = list(samplesDiffs.keys())[humanNum]
        return samplesDiffs[sampleName]

