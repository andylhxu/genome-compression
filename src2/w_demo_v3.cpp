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
#include <string.h>
#include <time.h>

#include <math.h>
#include <vector>
#include <set>

#include "ChromNames.h"
#include "DifferenceMap.h"
#include "ConvertJWBSubmissionToShorterForm.h"
#include "Compressor.h"

using namespace std;

ChromNames myChromNames; // declare global chromosome names

void ConvertJWBSubmissionToShortForm(char *infileSNPs, char *infileInDels, char *outfile);

void Pack(char *comp_dir, char *compressed_name); // packs files in compressed directory
void Unpack(char *comp_dir, char *compressed_name); // unpacks files in compressed directory
void CumSum(vector<int> &cumsum, const vector<int> &in); // computes cumulative sum of vector in, starts from 0, assumes cumsum and in of same size
string IntToStr(int); // converts int to string

int main(int argc, char **argv)
{
	////////////////////////////////////////////////////////////////
	// hard-coded variables
	char *JWB_SNPs_submission_filename = "files//JWB-snps-submission.txt";
	// char *JWB_SNPs_submission_filename = argv[1];
	char *JWB_indels_submission_filename = "files//JWB-indels-submission";
	// char *JWB_indels_submission_filename = argv[2];
	char *sourceGenome_filename = "Watson_short_form.txt"; // Watson genome in short form
	// char *sourceGenome_filename = argv[3]; // Watson genome in short form
	
	char *dbSNP_dirname = "dbSNP/"; // dbSNP directory
	bool usedbSNP = true;
	if (argc > 1)
		if (strcmp(argv[1],"-nodbsnp")==0) // don't use dbSNP
		{
			usedbSNP = false;
			dbSNP_dirname = "dbSNP_fake/";
			cout << "not using dbSNP" << endl;
		}
	
	cout << "using dbSNP: " << usedbSNP << endl;
	
	char *refgenome_dirname = "chr/"; // reference genome directory
		
	char *comp_dir = "comp/"; // compressed directory
	
	char *compressed_name = "Watson_short_form_compressed";
	char *uncompressed_name = "Watson_short_form_uncompressed.txt";
	
	// make compressed directory
	string systermstr_mkdir("mkdir "); systermstr_mkdir += string(comp_dir) + "/";
	system(systermstr_mkdir.c_str());
	
	// timer variables
	time_t tic;
	time_t toc;

	////////////////////////////////////////////////////////////////
	// convert Watson submission from long form to short form
	// and write short form to file (uncompressed file)
	cout << "parsing Watson map to shorter form..." << endl;
	time(&tic);
	// check if file exists
	ifstream test(sourceGenome_filename);
	if (!test.is_open()) // if did not already convert to short form
		ConvertJWBSubmissionToShortForm(JWB_SNPs_submission_filename,JWB_indels_submission_filename,sourceGenome_filename);
	else
	{	
		cout << "parsing already done" << endl;
		test.close();
	}
	time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	
	////////////////////////////////////////////////////////////////
	// load Watson from short form
	time(&tic);
	cout << "loading Watson map from shorter form..." << endl;
	time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	
	////////////////////////////////////////////////////////////////
	// compress
	// DifferenceMap myDifferenceMap_all_chrom;
	Compressor myCompressor(usedbSNP);
	
	NondistanceData myNondistanceData; // non-distance data
	
	for (int chromNum = 0; chromNum < myChromNames.GetNumChromosomes(); ++chromNum)
	{
		string currentChromName = myChromNames.IndexToChromName(chromNum);
		// grep part of genome corresponding to current chromosome
		string systemstr_grep("grep ,");
		systemstr_grep += currentChromName;
		systemstr_grep += ", ";
		systemstr_grep += sourceGenome_filename;
		systemstr_grep += " > ";
		systemstr_grep += sourceGenome_filename;
		systemstr_grep += ".";
		systemstr_grep += currentChromName;
		
		cout << "system string: " << systemstr_grep << endl;
		system(systemstr_grep.c_str());
		
		// load Watson from short form
		time(&tic);
		cout << "loading Watson map from shorter form..." << endl;
		string sourceGenome_filename_current_chrom(sourceGenome_filename);
		sourceGenome_filename_current_chrom += string(".") + currentChromName;
		VarMapWatson myVarMapWatson(sourceGenome_filename_current_chrom.c_str());
		myVarMapWatson.varMap.size();
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		// remove temp file
		string systemstr_rm("rm "); systemstr_rm += sourceGenome_filename_current_chrom;
		system(systemstr_rm.c_str());
		
		//read dbSNP
		time(&tic);
		cout << "reading dbSNP..." << endl;
		VarMapdbSNP myVarMapdbSNP(dbSNP_dirname,myChromNames.IndexToChromName(chromNum),usedbSNP); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		//make difference map
		time(&tic);
		cout << "making difference map..." << endl;
		DifferenceMap myDifferenceMap_current_chrom(myVarMapWatson,myVarMapdbSNP,myChromNames.IndexToChromName(chromNum));
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		// compress distances
		myCompressor.CompressDistances(myDifferenceMap_current_chrom,comp_dir,myChromNames.IndexToChromName(chromNum),myNondistanceData);
	}
	
	// compress non-distances
	myCompressor.CompressNondistances(myNondistanceData,comp_dir);
	
	// pack into single file
	Pack(comp_dir, compressed_name);

	//////////////////////////////////////////////////
	// End compression, begin uncompression, check if original, uncompressed files match
	//////////////////////////////////////////////////

	// unpack from single file
	Unpack(comp_dir, compressed_name);

	// uncompress
	DistanceData myDistanceData_uncompressed; // distance data
	for (int chromNum = 0; chromNum < myChromNames.GetNumChromosomes(); ++chromNum)
	{
		time(&tic);
		cout << "uncompressing distances..." << endl;
		myCompressor.UncompressDistances(comp_dir,myChromNames.IndexToChromName(chromNum),myDistanceData_uncompressed,usedbSNP);
		time(&toc); cout << "time elapsed uncompressing (s): " << difftime(toc,tic) << endl;
	}
	
	NondistanceData myNondistanceData_uncompressed; // non-distance data
	
	time(&tic);
	cout << "uncompressing non-distances..." << endl;
	myCompressor.UncompressNondistances(comp_dir,myNondistanceData_uncompressed, usedbSNP);
	time(&toc); cout << "time elapsed uncompressing (s): " << difftime(toc,tic) << endl;
	
	DifferenceMap uncompressedDifferenceMap;
	
	myCompressor.UncompressUpdateDiffMap_p1(refgenome_dirname,uncompressedDifferenceMap,myDistanceData_uncompressed,myNondistanceData_uncompressed, usedbSNP);
	
	
	for (int chromNum = 0; chromNum < myChromNames.GetNumChromosomes(); ++chromNum)
	{
		string currentChromName = myChromNames.IndexToChromName(chromNum);
	
		//read dbSNP
		time(&tic);
		cout << "reading dbSNP..." << endl;
		VarMapdbSNP myVarMapdbSNP(dbSNP_dirname,myChromNames.IndexToChromName(chromNum),usedbSNP); cout << "done" << endl;
		// VarMapdbSNP myVarMapdbSNP(dbSNP_dirname); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		myCompressor.UncompressUpdateDiffMap_p2(refgenome_dirname,myVarMapdbSNP,uncompressedDifferenceMap,currentChromName,myDistanceData_uncompressed,myNondistanceData_uncompressed);
	}
	
	myCompressor.UncompressUpdateDiffMap_p3(refgenome_dirname,uncompressedDifferenceMap,myDistanceData_uncompressed,myNondistanceData_uncompressed);
	
	SumMap mySumMapUnc;	
	for (int chromNum = 0; chromNum < myChromNames.GetNumChromosomes(); ++chromNum)
	{
		string currentChromName = myChromNames.IndexToChromName(chromNum);
	
		//read dbSNP
		time(&tic);
		cout << "reading dbSNP..." << endl;
		VarMapdbSNP myVarMapdbSNP(dbSNP_dirname,myChromNames.IndexToChromName(chromNum),usedbSNP); cout << "done" << endl;
		// VarMapdbSNP myVarMapdbSNP(dbSNP_dirname); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
		
		
		//reconstruct 
		time(&tic);
		cout << "making sum map...";
		mySumMapUnc.Update(uncompressedDifferenceMap,myVarMapdbSNP,currentChromName); cout << "done" << endl;
		time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	}
	
	//write sum map to file
	time(&tic);
	cout << "writing sum map to file...";
	// string destUncompressedfilename_string("uncomp");
	mySumMapUnc.WriteVarMapToFile(uncompressed_name); cout << "done" << endl;
	time(&toc); cout << "time elapsed (s): " << difftime(toc,tic) << endl;
	
	// remove compressed directory
	string systemstr_rm("rm -rf "); systemstr_rm += comp_dir;
	// system(systemstr_rm.c_str());	
	
	return 0;
}

