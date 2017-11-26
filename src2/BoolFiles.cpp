#include "BoolFiles.h"

//writes bool vector to binary file filename
//format last char is size of overhang for last char from 0 to 7, number of bits of last char used
void BoolToFile2(const vector<bool> &vec, const char *filename)
{
	if (vec.size() < 16)
	{
		// write too short version as string
		string str;
		for (vector<bool>::const_iterator it = vec.begin() ; it != vec.end() ; ++it)
			str.push_back(*it ? '1' : '0');
			
		cout << "dels str: " << str << endl;
		cout << "length: " << str.size() << endl;
		// int zzz;
		// cin >> zzz;
		
		string too_short_filename_string(filename);
		too_short_filename_string += ".tooshort";
		
		StringToFile(str,too_short_filename_string.c_str());
		
		// string x(vec);
		// IntsToFile(vec,filename);
	} else
	{
		fstream file;
		file.open(filename,ios::out | ios::binary);
		// fstream *filePointer = &file;
		
		unsigned char overhang; //number of bits to use in last char
		
		unsigned char bitBuffer = 0; //holds 8 bits
		unsigned char bitCount = 0; //counts bits in buffer
		
		int numWrites = 0; //number of writes to file
		
		for (vector<bool>::const_iterator it = vec.begin() ; it != vec.end() ; ++it)
		{
			//add bit to buffer
			bitBuffer = (bitBuffer << 1) | *it;
			++bitCount;
			if (bitCount == 8)// || (it+1==vec.end())) //buffer full, write to file
			{
				file.write(reinterpret_cast<char*>(&bitBuffer),sizeof(bitBuffer));
				bitCount = 0; //reset bit count
				bitBuffer = 0;
				++numWrites;
			}
		}
		
		//fill remainder of buffer with 0, write to file
		overhang = bitCount; //overhang is number of bits used in last buffer
		
		while (bitCount != 0)
		{	//add 0 to buffer
			bitBuffer = (bitBuffer << 1) | 0;
			++bitCount;
			if (bitCount == 8) //buffer full, write to file
			{
				file.write(reinterpret_cast<char*>(&bitBuffer),sizeof(bitBuffer));
				bitCount = 0; //reset bit count
				++numWrites;
			}
		}
		
		std::cout << "overhang1: " << (int)overhang << std::endl;
		
		//write overhang value
		file.write(reinterpret_cast<char*>(&overhang),sizeof(overhang));
		++numWrites;
		
		std::cout << "num writes: " << numWrites << std::endl;
		
		//close file
		file.close();
	}
}

//reads bool vector from file filename
//format last char is overhang for last char overhang from 0 to 7, number of bits of last char used, writes into bool vector vec
void FileToBool2(const char *filename, vector<bool> &vec)
{
	// check of too short version of file exists
	string too_short_filename_string(filename);
	too_short_filename_string += ".tooshort";
	ifstream test_source(too_short_filename_string.c_str());
	if (test_source.is_open()) // string too short
	{
		cout << "file too short" << endl;
		cout << "-------------------------------------" << endl;
		test_source.close();
		string str;
		FileToString(str,too_short_filename_string.c_str());
		vec.clear();
		for (string::const_iterator it = str.begin() ; it != str.end(); ++it)
			vec.push_back((*it == '1') ? true : false);
		cout << "str size: " << str.size() << endl;
		cout << "vec size: " << vec.size() << endl;
		cout << str[0] << "," << vec[0] << endl;
	} else
	{
		cout << "file not too short" << endl;
		cout << "-------------------------------------" << endl;
		fstream file;
		file.open(filename,ios::in | ios::binary);
		// fstream *filePointer = &file;
		
		// vec.clear(); //set to empty vector
		vec.resize(0);
		
		unsigned char overhang = 0; //number of bits to use in last char
		
		unsigned char bitBuffer = 0; //holds 8 bits
		unsigned char bitCount = 0; //counts bits in buffer
		
		unsigned char bit; //bit read from buffer
		unsigned char bitBufferShiftedLeftRight; //bit buffer shifted left, right by 1 to compare to bit buffer bitBuffer to get last bit
		
		int numReads = 0; //number of reads from file
		
		while (!(file.eof())) // while not done reading file
		{
			file.read(reinterpret_cast<char*>(&bitBuffer),sizeof(bitBuffer));
			// file >> bitBuffer;
			++numReads;
			if (!(file.eof()))
			{
				overhang = bitBuffer;
				for (int i = 0; i < 8 ; ++i) // read bits in bit buffer
				{
					bit = (bitBuffer >> 7);
					vec.push_back(bit);
					
					bitBuffer <<= 1; //push bit buffer to left
				}
			}
		}
		
		// overhang = bitBuffer;
		
		//pop back 8 bits for overhang value stored
		for (int i = 0 ; i < 8 ; ++i)
			vec.pop_back();
		
		//pop back 8 - overhang bits
		// std::cout << "overhang2: " << (int)overhang << std::endl;
		if (overhang > 0)
		{
			for (int i = 0; i < (8-overhang) ; ++i)
				vec.pop_back();
		}
			
		std::cout << "num reads: " << numReads << std::endl;
			
		//close file
		file.close();
	}
}

