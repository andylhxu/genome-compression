//tries reading, parsing text file

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <time.h>

// #include <iostream>
#include <math.h>
#include <vector>
#include <set>
// #include <fstream>
// #include <string>

#include "DifferenceMap.h"
#include "ConvertJWBSubmissionToShorterForm.h"
#include "Hist.h"
#include "Compressor.h"
#include "BoolFiles.h"
#include "HuffmanWrapper.h" //wrapper for Huffman coding LP implementation
#include "ArithmeticCodingWrapper.h" //wrapper for Arithmetic coding Malte implementation

// #include "ModelOrder0C.h" //Arithmetic coding Malte implementation

//#include <unordered_set.h>
// #include <unordered_set>

using namespace std;

ChromNames myChromNames; // declare global chromosome names

void ConvertJWBSubmissionToShortForm(char *infileSNPs, char *infileInDels, char *outfile);

int main(int argc, char **argv)
{
	////////////////////////////////////////////////////////////////
	
	//arg[1] - source genomes files directory name, ends with '/'
	//arg[2] - source of genome names
	//arg[3] - suffix for compressed folder, ends with '/'
	//arg[4] - suffix for uncompressed folder, ends with '/'
	//arg[5] - reference genome directory name, ends with '/'
	//arg[6] - dbSNP directory name, ends with '/'
	//arg[7] - "1" if should write uncompressed file, else will not write uncompressed file
	
	char *sourceGenome_filename = argv[1];
	string chromName(argv[2]); // "all" means read all chromosomes
	char *destCompressed_dirname = argv[3];
	char *destUncompressed_dirname= argv[4];
	char *refgenome_dirname = argv[5];
	char *dbSNP_dirname = argv[6];
	char *writeUncompressed = argv[7];
	
	string destUncompressedfilename_string = (string(destUncompressed_dirname) + string("/uncomp.txt"));
	const char *destUncompressedfilename = destUncompressedfilename_string.c_str();
	cout << "uncompressed file name:_____ " << destUncompressedfilename << endl;
	
	
	// char *sourceGenomename_dirname = argv[1];
	// char *sourceGenomename_filename = argv[2];
	// char *destCompresseddirname_suffix = argv[3];
	// char *destUncompressedfilename_suffix = argv[4];
	// char *refgenomedirname = argv[5];
	// char *dbSNPdirname = argv[6];
	// char *writeUncompressed = argv[7];
	// sum-product, belief propagation
	////////////////////////////////////////////////////////////////
	// check if should write uncompressed
	bool writeUncompressedbool;
	if (strcmp(writeUncompressed,"1")==0)
		writeUncompressedbool = true;
	else
		writeUncompressedbool = false;
	cout << "writing uncompressed: " << (char)writeUncompressedbool << endl;
	
	// read source genome names
	//open file
	// ifstream source_genomenames(sourceGenomename_filename);	
	// vector<string> genome_names; // genome names
	//read string
	// string line_read; // line read from file
	// while (getline(source_genomenames,line_read))
		// genome_names.push_back(line_read);
	//close file
	// source_genomenames.close();
	
	// read source genome directory names
	//open file
	/*
	ifstream source_genomedirnames(sourceGenomename_dirname);	
	vector<string> genome_dirnames; // genome names
	//read string
	while (getline(source_genomedirnames,line_read))
		genome_dirnames.push_back(line_read);
	//close file
	source_genomedirnames.close();
	*/
	
	////////////////////////////////////////////////////////////////
	time_t tic;
	time_t toc;

	// time_t timer;
	// time(&timer);
	
	time(&tic);
	time(&toc);
	double seconds = difftime(toc,tic);

	cout << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << endl;
	// cout << "seconds: " << ((float)t)/CLOCKS_PER_SEC << endl;
	cout << "seconds: " << seconds << endl;

	////////////
	
	// cout << "pause:";
	// int p;
	// cin >> p;
	
	//read dbSNP
	time(&tic);
	cout << "reading dbSNP..." << endl;
	VarMapdbSNP myVarMapdbSNP(dbSNP_dirname,chromName); cout << "done" << endl;
	// VarMapdbSNP myVarMapdbSNP(dbSNP_dirname); cout << "done" << endl;
	time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		

			//uncompress
			Compressor myCompressor;
			DifferenceMap uncompressedDifferenceMap;
			
			time(&tic);
			cout << "uncompressing..." << endl;
			// myCompressor.Uncompress(destCompressed_dirname,refgenome_dirname,myVarMapdbSNP,uncompressedDifferenceMap);
			myCompressor.Uncompress(destCompressed_dirname,refgenome_dirname,chromName,myVarMapdbSNP,uncompressedDifferenceMap);
			time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
			
			//reconstruct 
			time(&tic);
			cout << "making sum map...";
			SumMap mySumMapUnc(uncompressedDifferenceMap,myVarMapdbSNP); cout << "done" << endl;
			time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
			
			// clear reconstructed difference map, sum map
			cout << "clearing uncompressed difference map...";
			uncompressedDifferenceMap.Clear(); cout << "done" << endl;
			cout << "clearing uncompressed difference map2...";
			uncompressedDifferenceMap.ClearDifferenceMap(); cout << "done" << endl;
			
			//write sum map to file
			time(&tic);
			cout << "writing sum map to file...";
			mySumMapUnc.WriteVarMapToFile2(destUncompressedfilename); cout << "done" << endl;
			time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	// }
	
	//compare initial, uncompressed files
	
	return 0;
}
