//Arithmetic coding Malte implementation wrapper class

#ifndef __ArithmeticCodingWrapper__
#define __ArithmeticCodingWrapper__

// #include <vector>
// #include <map>
// #include <iostream>
// #include <string>

#include "ModelOrder0C.h" //Arithmetic coding Malte implementation
#include "BoolFiles.h"

class ArithmeticCodingWrapper
{
private:
public:
	ArithmeticCodingWrapper() {} //constructor
	~ArithmeticCodingWrapper() {} //destructor
	
	//Arithmetic coding compress string stringToCompress, save to file filename
	void Compress(const string &stringToCompress, const char *filename);
	//Arithmetic coding uncompress string to string stringUncomporessed from file filename
	void Uncompress(string &stringUncompressed, const char *filename);
};

#endif