void Pack(char *comp_dir, char *compressed_name) // packs files in compressed directory
{
	vector< pair<string,string> > keys;
	keys.push_back(make_pair("chr*","cat_c")); // distances files
	keys.push_back(make_pair("*from*","cat_s")); // SNP substitution values files
	keys.push_back(make_pair("*txt*","cat_t")); // txt files
	
	string sysstr;
	
	// remove .temp files
	// cout << "about to remove .temp files..." << endl;
	sysstr = "rm "; sysstr += string(comp_dir) + string("/*.temp");
	cout << ": " << sysstr << endl;
	system(sysstr.c_str());
	// cout << "removed .temp files" << endl; cin >> z1;
	
	/* begin commented out, doesn't work on Ubuntu, DSP 8/24/15
	for (vector< pair<string,string> >::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
		sysstr = "ls "; sysstr += string(comp_dir) + string("/") + (it->first) + " -l | awk '{n = split($0,a,\" \"); print a[5];}' > ";
		sysstr += string(comp_dir) + string("/") + (it->second) + "_sizes"; // record file sizes in bytes
		system(sysstr.c_str()); 
		cout << ": " << sysstr << endl;
		sysstr = "ls "; sysstr += string(comp_dir) + string("/") + (it->first) + " > ";
		sysstr += string(comp_dir) + string("/") + (it->second) + string("_names && gzip ") 
			+ string(comp_dir) + string("/") + (it->second) + "_names --best"; // record file names
		system(sysstr.c_str()); 
		cout << ": " << sysstr << endl;
		sysstr = "cat "; sysstr += string(comp_dir) + string("/") + (it->first) + " > ";
		sysstr += string(comp_dir) + string("/") + (it->second) + "_files"; // concatenate files
		system(sysstr.c_str()); 
		cout << ": " << sysstr << endl;
		sysstr = "rm "; sysstr += string(comp_dir) + string("/") + (it->first); // remove files
		system(sysstr.c_str()); 
		cout << ": " << sysstr << endl;
	}
	end commented out, doesn't work on Ubuntu, DSP 8/24/15 */
	
	// pack all, remove all unpacked
	sysstr = string("tar -czf ") + string(compressed_name) + string(" ") + string(comp_dir); // tar, compress all together
	system(sysstr.c_str()); 
	cout << ": " << sysstr << endl;
	sysstr = "rm "; sysstr += string(comp_dir) + " -rf"; // remove compressed directory
	system(sysstr.c_str()); 
	cout << ": " << sysstr << endl;
}