//writes bool vector to binary file filename, returns int size of overhang for last char from 0 to 7, number of bits of last char used
void BoolToFile(const vector<bool> &vec, const char *filename, int &overhang)
{
	if (vec.size() < 16)
	{
		// write too short version as string
		string str;
		for (vector<bool>::const_iterator it = vec.begin() ; it != vec.end() ; ++it)
			str.push_back(*it ? '1' : '0');
		
		string too_short_filename_string(filename);
		too_short_filename_string += ".tooshort";
		
		StringToFile(str,too_short_filename_string.c_str());
		
		// string x(vec);
		// IntsToFile(vec,filename);
	} else
	{
		fstream file;
		file.open(filename,ios::out | ios::binary);
		// fstream *filePointer = &file;
		
		unsigned char bitBuffer = 0; //holds 8 bits
		unsigned char bitCount = 0; //counts bits in buffer
		
		int numWrites = 0; //number of writes to file
		
		for (vector<bool>::const_iterator it = vec.begin() ; it != vec.end() ; ++it)
		{
			//add bit to buffer
			bitBuffer = (bitBuffer << 1) | *it;
			++bitCount;
			if (bitCount == 8)// || (it+1==vec.end())) //buffer full, write to file
			{
				file.write(reinterpret_cast<char*>(&bitBuffer),sizeof(bitBuffer));
				bitCount = 0; //reset bit count
				bitBuffer = 0;
				++numWrites;
			}
		}
		
		//fill remainder of buffer with 0, write to file
		overhang = bitCount; //overhang is number of bits used in last buffer
		
		while (bitCount != 0)
		{	//add 0 to buffer
			bitBuffer = (bitBuffer << 1) | 0;
			++bitCount;
			if (bitCount == 8) //buffer full, write to file
			{
				file.write(reinterpret_cast<char*>(&bitBuffer),sizeof(bitBuffer));
				bitCount = 0; //reset bit count
				++numWrites;
			}
		}
		std::cout << "num writes: " << numWrites << std::endl;
		
		//close file
		file.close();
	}
}

//reads bool vector from file filename with overhang for last char overhang from 0 to 7, number of bits of last char used, writes into bool vector vec
void FileToBool(const char *filename, const int overhang, vector<bool> &vec)
{
	// check of too short version of file exists
	string too_short_filename_string(filename);
	too_short_filename_string += ".tooshort";
	ifstream test_source(too_short_filename_string.c_str());
	if (test_source.is_open()) // string too short
	{
		test_source.close();
		string str;
		FileToString(str,too_short_filename_string.c_str());
		vec.clear();
		for (string::const_iterator it = str.begin() ; it != str.end(); ++it)
			vec.push_back((*it == '1') ? true : false);
	} else
	{
		fstream file;
		file.open(filename,ios::in | ios::binary);
		// fstream *filePointer = &file;
		
		// vec.clear(); //set to empty vector
		vec.resize(0);
		
		unsigned char bitBuffer = 0; //holds 8 bits
		unsigned char bitCount = 0; //counts bits in buffer
		
		unsigned char bit; //bit read from buffer
		unsigned char bitBufferShiftedLeftRight; //bit buffer shifted left, right by 1 to compare to bit buffer bitBuffer to get last bit
		
		int numReads = 0; //number of reads from file
		
		while (!(file.eof())) // while not done reading file
		{
			file.read(reinterpret_cast<char*>(&bitBuffer),sizeof(bitBuffer));
			// file >> bitBuffer;
			++numReads;
			if (!(file.eof())){
			for (int i = 0; i < 8 ; ++i) // read bits in bit buffer
			{
				bit = (bitBuffer >> 7);
				vec.push_back(bit);
				
				bitBuffer <<= 1; //push bit buffer to left
			}}
		}
		//pop back 8 - overhang bits
		if (overhang > 0)
		{
			for (int i = 0; i < (8-overhang) ; ++i)
				vec.pop_back();
		}
			
		std::cout << "num reads: " << numReads << std::endl;
			
		//close file
		file.close();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//writes integers list to file
void IntsToFile(const vector<int> &values, const char *filename) //writes integers list to file
{
	//open file
	ofstream dest(filename);
	
	for (vector<int>::const_iterator it = values.begin() ; it != values.end() ; ++it)
		dest << *it << std::endl;
	
	//close file
	dest.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reads integers list from file
void FileToInts(vector<int> &values, const char *filename)
{
	//open file
	ifstream source(filename);
	
	//empty vector
	values.clear();
	
	string line; //line read
	int value; //value
	
	stringstream numstr; //holds position number
	
	while (getline(source,line))
	{
		numstr.clear();
		numstr << line;
		numstr >> value;
		
		values.push_back(value); //push back value to list
	}
	//close file
	source.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//write string to file
void StringToFile(const string &s, const char *filename)
{
	//open file
	ofstream dest(filename);	
	
	//write string, new line character
	dest << s << std::endl;
	
	//close file
	dest.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//read string from file
void FileToString(string &s, const char *filename, const unsigned int numInitialLinesToSkip)
{
	//open file
	ifstream source(filename);	
	
	//empty string
	s.clear();
	
	string line; //line read
	
	//skip numInitialLinesToSkip initial lines
	for (unsigned int i = 0; i < numInitialLinesToSkip ; ++i)
		getline(source,line);
	
	//read string
	while (getline(source,line))
		s += line;
	
	//close file
	source.close();
}

// read multiple strings from file
void FileToStrings(vector<string> &v, const char *filename)
{
	//open file
	ifstream source(filename);
	
	//empty vector
	v.clear();
	
	string line; // line read
	
	//read string
	// cout << "HERE" << endl;
	while (getline(source,line))
	{
		// cout << "line: " << line << endl;
		v.push_back(line);}
	
	//close file
	source.close();
}