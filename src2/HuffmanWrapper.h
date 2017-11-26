//Huffman LP implementation wrapper class

#ifndef __HuffmanWrapper__
#define __HuffmanWrapper__

// #include <vector>
// #include <map>
#include <iostream>
// #include <string>

#include "huffman.h" //Huffman coding LP implementation
#include "BoolFiles.h"

using std::vector;
using std::map;
using std::string;
using std::cout;
using std::endl;

template <typename inT, typename freqT>
class HuffmanWrapper
{
private:
public:
	HuffmanWrapper() {} //constructor
	~HuffmanWrapper() {} //destructor
	
	//Huffman coding compress values vector valuesToCompress, save to file filename, frequencies table frequenciesMap
	void Compress(const map<inT,freqT> &frequenciesMap, const vector<inT> &valuesToCompress, const char *filename);
	//Huffman coding uncompress values vector from file filename, frequencies table frequenciesMap
	void Uncompress(const map<inT,freqT> &frequenciesMap, vector<inT> &valuesUncompressed, const char *filename);
	
	/*
	//Huffman coding compress values vector valuesToCompress, save to file filename, frequencies table frequenciesMap
	//saves frequency map to file
	void CompressWithFreqMap(const map<inT,freqT> &frequenciesMap, const vector<inT> &valuesToCompress, const char *filename);
	//Huffman coding uncompress values vector from file filename, frequencies table frequenciesMap
	//saves frequency map to file
	void UncompressWithFreqMap(const map<inT,freqT> &frequenciesMap, vector<inT> &valuesUncompressed, const char *filename);
	*/
};

#include "HuffmanWrapper.cpp"

#endif
