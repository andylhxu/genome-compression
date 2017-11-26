//diff map compressor class declaration

#ifndef __Compressor__
#define __Compressor__

#include "DifferenceMap.h"
#include "Hist.h"
#include "PMF_DPL_Compressor.h"

#include "ArithmeticCodingWrapper.h"

// #include<bitset>

struct NondistanceData // data other than distances between novel variations, in dbSNP
{
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
};

struct DistanceData // distances between novel variations, in dbSNP
{
	vector<int> novelDistances; // vector of distances
	vector<int> dbSNPDistances; // vector of distances between indices in dbSNP
};
	
class Compressor
{
private:
	ChromNames myChromNames; //chromosome names
	
	vector<double> novelDistances_PMF_DPL_parameters; //parameters for pmf for double power law fit for novel distances
	vector<double> dbSNPDistances_PMF_DPL_parameters; //parameters for pmf for double power law fit for distance between indices in dbSNP
	
	// map<int,double> novelDistanceFrequencies; //frequencies for novel distances from pmf for double power law
	// map<int,double> dbSNPDistanceFrequencies; //frequencies for distance between indices in dbSNP from pmf for double power law
	
	////////////////////////////////////////////////////////////////////////////////////////
	// Compression
	//gets offsets for all entries in var map
	void GetNovelDistances(
		const VarMap & varMap, vector<int> &novelDistances, string &novelLabels, vector<int> &chromLengths,
		vector<int> &deletionLengths, vector<int> &insertionLengths, string &insertionSequences); 
	//gets offsets for dbSNP matches, partial matches, gets match, partial match string, 1 means partial match, 0 match
	void GetdbSNPDistancesAndPartialMatchVector(
		const DifferenceMap & diffMap, vector<int> &dbSNPDistances, string &dbSNPMatchString, vector<int> &chromLengths);
	//gets SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	void GetSNPSubstitutionValues(
		const DifferenceMap &diffMap, map<char,string> &SNPSubstitutionStrings, map<char,string> &SNPSubstitutionStringsPartialMatches);
		
	////////////////////////////////////////////////////////////////////////////////////////	
	// Uncompression
	//puts offsets for all entries into var map
	void PutNovelDistances(
		VarMap & varMap, const vector<int> &novelDistances, const string &novelLabels, const vector<int> &chromLengths,
		const vector<int> &deletionLengths, const vector<int> &insertionLengths, const string &insertionSequences); 
	//puts offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	void PutdbSNPDistancesAndPartialMatchVector(
		DifferenceMap & diffMap, const vector<int> &dbSNPDistances, const string &dbSNPMatchString, const vector<int> &chromLengths, bool usedbSNP);
	//puts reference sequence values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	void PutReferenceSequences(
		DifferenceMap & diffMap, const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap);
	void PutReferenceSequences(
		DifferenceMap & diffMap, const char *refGenomeDirectory, const string chromName, const VarMapdbSNP &dbSNPMap); // puts for single chromosome only
	//puts SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	//assumes reference values loaded into difference map diffMap
	void PutSNPSubstitutionValues(
		DifferenceMap &diffMap, const map<char,string> &SNPSubstitutionStrings, const map<char,string> &SNPSubstitutionStringsPartialMatches);
		
	////////////////////////////////////////////////////////////////////////////////////////
	void WriteIntsToFile(const vector<int> &novelDistances, const char *filename); //writes distances to file
	void WriteStringToFile(const string &s, const char *filename); //write string to file
	
	//writes frequency map to file, writes two files, one for frequencies, one for unique values
	void WriteIntIntFreqMapToFile(const map<int,int> &freqMap, const char *filename);
	//reads frequency map from file, reads from two files, one for frequencies, one for unique values
	void ReadIntIntFreqMapFromFile(map<int,int> &freqMap, const char *filename);
	
	char Capitalize(const char &charToCapitalize); // capitalizes character
	string Capitalize(const string &stringToCapitalize); // capitalizes string
public:
	Compressor(bool usedbSNP); //constructor
	~Compressor(); //destructor
	
	// compresses distances only
	void CompressDistances(const DifferenceMap & in_diffMap, const char *outdirectory, const string chromName, NondistanceData & nondistanceData_);
	// compresses all objects other than distances
	void CompressNondistances(NondistanceData & nondistanceData_, const char *outdirectory);
	
	// uncompress distances only
	void UncompressDistances(const char *indirectory, const string chromName, DistanceData & distanceData_, bool usedbSNP);
	// uncompresses all objects other than distances
	void UncompressNondistances(const char *indirectory, NondistanceData & nondistanceData_, bool usedbSNP);
	void UncompressUpdateDiffMap_p1(const char *refGenomeDirectory, DifferenceMap &out_diffMap,	
		DistanceData & distanceData_, NondistanceData & nondistanceData_, bool usedbSNP);
	void UncompressUpdateDiffMap_p2(const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, const string chromName,
		DistanceData & distanceData_, NondistanceData & nondistanceData_);
	void UncompressUpdateDiffMap_p3(const char *refGenomeDirectory, DifferenceMap &out_diffMap,	
		DistanceData & distanceData_, NondistanceData & nondistanceData_);
	
	// updates difference map, combines distances data and non-distances data
	void UncompressUpdateDiffMap(const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, const string chromName,
		DistanceData & distanceData_, NondistanceData & nondistanceData_, bool usedbSNP);
	
	//compresses difference map in_diffMap, writes files to directory outdirectory
	void Compress(const DifferenceMap & in_diffMap, const char *outdirectory);
	void Compress(const DifferenceMap & in_diffMap, const char *outdirectory, const string chromName);
	//uncompresses difference map out_diffMap from files in directory indirectory, reference genome in directory refGenomeDirectory
	void Uncompress(const char *indirectory, const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, bool usedbSNP); 
	void Uncompress_by_chrom(const char *indirectory, const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, const string chromName, bool usedbSNP); 
	// void Uncompress_by_chrom(const char *indirectory, const char *refGenomeDirectory, DifferenceMap &out_diffMap); // uncompresses single chromosome only
	// void Uncompress_by_chrom_dbSNP(const VarMapdbSNP &dbSNPMap, const string chromName);
};

#endif
