# DNAzip: DNA sequence compression using a reference genome

## Intro
The demonstration code can be found here DNAzip.
The code is written in standard C++. We have used GNU C++ to compile in the provided makefile, but other C++ compilers should work. The resulting executable program is "perftest", currently as a demonstration program it is hard-coded to look for specific reference genome, reference SNP map, and genome to be compressed.

## Prep
To run the code, you will need the following two datasets that can be downloded from: http://www.ics.uci.edu/~dnazip/

The human reference genome (hg18, 840MB). This is the exact same data from UCSC goldenPath, you will likely achieve faster download directly from UCSC. There are 25 FASTA files, one for each chromosome and the mitochondrial genome (which typically isn't used), that should be uncompressed and placed in a "chr" subdirectory under the source code.
dbSNP database, 140MB, used as the refernece SNP map. The SNP data is based upon NCBI dbSNP build 129, but we use the UCSC SNP track as it has pre-processed all of the needed data into a single table. The UCSC Table Browser can be used to acquire the SNPs (group: Variation and Repeats, track: SNPs (129), table: snp129). There are 25 FASTA files, one for each chromosome and the mitochondrial genome. These should be in a "dbSNP" subdirectory under the source code.
James Watson's genome, 418M, used as the example genome to be compressed. The data is from the Nature publication and was kindly provided by David Wheeler. There are two files, one which lists all of the indels (JWB-indels-submission) and the other which lists the SNPs (JWB-snps-submission.txt). These should be in a "files" subdirectory under the source code.

## Run
All files generated by the "perftest" executable will be created in the "files" subdiretory. The program compresses the genome then uncompresses it. The following files will be created:
JWB-unified-file.txt: Post-processing of the variation data for JW genome into a more compact text representation; this removes the unneeded fields from the data set. This data is the basis for compression and is ~84MB for JW genome.
JWB-in-posFreq4.txt: The 4-mer frequency table for JW genome.
JWB-unified-compression.txt: This is the resulting compressed genome, it should ~4MB for JW genome.
JWB-unified-DeCompression.txt: This is the resulting decompressed genome, it should be equivalent to the original file (JWB-unified-file.txt).
Future work

## Credits
We have plans to enhance the code into a more flexible genome compression library.
Any questions about use of this code should be directed to Xiaohui Xie or Chen Li

For citation, please refer to the following paper

Human genomes as email attachments, Christley S, Lu Y, Li C, and Xie X, Bioinformatics. 2009 25:274-5. It was the most downloaded article on the Web site of the Journal of Bioinformatics for two months.
Additional info

One of the top 20 papers in translational bioinformatics chosen by Dr. Russ Altman of Stanford in his 2009 The Year in Review.
So Long, Data Depression -- Genome Technology, Sept, 2009
You've Got Email -- A Human Genome, Bio-IT World
Funding

Development of DNAzip is partially supported by funding from National Science Foundaton.
