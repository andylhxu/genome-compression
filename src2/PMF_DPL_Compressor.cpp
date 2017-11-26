//implementation for pmf double power law compressor class

#include "PMF_DPL_Compressor.h"

PMF_DPL_Compressor::PMF_DPL_Compressor() // constructor
{
}

PMF_DPL_Compressor::~PMF_DPL_Compressor() // destructor
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PMF_DPL_Compressor::SetParameters(const vector<double> &parameters) //sets parameters for pmf for double power law fit for distances
{
	this->PMF_DPL_parameters = parameters;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//evaluate double power law pmf at value val
double PMF_DPL_Compressor::DPL_evaluate(int val)
{
	// vector<double> &b = PMF_DPL_parameters;
	return pow(10,PMF_DPL_parameters[0]-
		PMF_DPL_parameters[4]*log10(
		pow(val,PMF_DPL_parameters[1]/PMF_DPL_parameters[4]) + 
		pow(val/pow(10,PMF_DPL_parameters[2]),PMF_DPL_parameters[3]/PMF_DPL_parameters[4])));
	
	// return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//compresses integer vector valuesToCompress, writes to file filename
	//values outside the range [minValue,maxValue] are stored to file exceptions filename
void PMF_DPL_Compressor::Compress(const vector<int> &valuesToCompress, const int minValue, const int maxValue, 
		 const char *filename)
{
	//check maxValue >= minValue

	//prepare frequency table
	map<int,double> frequenciesMap;
	
	vector<double> frequenciesVec;
	frequenciesVec.resize(maxValue-minValue+1);
	//evaluate frequencies
	for (int val = minValue ; val <= maxValue ; ++val)
		frequenciesMap.insert(make_pair(val,DPL_evaluate(val)));
	
	//insert exception case
	int exceptionValue = maxValue+1; //exception value
	frequenciesMap.insert(make_pair(exceptionValue,DPL_evaluate(exceptionValue)));
	
	int i = 0;
	for (map<int,double>::iterator it = frequenciesMap.begin() ; i < 10 ; ++it)
	{
		cout << "(" << it->first << "," << it->second << ")" << endl;
		++i;
	}
	
	cout << "getting exceptions...";
	//get exceptions
	//replace all values outside [minValue,maxValue] range by value exceptionValue
	vector<int> valuesToCompress_labeledExceptions = valuesToCompress;
	vector<int> exceptionValues; //stores exception values
	int maxExceptionValue = 0; //max exception value
	int minExceptionValue = 1000000; //min exception value
	int numExceptions = 0; //number of exceptions
	for (vector<int>::iterator it = valuesToCompress_labeledExceptions.begin() ; it != valuesToCompress_labeledExceptions.end() ; ++it)
	{	if ((*it < minValue) || (*it > maxValue)) //replace with exceptionValue
		{	
			exceptionValues.push_back(*it);
			if (*it > maxExceptionValue) 
				maxExceptionValue = *it;
			if (*it < minExceptionValue)
				minExceptionValue = *it;
			*it = exceptionValue;
			++numExceptions;}			
	}
	cout << "done" << endl;
	cout << "num exceptions: " << numExceptions << endl;
	cout << "max exception value: " << maxExceptionValue << endl;
	cout << "min exception value: " << minExceptionValue << endl;
	
	//compress
	HuffmanWrapper<int,double> myHuffmanWrapper;
	string filenamevalues(filename); filenamevalues += ".values";// "values_" + filenamevalues;
	myHuffmanWrapper.Compress(frequenciesMap,valuesToCompress_labeledExceptions,filenamevalues.c_str());
	
	//write exception values
	string filename_exceptions_values(filename); filename_exceptions_values += ".exceptions_values";//"exceptions_values_" + filename_exceptions_values;
	IntsToFile(exceptionValues,filename_exceptions_values.c_str());
	
	//compress exceptions string by arithmetic coding
	// ArithmeticCodingWrapper myArithmeticCodingWrapper;
	// string filename_exceptions_positions
	
	//compress exceptions vector by arithmetic coding
	// ArithmeticCodingWrapper myArithmeticCodingWrapper;
	// myArithmeticCodingWrapper.Compress();
	
	/*
	string filename_encodedDistances(filename);
	filename_encodedDistances = "encodedDistances_t_" + filename_encodedDistances;
	int overhang; //stores file overhang
	BoolToFile(encodedDistances,filename_encodedDistances.c_str(),overhang);
	
	std::cout << "overhang: " << overhang << std::endl;
	
	vector<bool> vec_in;
	FileToBool(filename_encodedDistances.c_str(),overhang,vec_in);*/
	
	// bool done = false;
	// int ii = 0;
	// while (!(done))
	// {
		// if (encodedDistances[ii] != vec_in[ii])
		// {	cout << "mistmatch at " << ii << endl;	
			// for (int j = ii-8 ; j < ii + 20 ; j++)
				// std::cout << encodedDistances[j] << vec_in[j] << "   " << j << endl;
			// done = true;
		// }
		// ++ii;
	// }
	
	// for (int i = 0; i < 669 ; ++i)
	// {
		
	// }
	// encodedDistances.resize(vec_in.size());
	// vec_in.resize(encodedDistances.size());
	
	// cout << "vec size: " << vec_in.size() << endl;
	// cout << "equal: " << (encodedDistances==vec_in) << endl;
	
	//use Huffman wrapper
		// frequenciesVec.push_back(DPL_evaluate(val));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//uncompresses integer vector to integer vector valuesUncompressed from file filename
void PMF_DPL_Compressor::Uncompress(vector<int> &valuesUncompressed, const int minValue, const int maxValue, 
   const char *filename)
{
	//check maxValue >= minValue

	//prepare frequency table
	map<int,double> frequenciesMap;
	
	vector<double> frequenciesVec;
	frequenciesVec.resize(maxValue-minValue+1);
	//evaluate frequencies
	for (int val = minValue ; val <= maxValue ; ++val)
		frequenciesMap.insert(make_pair(val,DPL_evaluate(val)));
	
	//insert exception case
	int exceptionValue = maxValue+1; //exception value
	frequenciesMap.insert(make_pair(exceptionValue,DPL_evaluate(exceptionValue)));
	
	//empty valuesUncompressed
	valuesUncompressed.clear();
	
	//uncompress
	
	cout << "In uncompressor1" << endl;
	
	HuffmanWrapper<int,double> myHuffmanWrapper;
	string filenamevalues(filename); filenamevalues += ".values";// "values_" + filenamevalues;
	myHuffmanWrapper.Uncompress(frequenciesMap,valuesUncompressed,filenamevalues.c_str());
	
	cout << "In uncompressor2" << endl;
	
	//read exception values
	string filename_exceptions_values(filename); filename_exceptions_values += ".exceptions_values";//"exceptions_values_" + filename_exceptions_values;
	vector<int> exceptionValues; //stores exception values
	FileToInts(exceptionValues,filename_exceptions_values.c_str());
	
	cout << "In uncompressor3" << endl;
	
	//replace exceptionValue in uncompressed vector with exception values
	vector<int>::iterator exceptionValues_it = exceptionValues.begin();
	for (vector<int>::iterator it = valuesUncompressed.begin() ; it != valuesUncompressed.end() ; ++it)
	{	
		if (*it == exceptionValue) //exception
		{
			*it = *exceptionValues_it;
			++exceptionValues_it;
		}
	}
	
	cout << "In uncompressor4" << endl;
}
