//Arithmetic coding Malte implementation ArithmeticcodingWrapper class implementation

#include "ArithmeticCodingWrapper.h"

const int g_Signature = 0x434D4341; //from Malte Arithmetic coding implementation

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arithmetic coding compress string stringToCompress, save to file filename
void ArithmeticCodingWrapper::Compress(const string &stringToCompress, const char *filename)
{
	//write string to temporary file
	// string string_temp_filename(filename); string_temp_filename = "string_temp_toCompress_" + string_temp_filename;
	string string_temp_filename(filename); string_temp_filename += ".temp";
	StringToFile(stringToCompress,string_temp_filename.c_str());
	
	if (stringToCompress.size() > 10)
	{
		//below copied from Malte arithmetic coding implementation Main.cpp
		
		fstream source, target;
		ModelI* model;

		// choose model, here just order-0
		model = new ModelOrder0C;

		source.open( string_temp_filename.c_str(), ios::in | ios::binary );
		target.open( filename, ios::out | ios::binary );

		if( !source.is_open() )
		{
			cout << "Cannot open input stream";
			// return 2;
		}
		if( !target.is_open() )
		{
			cout << "Cannot open output stream";
			// return 3;
		}

		unsigned int signature;
		source.read(reinterpret_cast<char*>(&signature),sizeof(signature));
		// if( signature == g_Signature )
		// {
			// cout << "Decoding " << string_temp_filename.c_str() << " to " << filename << endl;
			// model->Process( &source, &target, MODE_DECODE );
		// }
		// else
		// {
			cout << "Encoding " << filename << " to " << string_temp_filename.c_str() << endl;
			source.seekg( 0, ios::beg );
			target.write( reinterpret_cast<const char*>(&g_Signature),
										sizeof(g_Signature) );
			model->Process( &source, &target, MODE_ENCODE );
		// }

		//deallocate Model pointer //added
		delete model; //added
		
		source.close();
		target.close();
		
		//end copied from Malte arithmetic coding implementation Main.cpp
		
		//delete temporary file
		if (stringToCompress.size() < 2)
			remove(string_temp_filename.c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arithmetic coding uncompress string to string stringUncomporessed from file filename
void ArithmeticCodingWrapper::Uncompress(string &stringUncompressed, const char *filename)
{
	
	//write string to temporary file
	// string string_temp_filename(filename); string_temp_filename = "string_temp_toUncompress_" + string_temp_filename;
	string string_temp_filename(filename); string_temp_filename += ".temp";
	
	// check if temp exists
	ifstream test_source(string_temp_filename.c_str());
	if (test_source.is_open()) // string too short
	{
		// cout << "HERE" << endl;
		// cout << string_temp_filename << endl;
		test_source.close();
		FileToString(stringUncompressed,string_temp_filename.c_str());
		// cout << "HERE2" << endl;
		// int zzz;
		// cin >> zzz;
	} else
	{	
		// cout << "THERE" << endl;
		//below copied from Malte arithmetic coding implementation Main.cpp
		
		fstream source, target;
		ModelI* model;

		// choose model, here just order-0
		model = new ModelOrder0C;

		source.open( filename, ios::in | ios::binary );
		target.open( string_temp_filename.c_str(), ios::out | ios::binary );

		if( !source.is_open() )
		{
			cout << "Cannot open input stream";
			// return 2;
		}
		if( !target.is_open() )
		{
			cout << "Cannot open output stream";
			// return 3;
		}

		unsigned int signature;
		source.read(reinterpret_cast<char*>(&signature),sizeof(signature));
		// if( signature == g_Signature )
		// {
			cout << "Decoding " << filename << " to " << string_temp_filename.c_str() << endl;
			model->Process( &source, &target, MODE_DECODE );
		// }
		// else
		// {
			// cout << "Encoding " << string_temp_filename.c_str() << " to " << filename << endl;
			// source.seekg( 0, ios::beg );
			// target.write( reinterpret_cast<const char*>(&g_Signature),
										// sizeof(g_Signature) );
			// model->Process( &source, &target, MODE_ENCODE );
		// }

		//deallocate Model pointer //added
		delete model; //added
		
		source.close();
		target.close();
		
		//end copied from Malte arithmetic coding implementation Main.cpp
		
		//read string from temporary file	
		FileToString(stringUncompressed,string_temp_filename.c_str());
		
		//delete temporary file
		// remove(string_temp_filename.c_str());
	}
	//delete temporary file
	remove(string_temp_filename.c_str());
}