void Unpack(char *comp_dir, char *compressed_name) // unpacks files in compressed directory
{
	cout << "unpacking..." << endl;
	vector< pair<string,string> > keys;
	keys.push_back(make_pair("chr*","cat_c")); // distances files
	keys.push_back(make_pair("*from*","cat_s")); // SNP substitution values files
	keys.push_back(make_pair("*txt*","cat_t")); // txt files

	// unpack all
	string sysstr = string("tar -xf ") + string(compressed_name);
	system(sysstr.c_str()); 
	cout << ": " << sysstr << endl;

	/* begin commented out, doesn't work on Ubuntu, DSP 8/24/15
	for (vector< pair<string,string> >::const_iterator it = keys.begin(); it != keys.end(); ++it)
	{
		vector<int> fileSizes; vector<int> cumsum(fileSizes);
		vector<string> fileNames;
		
		string fileSizesName(comp_dir); fileSizesName += string("/") + (it->second) + "_sizes";
		// cout << "ints name: " << fileSizesName << endl;
		FileToInts(fileSizes,fileSizesName.c_str()); // file sizes
		// CumSum(cumsum,fileSizes);
		int sum = 0;
		
		for (unsigned int i = 0; i < cumsum.size(); ++i)
			cout << i << ": " << fileSizes[i] << '\t' << cumsum[i] << endl;
		// cout << "HERE2" << endl;
		
		string fileNamesName(comp_dir); fileNamesName += string("/") + (it->second) + "_names";
		
		sysstr = "gunzip "; sysstr += fileNamesName + string(".gz");
		system(sysstr.c_str());
		cout << ": " << sysstr << endl;
		
		// cout << "file name: " << fileNamesName << endl;
		FileToStrings(fileNames,fileNamesName.c_str()); // file names
		
		// cout << "SIZE::::: " << fileSizes.size() << endl;
		
		// split files
		for (unsigned int i = 0; i < fileSizes.size(); ++i)
		{		
			sum += fileSizes[i];
			sysstr = "head "; sysstr += string(comp_dir) + (it->second) + string("_files") + string(" -c ") + IntToStr(sum) + string(" | tail -c ") + IntToStr(fileSizes[i]) 
				+ string(" > ") + fileNames[i];
			system(sysstr.c_str());
			cout << ": " << sysstr << endl;
		}
	}
	end commented out, doesn't work on Ubuntu, DSP 8/24/15 */
}

void CumSum(vector<int> &cumsum, const vector<int> &in) // computes cumulative sum of vector in, starts from 0, assumes cumsum and in of same size
{
	int sum = 0;
	for (unsigned int i = 0; i < in.size(); ++i)
	{
		sum += in[i];
		cumsum[i] = sum;
		cout << i << ": " << in[i] << '\t' << cumsum[i] << endl;
	}
}

string IntToStr(int x) // converts int to string
{
	ostringstream numstr; //holds position
	numstr.str("");
	
	numstr << x;
	
	return numstr.str();
}
