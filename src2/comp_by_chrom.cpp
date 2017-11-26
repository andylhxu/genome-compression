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
void MakeDestCompressedDir(char *destCompressed_dirname);

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
	
	// char *sourceGenome_filename = argv[1];
	// string chromName(argv[2]); // "all" means read all chromosomes
	// char *destCompressed_dirname = argv[3];
	// char *destUncompressed_dirname= argv[4];
	// char *refgenome_dirname = argv[5];
	// char *dbSNP_dirname = argv[6];
	// char *writeUncompressed = argv[7];
	
	// string destUncompressedfilename_string = (string(destUncompressed_dirname) + string("/uncomp.txt"));
	// const char *destUncompressedfilename = destUncompressedfilename_string.c_str();
	// cout << "uncompressed file name:_____ " << destUncompressedfilename << endl;
	
	// source genome name
	char *sourceGenome_filename = "Watson_short_form.txt";
	
	// make destination compressed directory
	char *destCompressed_dirname = "comp/";
	// MakeDestCompressedDir(destCompressed_dirname);
	
	// reference genome directory
	char *refgenome_dirname = "/home/MainUser/genome_compress/code1/chr";	
	// dbSNP directory
	char *dbSNP_dirname = "/home/MainUser/genome_compress/code1/dbSNP";
	
	////////////////////////////////////////////////////////////////
	time_t tic;
	time_t toc;

	////////////
	// convert Watson submission to shortened form
	cout << "converting map..." << endl;
	time(&tic);
	// ConvertJWBSubmissionToShortForm("..//code1//files//JWB-snps-submission.txt","..//code1//files//JWB-indels-submission","destJWB.txt");
	time(&toc);
	cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	
	int zzz;
	cout << "____";
	cin >> zzz;
	
	for (int chromNum = 0; chromNum < myChromNames.GetNumChromosomes(); ++chromNum)
	{
		//read dbSNP
		time(&tic);
		cout << "reading dbSNP..." << endl;
		VarMapdbSNP myVarMapdbSNP(dbSNP_dirname,chromName); cout << "done" << endl;
		// VarMapdbSNP myVarMapdbSNP(dbSNP_dirname); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		// write Watson map to file
		time(&tic);
		cout << "writing Watson map to file..." << endl;
		// myVarMapWatson.WriteVarMapToFile("files/destJWB.txt"); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		//make difference map
		time(&tic);
		cout << "making difference map..." << endl;
		DifferenceMap myDifferenceMap(myVarMapWatson,myVarMapdbSNP); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		//clear Watson map
		myVarMapWatson.Clear();	
		myVarMapWatson.ClearWatson();
		
		/*
		//reconstruct 
		time(&tic);
		cout << "making sum map..." << endl;
		// SumMap mySumMap(myDifferenceMap,myVarMapdbSNP); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		//write sum map to file
		time(&tic);
		cout << "writing sum map to file..." << endl;
		// mySumMap.WriteVarMapToFile("uncompressed/dest_sum_map.txt"); cout << "done" << endl;
		// mySumMap.WriteVarMapToFile("files/destdestJWB.txt"); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		*/
		
		//compress
		time(&tic);
		cout << "compressing..." << endl;
		Compressor myCompressor;	
		myCompressor.Compress(myDifferenceMap,destCompressed_dirname);
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	}
	/*
	//clear difference map
	cout << "clearing difference map...";
	myDifferenceMap.Clear();
	cout << "done" << endl;
	
	if (writeUncompressedbool)
	{
		//uncompress
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
	}
	// }
	*/
	
	//compare initial, uncompressed files
	
	return 0;
}

// makes destination compressed directory
void MakeDestCompressedDir(char *destCompressed_dirname)
{
	// remove directory if previously existed
	string sys_string("rm -rf ");
	sys_string += destCompressed_dirname;
	system(sys_string.c_str()); // evaluate system command
	
	// create directory
	sys_string = "mkdir ";
	sys_string += string(destCompressed_dirname);
	system(sys_string.c_str()); // evaluate system command
	
	myChromNames;
	for (int i = 0; i < myChromNames.GetNumChromosomes(); ++i)
	{
		sys_string = ("mkdir ");
		sys_string += (string(destCompressed_dirname) + "/");
		sys_string += myChromNames.IndexToChromName(i);
		
		system(sys_string.c_str()); // evaluate system command
	}
}
