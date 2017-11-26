
//free functions to write bool vector to and from file
//used, modified code from Malte arithmetic code implementation

#ifndef __BoolFiles__
#define __BoolFiles__

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

// #include <iostream>
// #include <math.h>
// #include <vector>
// #include <set>
// #include <fstream>


using std::vector;
using std::map;
using std::fstream;
using std::ios;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ostringstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::cin;

// using namespace std;


//writes bool vector to binary file filename, returns int size of overhang for last char from 0 to 7, number of bits of last char used
void BoolToFile(const vector<bool> &vec, const char *filename, int &overhang);
//reads bool vector from file filename with overhang for last char overhang from 0 to 7, number of bits of last char used, writes into bool vector vec
void FileToBool(const char *filename, const int overhang, vector<bool> &vec);

//writes bool vector to binary file filename
//format last char is size of overhang for last char from 0 to 7, number of bits of last char used
void BoolToFile2(const vector<bool> &vec, const char *filename);
//reads bool vector from file filename
//format last char is overhang for last char overhang from 0 to 7, number of bits of last char used, writes into bool vector vec
void FileToBool2(const char *filename, vector<bool> &vec);

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//writes integers list to file
void IntsToFile(const vector<int> &values, const char *filename);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reads integers list from file
void FileToInts(vector<int> &values, const char *filename);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//write string to file
void StringToFile(const string &s, const char *filename);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//read string from file
void FileToString(string &s, const char *filename, const unsigned int numInitialLinesToSkip = 0);

// read multiple strings from file
void FileToStrings(vector<string> &v, const char *filename);

// using namespace std;

#endif
