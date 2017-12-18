# Analysis Tools on Genome SNP Distance Distribution

## apply_delta.py
Takes in the reference genome directory that contains chromosome sequences in FASTA(*.fa) format, and the variant indel/SNP submission file, reconstruct the complete variant genome.

Human Reference Genome in FASTA format: http://www.ics.uci.edu/~dnazip/chr.tgz

James Watson's Variant indel/SNP submission file (JWB-unified-file.txt) can be generated using DNAZip from: http://www.ics.uci.edu/~dnazip/JWB.tgz

## vcfparser.py
This script is used to generate a list of SNP positions of multiple (Homo sapiens) samples into a python direction, which is then extracted into a pickle file to be analyzed later.

It takes in a variant indel/SNP submission file in VCF format (download link see below) and a number specifying how many samples to extract.

Variant Calls from 1000 Genomes project (chromosome 22):
http://ftp.1000genomes.ebi.ac.uk/vol1/ftp/release/20130502/ALL.chr22.phase3_shapeit2_mvncall_integrated_v5a.20130502.genotypes.vcf.gz

## graph_distance.py
This script attempts to fit and graph the (logarithmized) SNP distance distribution using a polynomial.

It takes in a pickle file generated from vcfparser.py, a sample # (depends on how many samples are extracted by vcfparser) and the degree of the polynomial to fit with. Notice that the graph normalizes the curve by taking base-10 log of both x and y axes.

## evaluate_huffman.py

