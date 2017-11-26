//Huffman LP implementation HuffmanWrapper class implementation

#include "HuffmanWrapper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Huffman coding compress values vector valuesToCompress, save to file filename, frequencies table frequenciesMap
template <typename inT, typename freqT>
void HuffmanWrapper<inT,freqT>::Compress(const map<inT,freqT> &frequenciesMap, const vector<inT> &valuesToCompress, const char *filename)
{
	if (frequenciesMap.size() > 1)
	{
		//make Huffman table
		cout << "---";
		cout << frequenciesMap.size() << endl;
		Hufftree <inT,freqT> myHufftree(frequenciesMap.begin(),frequenciesMap.end());
		
		//compress
		cout << "Huffman coding compressing...";
		vector<bool> encodedDistances = myHufftree.encode(valuesToCompress.begin(), valuesToCompress.end());
		cout << "done" << endl;
		
		
		// vector<bool> encodedDistances = myHufftree.encode(valuesToCompress.begin(), valuesToCompress.end());
		
		//write to file
		string filename_encodedDistances(filename);
		// filename_encodedDistances = "encodedValues_" + filename_encodedDistances;
		//last character in file stores overhang
		cout << "num: " << encodedDistances.size() << endl;
		
		BoolToFile2(encodedDistances,filename_encodedDistances.c_str());
		
		// for (int i = 0; i < 10 ; ++i)
			// std::cout << encodedDistances[i] << ",";
		std::cout << std::endl;
	}
	else
	{
		IntsToFile(valuesToCompress,filename);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Huffman coding uncompress values vector to valuesUncompressed from file filename, frequencies table frequenciesMap
template <typename inT, typename freqT>
void HuffmanWrapper<inT,freqT>::Uncompress(const map<inT,freqT> &frequenciesMap, vector<inT> &valuesUncompressed, const char *filename)
{
	if (frequenciesMap.size() > 1)
	{
		// std::cout << "uncompressing1" << std::endl;
		//read bool vector from file
		vector<bool> encodedFromFile;
		FileToBool2(filename,encodedFromFile);
		
		// std::cout << "uncompressing2" << std::endl;
		//make Huffman table
		Hufftree<inT,freqT> myHufftree(frequenciesMap.begin(),frequenciesMap.end());
		
		// std::cout << "uncompressing3" << std::endl;
		//clear values uncompressed vector
		valuesUncompressed.clear();
		
		// for (vector<bool>::iterator it = encodedFromFile.begin() ; it != encodedFromFile.end() ; ++it)
			// std::cout << *it;
		std::cout << std::endl;
		
		// std::cout << "uncompressing4" << std::endl;
		//uncompress
		// typename vector<inT>::iterator uncompressed_it = valuesUncompressed.begin();
		// myHufftree.decode(encodedFromFile,uncompressed_it);
		myHufftree.decode(encodedFromFile,std::back_inserter(valuesUncompressed));
		
		// std::cout << "uncompressing5" << std::endl;
	}
	else
	{
		valuesUncompressed.clear();
		if (frequenciesMap.size() > 0)
		{
			valuesUncompressed = vector<int>(frequenciesMap.begin()->second,frequenciesMap.begin()->first);
		}
		cout << "values uncompressed: " << endl;
		for (vector<int>::iterator it = valuesUncompressed.begin(); it != valuesUncompressed.end(); ++it)
			cout << *it << endl;
		cout << "-----------" << endl;
	}
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Huffman coding compress values vector valuesToCompress, save to file filename, frequencies table frequenciesMap
//saves frequency map to file
template <typename inT, typename freqT>
void HuffmanWrapper<inT,freqT>::CompressWithFreqMap(const map<inT,freqT> &frequenciesMap, const vector<inT> &valuesToCompress, const char *filename)
{
	//call compress function
	Compress(frequenciesMap, valuesToCompress, filename);
	
	//save frequency map
	// string freqName
}

//Huffman coding uncompress values vector from file filename, frequencies table frequenciesMap
//saves frequency map to file
template <typename inT, typename freqT>
void HuffmanWrapper<inT,freqT>::UncompressWithFreqMap(const map<inT,freqT> &frequenciesMap, vector<inT> &valuesUncompressed, const char *filename)
{
}
*/
