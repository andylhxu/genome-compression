import vcf
import pickle
import sys

HUMAN_COUNT = 3
NAME = "chr22-phase3"

def construct_deltas(vcfFileName):
    vcf_reader = vcf.Reader(open(vcfFileName, 'r'))

    samples = []
    samples_delta = {}
    for i,human in enumerate(vcf_reader.samples):
        if i >= HUMAN_COUNT:
            break
        samples.append(human)
        samples_delta[human] = []

    for i,record in enumerate(vcf_reader):
        if i % 100 == 0:
            print(i)
        if i % 1000 == 0 and i > 1:
            dumpFileName = "source{}-samples{}-scanned{}.pkl".format(NAME, HUMAN_COUNT, i)
            f = open(dumpFileName, "wb")
            pickle.dump(samples_delta, f)
            f.close()
            print("Pickle {} dumped.".format(dumpFileName))
        recordPosition = record.POS
        if (record.is_snp == False):
            continue
        is_variants = [record.genotype(sample).is_variant for sample in samples]
        for sample in samples:
            if record.genotype(sample).is_variant:
                samples_delta[sample].append(recordPosition)

    return samples_delta

def main():
    if len(sys.argv) != 2:
        print("Usage python3 vcfparser.py <vcf file path>")
    name = './ALL.chr22.phase3_shapeit2_mvncall_integrated_v5a.20130502.genotypes.vcf'
    construct_deltas(name)
main()
