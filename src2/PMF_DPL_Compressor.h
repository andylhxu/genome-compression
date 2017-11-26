//pmf double power law compressor class

#ifndef __PMF_DPL_Compressor__
#define __PMF_DPL_Compressor__

#include <vector>
#include <map>
#include <math.h>
#include <iostream>

#include "huffman.h"
#include "HuffmanWrapper.h"

using std::vector;
using std::map;
using std::pair;

using namespace std;

class PMF_DPL_Compressor
{
private:
	vector<double> PMF_DPL_parameters; //parameters for pmf for double power law fit for distances
	// vector<double> exp_PMF_DPL_parameters; //pre-exponentiated parameters for pmf for double power law fit for distances
	
	//evaluate double power law pmf at value val
	double DPL_evaluate(int val);
public:
	PMF_DPL_Compressor(); //constructor
	// member initializer syntax, set parameters
	PMF_DPL_Compressor(vector<double> PMF_DPL_parameters) : PMF_DPL_parameters(PMF_DPL_parameters) {}
		// {exp_PMF_DPL_parameters.push_back(pow(10,PMF_DPL_parameters[0])); exp_PMF_DPL_parameters.push_back(pow(10,PMF_DPL_parameters[2]));} 
	~PMF_DPL_Compressor(); //destructor
	
	//sets parameters for pmf for double power law fit for distances
	void SetParameters(const vector<double> &parameters);
	
	//compresses integer vector valuesToCompress, writes to file filename
	//values outside the range [minValue,maxValue] are stored to file exceptions filename
	void Compress(const vector<int> &valuesToCompress, const int minValue, const int maxValue, 
		const char *filename);
	//uncompresses integer vector to integer vector valuesUncompressed from file filename
	void Uncompress(vector<int> &valuesUncompressed, const int minValue, const int maxValue, 
		const char *filename);
};

#endif

/*
exceptions positions file format
#exceptions
exc_pos_1
...
exc_pos_#exceptions
exc_val_1
...
exc_val_#exceptions
*/
