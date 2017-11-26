//implementation of Compressor class

#include "Compressor.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////
Compressor::Compressor(bool usedbSNP) //constructor
{
	//write parameters for doubple power law fits, computed in Matlab
	
	//dbSNP129
	
	//novel distances
	double paramsNovelDistances[] = {4.1143,0.6624,3.4149,4.0945,1.5913};
	for (int i = 0; i < 5 ; ++i)
		novelDistances_PMF_DPL_parameters.push_back(paramsNovelDistances[i]);
	//distance between indices in dbSNP
	double paramsdbSNPDistances[] = {6.0710,1.1503,0.4394,2.8109,0.6648};// {6.0942,1.2359,0.3882,2.7630,0.5379};// {6.0326,1.1435,0.4858,2.8054,0.5144};
	for (int i = 0; i < 5 ; ++i)
		dbSNPDistances_PMF_DPL_parameters.push_back(paramsdbSNPDistances[i]);	
	
	if (usedbSNP == false)
	{
		// no dbSNP
		//novel distances
		double paramsNovelDistances_nodbSNP[] = {4.3628,0.2898,2.8294,3.2070,2.3924};
		novelDistances_PMF_DPL_parameters.clear();
		for (int i = 0; i < 5 ; ++i)
			novelDistances_PMF_DPL_parameters.push_back(paramsNovelDistances_nodbSNP[i]);
	}		
	
	//dbSNP130
		
	/*
	//novel distances
	double paramsNovelDistances[] = {3.7561,0.7190,3.4335,3.3670,0.8313};
	for (int i = 0; i < 5 ; ++i)
		novelDistances_PMF_DPL_parameters.push_back(paramsNovelDistances[i]);
	//distance between indices in dbSNP
	double paramsdbSNPDistances[] = {6.1209,1.1699,0.4814,2.9028,0.5363};// {6.0942,1.2359,0.3882,2.7630,0.5379};// {6.0326,1.1435,0.4858,2.8054,0.5144};
	for (int i = 0; i < 5 ; ++i)
		dbSNPDistances_PMF_DPL_parameters.push_back(paramsdbSNPDistances[i]);	
	*/
	
	/*
	// HG00102, dbSNP137, genotype 1
	double paramsNovelDistances[] = {3.4955,0.6548,3.2134,2.7294,0.4565};
	for (int i = 0; i < 5 ; ++i)
		novelDistances_PMF_DPL_parameters.push_back(paramsNovelDistances[i]);
	//distance between indices in dbSNP
	double paramsdbSNPDistances[] = {5.9126,0.8970,0.4372,2.5234,0.5071};// {6.0942,1.2359,0.3882,2.7630,0.5379};// {6.0326,1.1435,0.4858,2.8054,0.5144};
	for (int i = 0; i < 5 ; ++i)
		dbSNPDistances_PMF_DPL_parameters.push_back(paramsdbSNPDistances[i]);	
	*/
	
	/*
	// HG00102, dbSNP137, genotype 2
	double paramsNovelDistances[] = {3.4880,0.6549,3.2169,2.7274,0.4544};
	for (int i = 0; i < 5 ; ++i)
		novelDistances_PMF_DPL_parameters.push_back(paramsNovelDistances[i]);
	//distance between indices in dbSNP
	double paramsdbSNPDistances[] = {5.9134,0.7094,0.4641,2.5589,0.8739};// {6.0942,1.2359,0.3882,2.7630,0.5379};// {6.0326,1.1435,0.4858,2.8054,0.5144};
	for (int i = 0; i < 5 ; ++i)
		dbSNPDistances_PMF_DPL_parameters.push_back(paramsdbSNPDistances[i]);
	*/
		
	/*
	// HG00102, dbSNP137
	// double paramsNovelDistances[] = {3.6734,0.6512,3.4139,3.4752,0.7928};
	double paramsNovelDistances[] = {3.9367,0.7019,3.3060,3.3891,0.8731};
	for (int i = 0; i < 5 ; ++i)
		novelDistances_PMF_DPL_parameters.push_back(paramsNovelDistances[i]);
	//distance between indices in dbSNP
	double paramsdbSNPDistances[] = {6.1369,1.0686,0.3136,2.6080,0.1120};// {6.0942,1.2359,0.3882,2.7630,0.5379};// {6.0326,1.1435,0.4858,2.8054,0.5144};
	for (int i = 0; i < 5 ; ++i)
		dbSNPDistances_PMF_DPL_parameters.push_back(paramsdbSNPDistances[i]);	
	*/
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
Compressor::~Compressor() //destructor
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Compressor::WriteIntsToFile(const vector<int> &novelDistances, const char *filename) //writes distances to file
{
	//open file
	ofstream dest(filename);	
	
	string line; //line to write
	ostringstream numstr; //holds position
	int position = 0; //variation position
	int prevPosition = 0; //previous position
	
	cout << "HERE" << endl;
	
	for (int i = 0; i < novelDistances.size() ; ++i)
	{
		position = novelDistances[i];
		if (position < prevPosition) //reset line buffer text
			numstr.str("");
		prevPosition = position;
		
		numstr << position;
		numstr.seekp(0); //move put pointer back to front
						
		line = numstr.str();
		
		//write line
		dest << line << endl;
	}
	//close file
	dest.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Compressor::WriteStringToFile(const string &s, const char *filename) //write string to file
{
	//open file
	ofstream dest(filename);	
	
	//write string, new line character
	dest << s << endl;
	
	//close file
	dest.close();
}
	
	// string line; //line to write
	
	// for (int i = 0; i < s.size() ; ++i)
		// dest << s[i]

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//compresses difference map in_diffMap, writes files to directory outdirectory
void Compressor::Compress(const DifferenceMap & in_diffMap, const char *outdirectory)
{
	//file names
	string novelDistancesFilename(outdirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(outdirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(outdirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(outdirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(outdirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(outdirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(outdirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will append A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(outdirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(outdirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(outdirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(outdirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(outdirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
		
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to compress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist(insertionLengths);
	
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	////////////////////////////////////////////////////////////////////////////////////////
	// compress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//compress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	int maxvalue = 100000;
	myPMF_DPL_Compressor.Compress(novelDistances,1,maxvalue,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//compress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Compress(dbSNPDistances,1,maxvalue,dbSNPDistancesFilename.c_str());
	
	//compress dbSNP match, partial match string by arithmetic coding
	myArithmeticCodingWrapper.Compress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	
	//compress deletion lengths by Huffman coding	
	myHuffmanWrapper.Compress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	
	//compress insertion lengths by Huffman coding	
	myHuffmanWrapper.Compress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	
	//compress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Compress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	//compress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}	
	//partial matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//compress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Compress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	////////////////////////////////////////////////////////////////////////////////////////
	// write more variables
	//deletions lengths histogram
	WriteIntIntFreqMapToFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	WriteIntIntFreqMapToFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	IntsToFile(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	IntsToFile(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	StringToFile(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	StringToFile(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	/*//uncompress novel distances by PMF double power law for distances
	vector<int> dist_uncompressed;
	cout << "uncompressing...";
	myPMF_DPL_Compressor.Uncompress(dist_uncompressed,1,100000,"file1.txt");
	cout << "done...";*/
	
	/*
	//uncompress dbSNP distances by PMF double power law for distances
	vector<int> dist_uncompressed;
	cout << "uncompressing...";
	myPMF_DPL_Compressor.SetParameters(novelDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Uncompress(dist_uncompressed,1,100000,novelDistancesFilename.c_str());
	cout << "done...";
	
	cout << "v1 size: " << novelDistances.size() << endl;
	cout << "v2 size: " << dist_uncompressed.size() << endl;
	int i = 0;
	bool match = true;
	if (novelDistances.size() != dist_uncompressed.size())
		match = false;
	for (vector<int>::iterator it = novelDistances.begin() ; it != novelDistances.end() ; ++it)
	{	if (*it != dist_uncompressed[i])
		{
			cout << "mismatch: " << *it << "," << dist_uncompressed[i] << " " << i << endl;
			match = false;
			int ii;
			cin >> ii;
		}
		++i;
	}
	
	cout << "in out vectors equal: " << match << endl;
	*/
	
	IntsToFile(dbSNPDistances,"dbSNP_distances.txt");
	IntsToFile(novelDistances,"novel_distances.txt");
		
	// PMF_DPL_Compressor myPMF_DPL_Compressor();
	// myPMF_DPL_Compressor.Make()
	
	// CompressHuffmanPMF_DPL(novelDistances,novelDistancePMFParameters);
	/*
	// WriteIntsToFile(novelDistances,"chr10distances.txt");
	// WriteIntsToFile(dbSNPDistances,"chr10_dbSNP_distances.txt");
	// WriteStringToFile(dbSNPMatchString,"chr10_dbSNP_MatchString.txt");
	// WriteStringToFile(novelLabels,"chr10novelLabels.txt");
	// WriteIntsToFile(novelChromLengths,"chr10chromLengths.txt");
	// WriteIntsToFile(dbSNPChromLengths,"chr10_dbSNP_chromLengths.txt");
	WriteIntsToFile(novelDistances,"noveldistances.txt");
	WriteIntsToFile(dbSNPDistances,"dbSNP_distances.txt");
	WriteStringToFile(dbSNPMatchString,"dbSNP_MatchString.txt");
	WriteStringToFile(novelLabels,"novelLabels.txt");
	WriteIntsToFile(novelChromLengths,"novelchromLengths.txt");
	WriteIntsToFile(dbSNPChromLengths,"dbSNP_chromLengths.txt");
	WriteStringToFile(insertionSequences,"insertionSequences.txt");
	WriteIntsToFile(deletionLengths,"deletionLengths.txt");
	WriteIntsToFile(insertionLengths,"insertionLengths.txt");
	
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename += "_substvalues.txt";
		WriteStringToFile(it->second,filename.c_str());
	}
	
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename += "_partialMatches_substvalues.txt";
		WriteStringToFile(it->second,filename.c_str());
	}*/
	
	cout << "max value: " << maxvalue << endl;
}

void Compressor::Compress(const DifferenceMap & in_diffMap, const char *outdirectory_, const string chromName)
{	
	string outdirectory_string(outdirectory_);
	outdirectory_string += "/";
	outdirectory_string += (chromName + "/");
	const char *outdirectory = outdirectory_string.c_str();

	//file names
	string novelDistancesFilename(outdirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(outdirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(outdirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(outdirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(outdirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(outdirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(outdirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will append A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(outdirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(outdirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(outdirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(outdirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(outdirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
		
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to compress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist(insertionLengths);
	
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	////////////////////////////////////////////////////////////////////////////////////////
	// compress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//compress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	myPMF_DPL_Compressor.Compress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//compress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Compress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	
	//compress dbSNP match, partial match string by arithmetic coding
	myArithmeticCodingWrapper.Compress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	
	
	//compress deletion lengths by Huffman coding	
	cout << "here1" << endl;
	myHuffmanWrapper.Compress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	cout << "here2" << endl;
	cout << "delsHist.histMap.size(): " << delsHist.histMap.size() << endl;
	cout << "deletionLengths.size(): " << deletionLengths.size() << endl;
	// int zzz; cin >> zzz;
	
	//compress insertion lengths by Huffman coding	
	cout << "here3" << endl;
	myHuffmanWrapper.Compress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	cout << "here4" << endl;
	
	//compress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Compress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	//compress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}	

	//partial matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//compress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Compress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	////////////////////////////////////////////////////////////////////////////////////////
	// write more variables
	//deletions lengths histogram
	WriteIntIntFreqMapToFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	WriteIntIntFreqMapToFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	IntsToFile(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	IntsToFile(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	StringToFile(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	StringToFile(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	/*//uncompress novel distances by PMF double power law for distances
	vector<int> dist_uncompressed;
	cout << "uncompressing...";
	myPMF_DPL_Compressor.Uncompress(dist_uncompressed,1,100000,"file1.txt");
	cout << "done...";*/
	
	/*
	//uncompress dbSNP distances by PMF double power law for distances
	vector<int> dist_uncompressed;
	cout << "uncompressing...";
	myPMF_DPL_Compressor.SetParameters(novelDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Uncompress(dist_uncompressed,1,100000,novelDistancesFilename.c_str());
	cout << "done...";
	
	cout << "v1 size: " << novelDistances.size() << endl;
	cout << "v2 size: " << dist_uncompressed.size() << endl;
	int i = 0;
	bool match = true;
	if (novelDistances.size() != dist_uncompressed.size())
		match = false;
	for (vector<int>::iterator it = novelDistances.begin() ; it != novelDistances.end() ; ++it)
	{	if (*it != dist_uncompressed[i])
		{
			cout << "mismatch: " << *it << "," << dist_uncompressed[i] << " " << i << endl;
			match = false;
			int ii;
			cin >> ii;
		}
		++i;
	}
	
	cout << "in out vectors equal: " << match << endl;
	*/
	
	IntsToFile(dbSNPDistances,"dbSNP_distances.txt");
	IntsToFile(novelDistances,"novel_distances.txt");
		
	// PMF_DPL_Compressor myPMF_DPL_Compressor();
	// myPMF_DPL_Compressor.Make()
	
	// CompressHuffmanPMF_DPL(novelDistances,novelDistancePMFParameters);
	/*
	// WriteIntsToFile(novelDistances,"chr10distances.txt");
	// WriteIntsToFile(dbSNPDistances,"chr10_dbSNP_distances.txt");
	// WriteStringToFile(dbSNPMatchString,"chr10_dbSNP_MatchString.txt");
	// WriteStringToFile(novelLabels,"chr10novelLabels.txt");
	// WriteIntsToFile(novelChromLengths,"chr10chromLengths.txt");
	// WriteIntsToFile(dbSNPChromLengths,"chr10_dbSNP_chromLengths.txt");
	WriteIntsToFile(novelDistances,"noveldistances.txt");
	WriteIntsToFile(dbSNPDistances,"dbSNP_distances.txt");
	WriteStringToFile(dbSNPMatchString,"dbSNP_MatchString.txt");
	WriteStringToFile(novelLabels,"novelLabels.txt");
	WriteIntsToFile(novelChromLengths,"novelchromLengths.txt");
	WriteIntsToFile(dbSNPChromLengths,"dbSNP_chromLengths.txt");
	WriteStringToFile(insertionSequences,"insertionSequences.txt");
	WriteIntsToFile(deletionLengths,"deletionLengths.txt");
	WriteIntsToFile(insertionLengths,"insertionLengths.txt");
	
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename += "_substvalues.txt";
		WriteStringToFile(it->second,filename.c_str());
	}
	
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename += "_partialMatches_substvalues.txt";
		WriteStringToFile(it->second,filename.c_str());
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Compressor::CompressDistances(const DifferenceMap & in_diffMap, const char *outdirectory_, const string chromName, NondistanceData & nondistanceData_)
{	
	string outdirectory_string(outdirectory_);
	outdirectory_string += "/";
	// outdirectory_string += (chromName + "/");
	const char *outdirectory = outdirectory_string.c_str();

	//file names
	string novelDistancesFilename(outdirectory); novelDistancesFilename += chromName + ".novelDistances.txt";
	// string deletionsLengthsFilename(outdirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	// string insertionsLengthsFilename(outdirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	// string insertionSequenceFilename(outdirectory); insertionSequenceFilename += "insertionSequence.txt";
	// string novelChromLengthsFilename(outdirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	// string novelLabelsFilename(outdirectory); novelLabelsFilename += "novelLabels.txt";
	// string SNPNovelValuesFilename(outdirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will append A,C,G,T initial symbol to filename
	// string SNPNovelValuesFilenameUniqueOldValuesMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	// string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(outdirectory); dbSNPDistancesFilename += chromName + ".dbSNPDistances.txt";
	// string dbSNPChromLengthsFilename(outdirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	// string dbSNPMatchesAndPartialMatchesFilename(outdirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	// string deletionsLengthsHistFilename(outdirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	// string insertionsLengthsHistFilename(outdirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
		
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to compress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist(insertionLengths);
	
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	////////////////////////////////////////////////////////////////////////////////////////
	// compress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//compress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	myPMF_DPL_Compressor.Compress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;

	//compress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Compress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	
	/*
	//compress dbSNP match, partial match string by arithmetic coding
	myArithmeticCodingWrapper.Compress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	
	
	//compress deletion lengths by Huffman coding	
	cout << "here1" << endl;
	myHuffmanWrapper.Compress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	cout << "here2" << endl;
	cout << "delsHist.histMap.size(): " << delsHist.histMap.size() << endl;
	cout << "deletionLengths.size(): " << deletionLengths.size() << endl;
	// int zzz; cin >> zzz;
	
	//compress insertion lengths by Huffman coding	
	cout << "here3" << endl;
	myHuffmanWrapper.Compress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	cout << "here4" << endl;
	
	//compress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Compress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	//compress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}	

	//partial matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//compress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Compress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	////////////////////////////////////////////////////////////////////////////////////////
	// write more variables
	//deletions lengths histogram
	WriteIntIntFreqMapToFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	WriteIntIntFreqMapToFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	IntsToFile(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	IntsToFile(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	StringToFile(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	StringToFile(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	
	
	IntsToFile(dbSNPDistances,"dbSNP_distances.txt");
	IntsToFile(novelDistances,"novel_distances.txt");
	*/	
	
	// append to non-distance data
	for (string::const_iterator it = novelLabels.begin(); it != novelLabels.end(); ++it)
		nondistanceData_.novelLabels.push_back(*it);
	nondistanceData_.novelChromLengths.push_back(novelChromLengths[myChromNames.ChromNameToIndex(chromName)]);
	for (vector<int>::const_iterator it = deletionLengths.begin(); it != deletionLengths.end(); ++it)
		nondistanceData_.deletionLengths.push_back(*it);
	for (vector<int>::const_iterator it = insertionLengths.begin(); it != insertionLengths.end(); ++it)
		nondistanceData_.insertionLengths.push_back(*it);
	for (string::const_iterator it = insertionSequences.begin(); it != insertionSequences.end(); ++it)
		nondistanceData_.insertionSequences.push_back(*it);
	
	for (map<char,string>::const_iterator it_map = SNPSubstitutionStrings.begin(); it_map != SNPSubstitutionStrings.end(); ++it_map)
	{
		char current_char = it_map->first;
		for (string::const_iterator it_string = it_map->second.begin(); it_string != it_map->second.end(); ++it_string)
			nondistanceData_.SNPSubstitutionStrings[current_char].push_back(*it_string);
	}
	for (map<char,string>::const_iterator it_map = SNPSubstitutionStringsPartialMatches.begin(); it_map != SNPSubstitutionStringsPartialMatches.end(); ++it_map)
	{
		char current_char = it_map->first;
		for (string::const_iterator it_string = it_map->second.begin(); it_string != it_map->second.end(); ++it_string)
			nondistanceData_.SNPSubstitutionStringsPartialMatches[current_char].push_back(*it_string);
	}
	
	for (string::const_iterator it = dbSNPMatchString.begin(); it != dbSNPMatchString.end(); ++it)
		nondistanceData_.dbSNPMatchString.push_back(*it);
	nondistanceData_.dbSNPChromLengths.push_back(dbSNPChromLengths[myChromNames.ChromNameToIndex(chromName)]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//compresses difference map in_diffMap, writes files to directory outdirectory
void Compressor::CompressNondistances(NondistanceData & nondistanceData_, const char *outdirectory)
{
	//file names
	string novelDistancesFilename(outdirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(outdirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(outdirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(outdirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(outdirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(outdirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(outdirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will append A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(outdirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(outdirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(outdirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(outdirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(outdirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(outdirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
		
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to compress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	// vector<int> novelDistances;
	string novelLabels = nondistanceData_.novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths = nondistanceData_.novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths = nondistanceData_.deletionLengths; //deletion lengths
	vector<int> insertionLengths = nondistanceData_.insertionLengths; //insertion lengths
	string insertionSequences = nondistanceData_.insertionSequences; //insertion sequences concatenated
	// GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings = nondistanceData_.SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches = nondistanceData_.SNPSubstitutionStringsPartialMatches;
	// GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist(insertionLengths);
	
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	// vector<int> dbSNPDistances;
	string dbSNPMatchString = nondistanceData_.dbSNPMatchString;
	vector<int> dbSNPChromLengths = nondistanceData_.dbSNPChromLengths; //chromosome lengths in number of novel variations
	// GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	////////////////////////////////////////////////////////////////////////////////////////
	// compress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	/*
	//compress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	int maxvalue = 100000;
	myPMF_DPL_Compressor.Compress(novelDistances,1,maxvalue,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//compress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Compress(dbSNPDistances,1,maxvalue,dbSNPDistancesFilename.c_str());
	*/
	
	//compress dbSNP match, partial match string by arithmetic coding
	myArithmeticCodingWrapper.Compress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	
	//compress deletion lengths by Huffman coding	
	myHuffmanWrapper.Compress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	
	//compress insertion lengths by Huffman coding	
	myHuffmanWrapper.Compress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	
	//compress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Compress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	//compress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}	
	//partial matches
	for (map<char,string>::const_iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Compress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//compress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Compress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	////////////////////////////////////////////////////////////////////////////////////////
	// write more variables
	//deletions lengths histogram
	WriteIntIntFreqMapToFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	WriteIntIntFreqMapToFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	IntsToFile(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	IntsToFile(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	StringToFile(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	StringToFile(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	// IntsToFile(dbSNPDistances,"dbSNP_distances.txt");
	// IntsToFile(novelDistances,"novel_distances.txt");
		
	// cout << "max value: " << maxvalue << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Compressor::UncompressDistances(const char *indirectory_, const string chromName, DistanceData & distanceData_, bool usedbSNP)
{
	string indirectory_string(indirectory_);
	indirectory_string += "/";
	// indirectory_string += (chromName + "/");
	const char *indirectory = indirectory_string.c_str();

	cout << "Uncompress 1" << endl;
	//file names
	string novelDistancesFilename(indirectory); novelDistancesFilename += chromName + ".novelDistances.txt";
	// string deletionsLengthsFilename(indirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	// string insertionsLengthsFilename(indirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	// string insertionSequenceFilename(indirectory); insertionSequenceFilename += "insertionSequence.txt";
	// string novelChromLengthsFilename(indirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	// string novelLabelsFilename(indirectory); novelLabelsFilename += "novelLabels.txt";
	// string SNPNovelValuesFilename(indirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will prepend A,C,G,T initial symbol to filename
	// string SNPNovelValuesFilenameUniqueOldValuesMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	// string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(indirectory); dbSNPDistancesFilename += chromName + ".dbSNPDistances.txt";
	// string dbSNPChromLengthsFilename(indirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	// string dbSNPMatchesAndPartialMatchesFilename(indirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	// string deletionsLengthsHistFilename(indirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	// string insertionsLengthsHistFilename(indirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
	
	cout << "Uncompress 2" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to uncompress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;

	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	
	cout << "Uncompress 6" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// uncompress
	
	/*//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	*/
	//uncompress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	myPMF_DPL_Compressor.Uncompress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//uncompress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	cout << "Uncompress 6.1" << endl;
	if (usedbSNP)
		myPMF_DPL_Compressor.Uncompress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	cout << "Uncompress 6.2" << endl;
	
	// append to distance data
	for (vector<int>::const_iterator it = novelDistances.begin(); it != novelDistances.end(); ++it)
		distanceData_.novelDistances.push_back(*it);
	for (vector<int>::const_iterator it = dbSNPDistances.begin(); it != dbSNPDistances.end(); ++it)
		distanceData_.dbSNPDistances.push_back(*it);
}

//uncompresses difference map out_diffMap from files in directory indirectory, reference genome in directory refGenomeDirectory
void Compressor::UncompressNondistances(const char *indirectory, NondistanceData & nondistanceData_, bool usedbSNP)
{
	cout << "Uncompress 1" << endl;
	//file names
	// string novelDistancesFilename(indirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(indirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(indirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(indirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(indirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(indirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(indirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will prepend A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	// string dbSNPDistancesFilename(indirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(indirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(indirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(indirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(indirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
	
	cout << "Uncompress 2" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to uncompress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	// vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	// GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	// GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		// SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		// SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist;//(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist;//(insertionLengths);
	
	cout << "Uncompress 3" << endl;
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	// vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	// GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	cout << "Uncompress 4" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// read more variables
	//deletions lengths histogram
	ReadIntIntFreqMapFromFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	ReadIntIntFreqMapFromFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	FileToInts(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	FileToInts(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	FileToString(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	FileToString(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	cout << "Uncompress 5" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//prepare SNPSubstitutionValues maps
	//novel SNPs
	
	for (string::iterator it = SNPSubstitutionOldValues.begin() ; it != SNPSubstitutionOldValues.end() ; ++it)
		SNPSubstitutionStrings.insert(make_pair(*it,string("")));
	//partial match SNPs
	for (string::iterator it = SNPSubstitutionOldValuesPartialMatches.begin() ; it != SNPSubstitutionOldValuesPartialMatches.end() ; ++it)
		SNPSubstitutionStringsPartialMatches.insert(make_pair(*it,string("")));
	
	cout << "Uncompress 6" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// uncompress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//uncompress novel distances by PMF double power law for distances
	// PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	// myPMF_DPL_Compressor.Uncompress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//uncompress dbSNP distances by PMF double power law for distances
	// myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	cout << "Uncompress 6.1" << endl;
	// myPMF_DPL_Compressor.Uncompress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	cout << "Uncompress 6.2" << endl;
	
	//uncompress dbSNP match, partial match string by arithmetic coding
	if (usedbSNP)
		myArithmeticCodingWrapper.Uncompress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	cout << "Uncompress 6.3" << endl;
	//uncompress deletion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	cout << "Uncompress 6.4" << endl;
	//uncompress insertion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	cout << "Uncompress 6" << endl;
	//uncompress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Uncompress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 7" << endl;
	//uncompress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	//partial matches
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//uncompress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Uncompress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 8" << endl;
	
	// store non-distance data
	nondistanceData_.novelLabels = novelLabels;
	nondistanceData_.novelChromLengths = novelChromLengths;
	nondistanceData_.deletionLengths = deletionLengths;
	nondistanceData_.insertionLengths = insertionLengths;
	nondistanceData_.insertionSequences = insertionSequences;
	
	nondistanceData_.SNPSubstitutionStrings = SNPSubstitutionStrings;
	nondistanceData_.SNPSubstitutionStringsPartialMatches = SNPSubstitutionStringsPartialMatches;
	
	nondistanceData_.dbSNPMatchString = dbSNPMatchString;
	nondistanceData_.dbSNPChromLengths = dbSNPChromLengths;
	
	/*
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << novelDistances.size() << "," << novelLabels.size() << "," 
		 << novelChromLengths.size() << "," << deletionLengths.size() << "," << insertionLengths.size() << "," << insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, dbSNPDistances, dbSNPMatchString, dbSNPChromLengths);
	
	cout << "Uncompress 10" << endl;
	
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, dbSNPMap);
	
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, SNPSubstitutionStrings, SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// updates difference map, combines distances data and non-distances data
void Compressor::UncompressUpdateDiffMap_p1(const char *refGenomeDirectory, DifferenceMap &out_diffMap,	
	DistanceData & distanceData_, NondistanceData & nondistanceData_, bool usedbSNP)
{
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << distanceData_.novelDistances.size() << "," << nondistanceData_.novelLabels.size() << "," 
		 << nondistanceData_.novelChromLengths.size() << "," << nondistanceData_.deletionLengths.size() << "," << nondistanceData_.insertionLengths.size() << "," << nondistanceData_.insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,distanceData_.novelDistances,nondistanceData_.novelLabels,nondistanceData_.novelChromLengths,nondistanceData_.deletionLengths,nondistanceData_.insertionLengths,nondistanceData_.insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, distanceData_.dbSNPDistances, nondistanceData_.dbSNPMatchString, nondistanceData_.dbSNPChromLengths, usedbSNP);
	
	cout << "Uncompress 10" << endl;
	/*
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, chromName, dbSNPMap);
	
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, nondistanceData_.SNPSubstitutionStrings, nondistanceData_.SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// updates difference map, combines distances data and non-distances data
void Compressor::UncompressUpdateDiffMap_p2(const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, const string chromName,
	DistanceData & distanceData_, NondistanceData & nondistanceData_)
{
	/*
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << distanceData_.novelDistances.size() << "," << nondistanceData_.novelLabels.size() << "," 
		 << nondistanceData_.novelChromLengths.size() << "," << nondistanceData_.deletionLengths.size() << "," << nondistanceData_.insertionLengths.size() << "," << nondistanceData_.insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,distanceData_.novelDistances,nondistanceData_.novelLabels,nondistanceData_.novelChromLengths,nondistanceData_.deletionLengths,nondistanceData_.insertionLengths,nondistanceData_.insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, distanceData_.dbSNPDistances, nondistanceData_.dbSNPMatchString, nondistanceData_.dbSNPChromLengths);
	
	cout << "Uncompress 10" << endl;
	*/
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, chromName, dbSNPMap);
	/*
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, nondistanceData_.SNPSubstitutionStrings, nondistanceData_.SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// updates difference map, combines distances data and non-distances data
void Compressor::UncompressUpdateDiffMap_p3(const char *refGenomeDirectory, DifferenceMap &out_diffMap, 
	DistanceData & distanceData_, NondistanceData & nondistanceData_)
{
	/*
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << distanceData_.novelDistances.size() << "," << nondistanceData_.novelLabels.size() << "," 
		 << nondistanceData_.novelChromLengths.size() << "," << nondistanceData_.deletionLengths.size() << "," << nondistanceData_.insertionLengths.size() << "," << nondistanceData_.insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,distanceData_.novelDistances,nondistanceData_.novelLabels,nondistanceData_.novelChromLengths,nondistanceData_.deletionLengths,nondistanceData_.insertionLengths,nondistanceData_.insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, distanceData_.dbSNPDistances, nondistanceData_.dbSNPMatchString, nondistanceData_.dbSNPChromLengths);
	
	cout << "Uncompress 10" << endl;
	
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, chromName, dbSNPMap);
	*/
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, nondistanceData_.SNPSubstitutionStrings, nondistanceData_.SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// updates difference map, combines distances data and non-distances data
void Compressor::UncompressUpdateDiffMap(const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, const string chromName,
	DistanceData & distanceData_, NondistanceData & nondistanceData_, bool usedbSNP)
{
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << distanceData_.novelDistances.size() << "," << nondistanceData_.novelLabels.size() << "," 
		 << nondistanceData_.novelChromLengths.size() << "," << nondistanceData_.deletionLengths.size() << "," << nondistanceData_.insertionLengths.size() << "," << nondistanceData_.insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,distanceData_.novelDistances,nondistanceData_.novelLabels,nondistanceData_.novelChromLengths,nondistanceData_.deletionLengths,nondistanceData_.insertionLengths,nondistanceData_.insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, distanceData_.dbSNPDistances, nondistanceData_.dbSNPMatchString, nondistanceData_.dbSNPChromLengths,usedbSNP);
	
	cout << "Uncompress 10" << endl;
	
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, chromName, dbSNPMap);
	
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, nondistanceData_.SNPSubstitutionStrings, nondistanceData_.SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//uncompresses difference map out_diffMap from files in directory indirectory, reference genome in directory refGenomeDirectory
void Compressor::Uncompress(const char *indirectory, const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, bool usedbSNP)
{
	cout << "Uncompress 1" << endl;
	//file names
	string novelDistancesFilename(indirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(indirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(indirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(indirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(indirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(indirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(indirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will prepend A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(indirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(indirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(indirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(indirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(indirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
	
	cout << "Uncompress 2" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to uncompress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	// GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	// GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		// SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		// SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist;//(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist;//(insertionLengths);
	
	cout << "Uncompress 3" << endl;
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	// GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	cout << "Uncompress 4" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// read more variables
	//deletions lengths histogram
	ReadIntIntFreqMapFromFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	ReadIntIntFreqMapFromFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	FileToInts(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	FileToInts(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	FileToString(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	FileToString(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	cout << "Uncompress 5" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//prepare SNPSubstitutionValues maps
	//novel SNPs
	
	for (string::iterator it = SNPSubstitutionOldValues.begin() ; it != SNPSubstitutionOldValues.end() ; ++it)
		SNPSubstitutionStrings.insert(make_pair(*it,string("")));
	//partial match SNPs
	for (string::iterator it = SNPSubstitutionOldValuesPartialMatches.begin() ; it != SNPSubstitutionOldValuesPartialMatches.end() ; ++it)
		SNPSubstitutionStringsPartialMatches.insert(make_pair(*it,string("")));
	
	cout << "Uncompress 6" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// uncompress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//uncompress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	myPMF_DPL_Compressor.Uncompress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//uncompress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	cout << "Uncompress 6.1" << endl;
	if (usedbSNP)
		myPMF_DPL_Compressor.Uncompress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	cout << "Uncompress 6.2" << endl;
	
	//uncompress dbSNP match, partial match string by arithmetic coding
	if (usedbSNP)
		myArithmeticCodingWrapper.Uncompress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	cout << "Uncompress 6.3" << endl;
	//uncompress deletion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	cout << "Uncompress 6.4" << endl;
	//uncompress insertion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	cout << "Uncompress 6" << endl;
	//uncompress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Uncompress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 7" << endl;
	//uncompress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	//partial matches
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//uncompress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Uncompress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 8" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << novelDistances.size() << "," << novelLabels.size() << "," 
		 << novelChromLengths.size() << "," << deletionLengths.size() << "," << insertionLengths.size() << "," << insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, dbSNPDistances, dbSNPMatchString, dbSNPChromLengths,usedbSNP);
	
	cout << "Uncompress 10" << endl;
	
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, dbSNPMap);
	
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, SNPSubstitutionStrings, SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//uncompresses difference map out_diffMap from files in directory indirectory, reference genome in directory refGenomeDirectory
void Compressor::Uncompress_by_chrom(const char *indirectory_, const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap, DifferenceMap &out_diffMap, const string chromName, bool usedbSNP)
{
	string indirectory_string(indirectory_);
	indirectory_string += "/";
	indirectory_string += (chromName + "/");
	const char *indirectory = indirectory_string.c_str();

	cout << "Uncompress 1" << endl;
	//file names
	string novelDistancesFilename(indirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(indirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(indirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(indirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(indirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(indirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(indirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will prepend A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(indirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(indirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(indirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(indirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(indirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
	
	cout << "Uncompress 2" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to uncompress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	// GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	// GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		// SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		// SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist;//(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist;//(insertionLengths);
	
	cout << "Uncompress 3" << endl;
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	// GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	cout << "Uncompress 4" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// read more variables
	//deletions lengths histogram
	ReadIntIntFreqMapFromFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	ReadIntIntFreqMapFromFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	FileToInts(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	FileToInts(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	FileToString(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	FileToString(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	cout << "Uncompress 5" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//prepare SNPSubstitutionValues maps
	//novel SNPs
	
	for (string::iterator it = SNPSubstitutionOldValues.begin() ; it != SNPSubstitutionOldValues.end() ; ++it)
		SNPSubstitutionStrings.insert(make_pair(*it,string("")));
	//partial match SNPs
	for (string::iterator it = SNPSubstitutionOldValuesPartialMatches.begin() ; it != SNPSubstitutionOldValuesPartialMatches.end() ; ++it)
		SNPSubstitutionStringsPartialMatches.insert(make_pair(*it,string("")));
	
	cout << "Uncompress 6" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// uncompress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//uncompress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	myPMF_DPL_Compressor.Uncompress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//uncompress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	cout << "Uncompress 6.1" << endl;
	if (usedbSNP)
		myPMF_DPL_Compressor.Uncompress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	cout << "Uncompress 6.2" << endl;
	//uncompress dbSNP match, partial match string by arithmetic coding
	if (usedbSNP)
		myArithmeticCodingWrapper.Uncompress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	cout << "Uncompress 6.3" << endl;
	//uncompress deletion lengths by Huffman coding	
	cout << "dels hist size: " << delsHist.histMap.size() << endl;
	myHuffmanWrapper.Uncompress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	cout << "num deletions: " << deletionLengths.size() << endl;
	cout << "Uncompress 6.4" << endl;
	//uncompress insertion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	
	//uncompress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Uncompress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	// cout << "&&&&&&&&&&&&&&&&&&&&&&&1" << endl;
	cout << "Uncompress 7" << endl;
	//uncompress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	// cout << "&&&&&&&&&&&&&&&&&&&&&&&2" << endl;
	//partial matches
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		// cout << oldValue << endl;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	// cout << "&&&&&&&&&&&&&&&&&&&&&&&3" << endl;
	
	//uncompress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Uncompress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 8" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << novelDistances.size() << "," << novelLabels.size() << "," 
		 << novelChromLengths.size() << "," << deletionLengths.size() << "," << insertionLengths.size() << "," << insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, dbSNPDistances, dbSNPMatchString, dbSNPChromLengths,usedbSNP);
	
	cout << "Uncompress 10" << endl;
	
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	// PutReferenceSequences(out_diffMap, refGenomeDirectory, dbSNPMap);
	PutReferenceSequences(out_diffMap, refGenomeDirectory, chromName, dbSNPMap);
	
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, SNPSubstitutionStrings, SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;
}

/*
void Compressor::Uncompress_by_chrom_dbSNP(const char *indirectory, const char *refGenomeDirectory, DifferenceMap &out_diffMap)
{
	cout << "Uncompress 1" << endl;
	//file names
	string novelDistancesFilename(indirectory); novelDistancesFilename += "novelDistances.txt";
	string deletionsLengthsFilename(indirectory); deletionsLengthsFilename += "deletionsLengths.txt";
	string insertionsLengthsFilename(indirectory); insertionsLengthsFilename += "insertionsLengths.txt";
	string insertionSequenceFilename(indirectory); insertionSequenceFilename += "insertionSequence.txt";
	string novelChromLengthsFilename(indirectory); novelChromLengthsFilename += "novelChromLengths.txt";
	string novelLabelsFilename(indirectory); novelLabelsFilename += "novelLabels.txt";
	string SNPNovelValuesFilename(indirectory);  SNPNovelValuesFilename += "SNPNovelValues.txt"; //will prepend A,C,G,T initial symbol to filename
	string SNPNovelValuesFilenameUniqueOldValuesMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesMatch += "SNPNovelValuesUniqueOldValuesMatch.txt";
	string SNPNovelValuesFilenameUniqueOldValuesPartialMatch(indirectory); SNPNovelValuesFilenameUniqueOldValuesPartialMatch += "SNPNovelValuesUniqueOldValuesPartialMatch.txt";
	
	string dbSNPDistancesFilename(indirectory); dbSNPDistancesFilename += "dbSNPDistances.txt";
	string dbSNPChromLengthsFilename(indirectory); dbSNPChromLengthsFilename += "dbSNPChromLengths.txt";
	string dbSNPMatchesAndPartialMatchesFilename(indirectory); dbSNPMatchesAndPartialMatchesFilename += "dbSNPMatchesAndPartialMatches.txt";
	
	string deletionsLengthsHistFilename(indirectory); deletionsLengthsHistFilename += "deletionsLengthsHist.txt";
	string insertionsLengthsHistFilename(indirectory); insertionsLengthsHistFilename += "insertionsLengthsHist.txt";
	
	cout << "Uncompress 2" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// prepare objects to uncompress
	//novel
	////////////////////////////////////////////
	//prepare vector of distances
	vector<int> novelDistances;
	string novelLabels; //0 - SNP, 1 - deletion, 2 - insertion
	vector<int> novelChromLengths; //chromosome lengths in number of novel variations
	vector<int> deletionLengths; //deletion lengths
	vector<int> insertionLengths; //insertion lengths
	string insertionSequences; //insertion sequences concatenated
	// GetNovelDistances(in_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	//get SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
	map<char,string> SNPSubstitutionStrings; //A, C, G, (T and others)
	map<char,string> SNPSubstitutionStringsPartialMatches;
	// GetSNPSubstitutionValues(in_diffMap,SNPSubstitutionStrings,SNPSubstitutionStringsPartialMatches);
	string SNPSubstitutionOldValues; //list of unique SNP values in reference sequence, novel SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
		// SNPSubstitutionOldValues.push_back(it->first);
	string SNPSubstitutionOldValuesPartialMatches; //list of unique SNP values in reference sequence, partial match SNPs
	// for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
		// SNPSubstitutionOldValuesPartialMatches.push_back(it->first);
	
	//get deletion lengths histogram
	Hist<int> delsHist;//(deletionLengths);
	
	//get insertion lengths histogram
	Hist<int> insHist;//(insertionLengths);
	
	cout << "Uncompress 3" << endl;
	//dbSNP
	////////////////////////////////////////////
	//prepare vector of distances between indices in dbSNP, binary string for each match, partial match in dbSNP, 1 means partial match, 0 match
	vector<int> dbSNPDistances;
	string dbSNPMatchString;
	vector<int> dbSNPChromLengths; //chromosome lengths in number of novel variations
	// GetdbSNPDistancesAndPartialMatchVector(in_diffMap,dbSNPDistances,dbSNPMatchString,dbSNPChromLengths);
	
	cout << "Uncompress 4" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// read more variables
	//deletions lengths histogram
	ReadIntIntFreqMapFromFile(delsHist.histMap,deletionsLengthsHistFilename.c_str());
	
	//insertions lengths histogram
	ReadIntIntFreqMapFromFile(insHist.histMap,insertionsLengthsHistFilename.c_str());
	
	//novel variations chromosome lengths in units of number of novel variations
	FileToInts(novelChromLengths,novelChromLengthsFilename.c_str());
	
	//dbSNP chromosome lengths in units of number of valid variations
	FileToInts(dbSNPChromLengths,dbSNPChromLengthsFilename.c_str());
	
	//list of unique SNP values in reference sequence, novel SNPs
	FileToString(SNPSubstitutionOldValues,SNPNovelValuesFilenameUniqueOldValuesMatch.c_str());
	
	//list of unique SNP values in reference sequence, partial match SNPs
	FileToString(SNPSubstitutionOldValuesPartialMatches,SNPNovelValuesFilenameUniqueOldValuesPartialMatch.c_str());
	
	cout << "Uncompress 5" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//prepare SNPSubstitutionValues maps
	//novel SNPs
	
	for (string::iterator it = SNPSubstitutionOldValues.begin() ; it != SNPSubstitutionOldValues.end() ; ++it)
		SNPSubstitutionStrings.insert(make_pair(*it,string("")));
	//partial match SNPs
	for (string::iterator it = SNPSubstitutionOldValuesPartialMatches.begin() ; it != SNPSubstitutionOldValuesPartialMatches.end() ; ++it)
		SNPSubstitutionStringsPartialMatches.insert(make_pair(*it,string("")));
	
	cout << "Uncompress 6" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// uncompress
	
	//initialize Huffman coding, arithmetic coding wrappers
	HuffmanWrapper<int,int> myHuffmanWrapper;
	ArithmeticCodingWrapper myArithmeticCodingWrapper;
	
	//uncompress novel distances by PMF double power law for distances
	PMF_DPL_Compressor myPMF_DPL_Compressor(novelDistances_PMF_DPL_parameters); //PMF double power law compressor
	cout << "table...";
	myPMF_DPL_Compressor.Uncompress(novelDistances,1,100000,novelDistancesFilename.c_str());
	cout << "done" << endl;
	
	//uncompress dbSNP distances by PMF double power law for distances
	myPMF_DPL_Compressor.SetParameters(dbSNPDistances_PMF_DPL_parameters);
	myPMF_DPL_Compressor.Uncompress(dbSNPDistances,1,100000,dbSNPDistancesFilename.c_str());
	
	//uncompress dbSNP match, partial match string by arithmetic coding
	myArithmeticCodingWrapper.Uncompress(dbSNPMatchString,dbSNPMatchesAndPartialMatchesFilename.c_str());
	
	//uncompress deletion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(delsHist.histMap,deletionLengths,deletionsLengthsFilename.c_str());
	
	//uncompress insertion lengths by Huffman coding	
	myHuffmanWrapper.Uncompress(insHist.histMap,insertionLengths,insertionsLengthsFilename.c_str());
	
	//uncompress novel labels string by arithmetic coding
	cout << "novel labels...";
	myArithmeticCodingWrapper.Uncompress(novelLabels,novelLabelsFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 7" << endl;
	//uncompress SNP substitution sequences conditional on previous values by arithmetic coding
	//matches
	for (map<char,string>::iterator it = SNPSubstitutionStrings.begin() ; it != SNPSubstitutionStrings.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".match.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	//partial matches
	for (map<char,string>::iterator it = SNPSubstitutionStringsPartialMatches.begin() ; it != SNPSubstitutionStringsPartialMatches.end() ; ++it)
	{
		char oldValue = it->first;
		string filename(1,oldValue); filename = SNPNovelValuesFilename + (".partiaMatch.from" + filename);
		cout << oldValue << " SNP values...";
		myArithmeticCodingWrapper.Uncompress(it->second,filename.c_str()); cout << "done" << endl;
	}
	
	//uncompress insertion sequence string by arithmetic coding
	cout << "insertion sequence...";
	myArithmeticCodingWrapper.Uncompress(insertionSequences,insertionSequenceFilename.c_str()); cout << "done" << endl;
	
	cout << "Uncompress 8" << endl;
	////////////////////////////////////////////////////////////////////////////////////////
	//construct difference map
	cout << "sizes: " << novelDistances.size() << "," << novelLabels.size() << "," 
		 << novelChromLengths.size() << "," << deletionLengths.size() << "," << insertionLengths.size() << "," << insertionSequences.size() << endl;
	
	//put offsets for all entries into var map
	PutNovelDistances(out_diffMap,novelDistances,novelLabels,novelChromLengths,deletionLengths,insertionLengths,insertionSequences);
	
	cout << "Uncompress 9" << endl;
	
	//put offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
	PutdbSNPDistancesAndPartialMatchVector(out_diffMap, dbSNPDistances, dbSNPMatchString, dbSNPChromLengths);
	
	cout << "Uncompress 10" << endl;
}

void Compressor::Uncompress_by_chrom_dbSNP(const VarMapdbSNP &dbSNPMap, const string chromName)
{
	//get reference values
	//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
	PutReferenceSequences(out_diffMap, refGenomeDirectory, chromName, dbSNPMap);
	
	cout << "Uncompress 11" << endl;
	
	//put SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
	PutSNPSubstitutionValues(out_diffMap, SNPSubstitutionStrings, SNPSubstitutionStringsPartialMatches);
	
	cout << "Uncompress 12" << endl;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//put reference values into DifferenceMap out_diffMap, give directory where reference genome stored, dbSNPMap to look up positions of partial matches in reference genome
void Compressor::PutReferenceSequences(DifferenceMap & diffMap, const char *refGenomeDirectory, const VarMapdbSNP &dbSNPMap)
{
	cout << "here1" << endl;

	// string refSeq; // reference sequence
	string chromName; // current chromosome name
	string fileNameCurrentChromosomeRefSeq; // file name of reference sequence of current chromosome
	varType_ currentVarType; // current variation type
	int delLength; // current deletion length
	int partialMatchPosition; // position of current partial match
	// for (int chromIndex = 1; chromIndex < 2 ; ++chromIndex)
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		// load reference sequence
		chromName = myChromNames.IndexToChromName(chromIndex);
		fileNameCurrentChromosomeRefSeq = refGenomeDirectory;
		fileNameCurrentChromosomeRefSeq += (chromName + ".fa");
		cout << "chrom " << fileNameCurrentChromosomeRefSeq << /*" here1" << */endl;
		string refSeq; // reference sequence
		FileToString(refSeq,fileNameCurrentChromosomeRefSeq.c_str(),1); //skip first line, contains [chromName]>
		// StringToFile(refSeq,"temp.txt");
		
		
		// cout << "chrom " << fileNameCurrentChromosomeRefSeq << " here2" << endl;
		
		// cout << "here3" << endl;
		cout << "chrom length: " << refSeq.size() << endl;
		
		// novel changes
		for (vector<Var>::iterator it = diffMap.varMap[chromIndex].begin() ; it != diffMap.varMap[chromIndex].end() ; ++it)
		{
			currentVarType = it->GetvarType();
			
			if (currentVarType == SNP_)
			{
				it->SetOldSequence(Capitalize(refSeq.substr((it->GetPosition())-1,1)));
				
				// cout << "pos: " << it->GetPosition() << ", seq: " << refSeq.substr(it->GetPosition(),1);
				// int ii;
				// cin >> ii;
				
			} else if ((currentVarType == DEL_) || (currentVarType == INDEL_))
			{
				delLength = it->GetOldSequence().size(); // deletion length
				it->SetOldSequence(Capitalize(refSeq.substr((it->GetPosition())-1,delLength))); // catch over run end of current chromosome reference sequence?
			} else if (currentVarType == INS_)
			{
			} else
				cout << "not valid var type" << endl;
			// cout << "pos: " << it->GetPosition() << endl;
		}
		
		// cout << "here4" << endl;
				
		// partial matches, distances match, get sequences from reference sequence
		vector<int>::iterator indices_it = diffMap.partialMatchIndices[chromIndex].begin();
		vector<string>::iterator seqs_it = diffMap.partialMatchOldSequences[chromIndex].begin();
		for ( ; 
			indices_it != diffMap.partialMatchIndices[chromIndex].end(); 
			++indices_it, ++seqs_it)
		{
			//partial matches only SNPs, change this?
			// get position of partial match from dbSNP
			partialMatchPosition = dbSNPMap.varMap[chromIndex][*indices_it].GetPosition()-1;
			// get sequence
			*seqs_it = Capitalize(refSeq.substr(partialMatchPosition,1)); //SNP
		}
		
		// cout << "here5" << endl;
	}
}

void Compressor::PutReferenceSequences(DifferenceMap & diffMap, const char *refGenomeDirectory, const string chromName_, const VarMapdbSNP &dbSNPMap)
{
	cout << "here1" << endl;

	// string refSeq; // reference sequence
	string chromName; // current chromosome name
	string fileNameCurrentChromosomeRefSeq; // file name of reference sequence of current chromosome
	varType_ currentVarType; // current variation type
	int delLength; // current deletion length
	int partialMatchPosition; // position of current partial match
	
	int chromIndex = myChromNames.ChromNameToIndex(chromName_);
	// for (int chromIndex = 1; chromIndex < 2 ; ++chromIndex)
	// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	// {
		// load reference sequence
		chromName = myChromNames.IndexToChromName(chromIndex);
		fileNameCurrentChromosomeRefSeq = refGenomeDirectory;
		fileNameCurrentChromosomeRefSeq += (chromName + ".fa");
		cout << "chrom " << fileNameCurrentChromosomeRefSeq << /*" here1" << */endl;
		string refSeq; // reference sequence
		FileToString(refSeq,fileNameCurrentChromosomeRefSeq.c_str(),1); //skip first line, contains [chromName]>
		// StringToFile(refSeq,"temp.txt");
		
		
		// cout << "chrom " << fileNameCurrentChromosomeRefSeq << " here2" << endl;
		
		// cout << "here3" << endl;
		cout << "chrom length: " << refSeq.size() << endl;
		
		// novel changes
		for (vector<Var>::iterator it = diffMap.varMap[chromIndex].begin() ; it != diffMap.varMap[chromIndex].end() ; ++it)
		{
			currentVarType = it->GetvarType();
			
			if (currentVarType == SNP_)
			{
				it->SetOldSequence(Capitalize(refSeq.substr((it->GetPosition())-1,1)));
				
				// cout << "pos: " << it->GetPosition() << ", seq: " << refSeq.substr(it->GetPosition(),1);
				// int ii;
				// cin >> ii;
				
			} else if ((currentVarType == DEL_) || (currentVarType == INDEL_))
			{
				delLength = it->GetOldSequence().size(); // deletion length
				it->SetOldSequence(Capitalize(refSeq.substr((it->GetPosition())-1,delLength))); // catch over run end of current chromosome reference sequence?
			} else if (currentVarType == INS_)
			{
			} else
				cout << "not valid var type" << endl;
			// cout << "pos: " << it->GetPosition() << endl;
		}
		
		// cout << "here4" << endl;
				
		// partial matches, distances match, get sequences from reference sequence
		vector<int>::iterator indices_it = diffMap.partialMatchIndices[chromIndex].begin();
		vector<string>::iterator seqs_it = diffMap.partialMatchOldSequences[chromIndex].begin();
		for ( ; 
			indices_it != diffMap.partialMatchIndices[chromIndex].end(); 
			++indices_it, ++seqs_it)
		{
			//partial matches only SNPs, change this?
			// get position of partial match from dbSNP
			partialMatchPosition = dbSNPMap.varMap[chromIndex][*indices_it].GetPosition()-1;
			// get sequence
			*seqs_it = Capitalize(refSeq.substr(partialMatchPosition,1)); //SNP
		}
		
		// cout << "here5" << endl;
	// }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//puts offsets for all entries into var map
void Compressor::PutNovelDistances(
	VarMap & varMap, const vector<int> &novelDistances, const string &novelLabels, const vector<int> &chromLengths,
	const vector<int> &deletionLengths, const vector<int> &insertionLengths, const string &insertionSequences)
{
	int currentPos; //current variation position
	int currentChromLength; //current chromosome length
	int currentChromNum = 0; //current chromosome number
	
	int varNum = 0; //number of current variation read
	int delNum = 0; //number of current deletion read
	int insNum = 0; //number of current insertion read
	
	int insSequenceLengthOffset = 0; //starting point to read current insertion sequence
	
	varType_ currentVarType; //current variation type
	
	int delLength, insLength; //deletion, insertion lengths
	string oldSeq, newSeq; //reference and new variation sequences
	
	// Var insVar = Var(-1,SNP_,"Z","Z"); // used to insert in variations map
	
	//clear varMap
	for (vector< vector<Var> >::iterator it = varMap.varMap.begin(); it != varMap.varMap.end() ; ++it)
		it->clear();
	//fill varMap
	for (vector<int>::const_iterator chromLengths_it = chromLengths.begin() ; chromLengths_it != chromLengths.end() ; ++chromLengths_it)
	{
		// cout << "chrom: " << currentChromNum << endl;
	
		currentPos = 0;
		currentChromLength = *chromLengths_it;
		
		// cout << "chrom length: " << currentChromLength << endl;
		
		for (int i = 0; i < currentChromLength ; ++i)
		{
			//cout << "i1: " << i << endl;
			
			currentPos += novelDistances[varNum];
			currentVarType = (varType_)novelLabels[varNum];
			
			//cout << "i2: " << i << endl;
			
			if (currentVarType == SNP_)
			{	//cout << "i3snp: " << i << endl;	
				oldSeq = "Z"; newSeq = "Z";
			}
			else if (currentVarType == DEL_)
			{	//cout << "i3del: " << i << endl;
				delLength = deletionLengths[delNum];
				oldSeq = string(delLength,'Z');
				newSeq = string(delLength,'-');
				++delNum;
			} else if (currentVarType == INS_)
			{	//cout << "i3ins: " << i << endl;
				insLength = insertionLengths[insNum];
				oldSeq = string(insLength,'-');
				newSeq = insertionSequences.substr(insSequenceLengthOffset,insLength);
				insSequenceLengthOffset += insLength;
				++insNum;
			} else if (currentVarType == INDEL_)
			{
				delLength = deletionLengths[delNum];
				oldSeq = string(delLength,'Z');
				++delNum;
				
				insLength = insertionLengths[insNum];
				newSeq = insertionSequences.substr(insSequenceLengthOffset,insLength);
				insSequenceLengthOffset += insLength;
				++insNum;
			} else
				cout << "not valid var type" << endl;
			//cout << "i4: " << i << endl;
			varMap.varMap[currentChromNum].push_back(Var(currentPos,currentVarType,oldSeq,newSeq));
			
			//cout << "i5: " << i << endl;
			++varNum;
			//cout << "i6: " << i << endl;
		}
			
		++currentChromNum;
	}

	// for (int pos = 0; pos < chrom
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gets offsets for all entries in var map, string novelLabels - 0 - SNP, 1 - deletion, 2 - insertion, 
//chromLengths chromosome lengths in number of novel variations
void Compressor::GetNovelDistances(
	const VarMap & varMap, vector<int> &novelDistances, string &novelLabels, vector<int> &chromLengths, 
	vector<int> &deletionLengths, vector<int> &insertionLengths, string &insertionSequences) 
{
	int writePos = 0; //place to write new entry
	int prevChromWritePosEnd = 0; //previous chromosome writePos value when done reading chromosome
	int prevPos = 0; //previous variation position
	int insertionLength; //insertion length
	string insertionSequence; //insertion sequence
	novelLabels = string("");
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		prevPos = 0; //previous variation position
		for (vector<Var>::const_iterator it = varMap.varMap[chromIndex].begin(); it != varMap.varMap[chromIndex].end() ; ++it)
		{
				novelDistances.push_back(it->GetPosition() - prevPos); //write offset
				prevPos = it->GetPosition();
				novelLabels.push_back((char)(it->GetvarType()));
				
				if (it->IsDel()) //deletion
					deletionLengths.push_back(it->GetOldSequence().size()); //write deletion length
				else if (it->IsIns()) //insertion
				{
					insertionSequence = it->GetNewSequence();
					insertionLength = insertionSequence.size();
					insertionLengths.push_back(insertionLength); //write insertion length
					for (int i = 0; i < insertionLength ; ++i)
						insertionSequences.push_back(insertionSequence[i]);
				} else if (it->IsIndel()) // indel
				{
					deletionLengths.push_back(it->GetOldSequence().size()); //write deletion length
					
					insertionSequence = it->GetNewSequence();
					insertionLength = insertionSequence.size();
					insertionLengths.push_back(insertionLength); //write insertion length
					for (int i = 0; i < insertionLength ; ++i)
						insertionSequences.push_back(insertionSequence[i]);
				}
				// novelDistances[writePos] = (it->GetPosition() - (it-1)->GetPosition()); //write offset
				// novelLabels[writePos] = (char)(it->GetvarType());
				++writePos;
		}
		chromLengths.push_back(writePos - prevChromWritePosEnd);
		prevChromWritePosEnd = writePos;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//puts offsets for dbSNP matches into DifferenceMap diffMap, partial matches, gets match, partial match string, 1 means partial match, 0 match
void Compressor::PutdbSNPDistancesAndPartialMatchVector(
	DifferenceMap & diffMap, const vector<int> &dbSNPDistances, const string &dbSNPMatchString, const vector<int> &chromLengths, bool usedbSNP)
{
	int currentPos; //current variation position
	int currentChromLength; //current chromosome length
	int currentChromNum = 0; //current chromosome number
	
	int varNum = 0; //number of variation read
	
	//clear DifferenceMap diffMap
	diffMap.matchIndices.resize(chromLengths.size());
	diffMap.partialMatchIndices.resize(chromLengths.size());
	diffMap.partialMatchVarTypes.resize(chromLengths.size());
	diffMap.partialMatchOldSequences.resize(chromLengths.size());
	diffMap.partialMatchNewSequences.resize(chromLengths.size());
	for (int chromNum = 0 ; chromNum < chromLengths.size() ; ++chromNum)
	{
		diffMap.matchIndices[chromNum].clear();
		diffMap.partialMatchIndices[chromNum].clear();
		diffMap.partialMatchVarTypes[chromNum].clear();
		diffMap.partialMatchOldSequences[chromNum].clear();
		diffMap.partialMatchNewSequences[chromNum].clear();
	}
	
	if (usedbSNP)
	{
		//iterate over chromosomes
		// for (vector< vector<Var> >::iterator it = diffMap.varMap.begin() ; it != diffMap.varMap.end() ; ++it)
		for (int chromNum = 0 ; chromNum < chromLengths.size() ; ++chromNum)
		{
			currentPos = 0;
			
			currentChromLength = chromLengths[chromNum];		
			for (int i = 0 ; i < currentChromLength ; ++i)
			{
				currentPos += dbSNPDistances[varNum];
				if (dbSNPMatchString[varNum] == '0') // match
					diffMap.matchIndices[chromNum].push_back(currentPos);
				else if (dbSNPMatchString[varNum] == '1') // partial match
				{
					diffMap.partialMatchIndices[chromNum].push_back(currentPos);
					diffMap.partialMatchVarTypes[chromNum].push_back(SNP_);
					diffMap.partialMatchOldSequences[chromNum].push_back("Z");
					diffMap.partialMatchNewSequences[chromNum].push_back("Z");
				} else
					cout << "not valid dbSNP match type" << endl;
				
				++varNum;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gets offsets for dbSNP matches, partial matches, gets match, partial match string, 1 means partial match, 0 match, 
//chromLengths chromosome lengths in number of matching or partially matching variations
void Compressor::GetdbSNPDistancesAndPartialMatchVector(
	const DifferenceMap & diffMap, vector<int> &dbSNPDistances, string &dbSNPMatchString, vector<int> &chromLengths) 
{
	//count number of matche and partial matches
	int numMatchesAndPartialMatches = 0;
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
		numMatchesAndPartialMatches += (diffMap.matchIndices[chromIndex].size() + diffMap.partialMatchIndices[chromIndex].size());
	
	int writePos = 0; //place to write new entry
	int prevChromWritePosEnd = 0; //previous chromosome writePos value when done reading chromosome
	dbSNPDistances.resize(numMatchesAndPartialMatches);
	// cout << "#: " << numMatchesAndPartialMatches << endl;
	dbSNPMatchString = string(numMatchesAndPartialMatches,'Z'); //initialize string
	// cout << "str size2	: " << dbSNPMatchString.size() << endl;
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		vector<int>::const_iterator matches_it = diffMap.matchIndices[chromIndex].begin();
		vector<int>::const_iterator partialMatches_it = diffMap.partialMatchIndices[chromIndex].begin();

		int prevPos = 0; //previous position
		
		while ((matches_it != diffMap.matchIndices[chromIndex].end()) && (partialMatches_it != diffMap.partialMatchIndices[chromIndex].end()))
		{
			if ((*matches_it) < (*partialMatches_it))
			{
				dbSNPDistances[writePos] = *matches_it - prevPos;
				dbSNPMatchString[writePos] = '0';
				prevPos = *matches_it;
				++matches_it;
			} else
			{
				dbSNPDistances[writePos] = *partialMatches_it - prevPos;
				dbSNPMatchString[writePos] = '1';
				prevPos = *partialMatches_it;
				++partialMatches_it;
			}
			++writePos;
		}
		//write remaining matches past last partial match
		while (matches_it != diffMap.matchIndices[chromIndex].end())
		{	dbSNPDistances[writePos] = *matches_it - prevPos; 
			dbSNPMatchString[writePos] = '0';
			prevPos = *matches_it;
			++matches_it;
			++writePos;
		}
		//write remaining partial matches past last match
		while (partialMatches_it != diffMap.partialMatchIndices[chromIndex].end())
		{	dbSNPDistances[writePos] = *partialMatches_it - prevPos;
			dbSNPMatchString[writePos] = '1';
			prevPos = *partialMatches_it;
			++partialMatches_it;
			++writePos;
		}
		
		chromLengths.push_back(writePos - prevChromWritePosEnd);
		prevChromWritePosEnd = writePos;
	}
	
	
	/*
	vector<int> mergedMatchesAndPartialMatches; //vector of merged matches, partial matches positions
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		mergedMatchesAndPartialMatches.resize(diffMap.matchIndices[chromIndex].size() + diffMap.partialMatchIndices[chromIndex].size());
		//merge matches, partial matches
		merge(diffMap.matchIndices[chromIndex].begin(),       diffMap.matchIndices[chromIndex].end(),
			  diffMap.partialMatchIndices[chromIndex].begin(),diffMap.partialMatchIndices[chromIndex].end(),
			  mergedMatchesAndPartialMatches.begin());
		// for (int i = 0; i < mergedMatchesAndPartialMatches.size() ; ++i)
			// dbSNPDistances[
	}	*/
	
	// map<string,void *> y;
	// void *p;
	// int x;
	// string z("hello");
	// p = &x;
	// y.insert(make_pair("here",&x));
	// p = &z;
	// y.insert(make_pair("now",&z));
	
	// vector<void*> x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//puts SNP substitution values conditioned on value in reference sequence into DifferenceMap diffMap, includes partial match SNP values
//assumes reference values loaded into difference map diffMap
void Compressor::PutSNPSubstitutionValues(
	DifferenceMap &diffMap, const map<char,string> &SNPSubstitutionStrings, const map<char,string> &SNPSubstitutionStringsPartialMatches)
{
	map<char,string>::const_iterator map_it;
	
	//show map
	for (map_it = SNPSubstitutionStrings.begin(); map_it != SNPSubstitutionStrings.end(); ++map_it)
	{
		cout << "char: " << map_it->first << endl;
		cout << "string length: " << (map_it->second).length() << endl;
	}
	
	for (map_it = SNPSubstitutionStringsPartialMatches.begin() ; map_it != SNPSubstitutionStringsPartialMatches.end() ; ++map_it)
	{
		cout << "char: " << map_it->first << endl;
		cout << "string length: " << (map_it->second).length() << endl;
		cout << "string: " << map_it->second << endl;
	}
	
	char oldChar; // character in reference sequence
	char newChar; // character in new sequence
	
	//prepare string iterators, one for each initial reference sequence value
	map<char,string::const_iterator> SNPmap_iterators;
	for (map_it = SNPSubstitutionStrings.begin() ; map_it != SNPSubstitutionStrings.end() ; ++map_it)
		SNPmap_iterators.insert(make_pair(map_it->first,map_it->second.begin()));
	map<char,string::const_iterator> SNPPartialMatchesmap_iterators;
	for (map_it = SNPSubstitutionStringsPartialMatches.begin() ; map_it != SNPSubstitutionStringsPartialMatches.end() ; ++map_it)
		SNPPartialMatchesmap_iterators.insert(make_pair(map_it->first,map_it->second.begin()));
		
	map<char,string::const_iterator>::iterator iterator_map_it;
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		// novel changes
		for (vector<Var>::iterator it = diffMap.varMap[chromIndex].begin() ; it != diffMap.varMap[chromIndex].end() ; ++it)
		{
			if (diffMap.varMap[chromIndex].begin() == it)
				cout << "novel, " << myChromNames.IndexToChromName(chromIndex) << endl;
			// it->Show();
			if (it->IsSNP()) // SNP
			{
				oldChar = Capitalize((it->GetOldSequence())[0]); // character in reference sequence
				
				// if ((oldChar == 'R') || (oldChar == 'M') || (oldChar == 'N')) // treat 'R', 'M', 'N' as 'T'
				if (!(('A' == oldChar) || ('C' == oldChar) || ('G' == oldChar) || ('T' == oldChar)))
					oldChar = 'T';
				// cout << "old char: " << oldChar << endl;
				// map_it = SNPSubstitutionStrings.find(oldChar);
				iterator_map_it = SNPmap_iterators.find(oldChar);
				// cout << "at end: " << (SNPmap_iterators.end() == iterator_map_it) << endl;
				newChar = *(iterator_map_it->second); // character in new sequence
				// cout << "new char: " << newChar << endl;
				it->SetNewSequence(string(1,newChar));
				// cout << "here" << endl;
				// advance string iterator
				++(iterator_map_it->second);
			}
		}
		// partial matches
		int numPartialMatch = 0; // number of partial match in current chromosome
		for (vector<int>::iterator it = diffMap.partialMatchIndices[chromIndex].begin() ; it != diffMap.partialMatchIndices[chromIndex].end() ; ++it)
		{
			if (diffMap.partialMatchIndices[chromIndex].begin() == it)
				cout << "novel, " << myChromNames.IndexToChromName(chromIndex) << endl;
		
			//check var type of match
			// if (it->IsSNP()) // SNP
			// {
				oldChar = Capitalize(diffMap.partialMatchOldSequences[chromIndex][numPartialMatch][0]); // character in reference sequence
				
				// if ((oldChar == 'R') || (oldChar == 'M') || (oldChar == 'N')) // treat 'R', 'M', 'N' as 'T'
				if (!(('A' == oldChar) || ('C' == oldChar) || ('G' == oldChar) || ('T' == oldChar)))
					oldChar = 'T';
				
				// map_it = SNPSubstitutionStrings.find(oldChar);
				iterator_map_it = SNPPartialMatchesmap_iterators.find(oldChar);
				
				newChar = *(iterator_map_it->second); // character in new sequence
				// it->SetNewSequence(string(1,newChar));
				diffMap.partialMatchNewSequences[chromIndex][numPartialMatch] = string(1,newChar);
				
				++numPartialMatch;
				// advance string iterator
				++(iterator_map_it->second);
			// }
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//gets SNP substitution values conditioned on value in reference sequence, includes partial match SNP values
void Compressor::GetSNPSubstitutionValues(
	const DifferenceMap &diffMap, map<char,string> &SNPSubstitutionStrings, map<char,string> &SNPSubstitutionStringsPartialMatches)
{
	// map<char,vector<char> > SNPSubstitutionVectors; //A, C, G, (T and others)
	// map<char,vector<char> > SNPSubstitutionVectorsPartialMatches; //A, C, G, (T and others)
	// vector<char> emptyVec; //initial empty vector to insert in map when found new reference sequence character
	
	map< char,string >::iterator map_it;
	char oldChar; //character in reference sequence
	char newChar; //character in new sequence
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		//novel SNPs
		for (vector<Var>::const_iterator vec_it = diffMap.varMap[chromIndex].begin() ; vec_it != diffMap.varMap[chromIndex].end() ; ++vec_it)
		{
			// if (IsSNP(*vec_it))
			if (vec_it->IsSNP())
			{
				oldChar = Capitalize(vec_it->GetOldSequence()[0]); //character in reference sequence
				
				// if ((oldChar == 'R') || (oldChar == 'M') || (oldChar == 'N')) // treat 'R', 'M', 'N' as 'T'
				if (!(('A' == oldChar) || ('C' == oldChar) || ('G' == oldChar) || ('T' == oldChar)))
					oldChar = 'T';
					
				// if ((oldChar == 'R') || (oldChar == 'M') || (oldChar == 'N')) // treat 'R', 'M', 'N' as 'T'
					// oldChar = 'T';
				
				newChar = vec_it->GetNewSequence()[0]; //character in new sequence
				
				map_it = SNPSubstitutionStrings.find(oldChar);
				if (map_it == SNPSubstitutionStrings.end()) //new value, add to map, empty vector
				{	
					SNPSubstitutionStrings.insert(make_pair(oldChar,string()));
					map_it = SNPSubstitutionStrings.find(oldChar); //get pointer to newly inserted character in reference sequence
					
					
				}
				//extend SNP substitution sequence
				map_it->second.push_back(newChar);
			}
		}
		//partial matches
		// string::const_iterator st_it;
		for (int i = 0 ; i < diffMap.partialMatchOldSequences[chromIndex].size() ; ++i)
		{			
			oldChar = Capitalize(diffMap.partialMatchOldSequences[chromIndex][i][0]);
			
			// if ((oldChar == 'R') || (oldChar == 'M') || (oldChar == 'N')) // treat 'R', 'M', 'N' as 'T'
			if (!(('A' == oldChar) || ('C' == oldChar) || ('G' == oldChar) || ('T' == oldChar)))
				oldChar = 'T';
				
			// if ((oldChar == 'R') || (oldChar == 'M') || (oldChar == 'N')) // treat 'R', 'M', 'N' as 'T'
				// oldChar = 'T';
			
			newChar = diffMap.partialMatchNewSequences[chromIndex][i][0];
			
			map_it = SNPSubstitutionStringsPartialMatches.find(oldChar);
			if (map_it == SNPSubstitutionStringsPartialMatches.end()) //new value, add to map, empty vector
			{
				SNPSubstitutionStringsPartialMatches.insert(make_pair(oldChar,string()));
				map_it = SNPSubstitutionStringsPartialMatches.find(oldChar); //get pointer to newly inserted character in reference sequence
				
				// cout << "found new char_: " << oldChar << endl;
				// cout << "added_: " << newChar << endl;
				// int ii;
				// cin >> ii;
			}
			//extend SNP substitution sequence
			map_it->second.push_back(newChar);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//writes frequency map to file, writes two files, one for frequencies, one for unique values
void Compressor::WriteIntIntFreqMapToFile(const map<int,int> &freqMap, const char *filename)
{
	string uniqueValuesFilename(filename); uniqueValuesFilename += ".map_uniqueValues"; //"map_uniqueValues_" + uniqueValuesFilename;
	string countsFilename(filename); countsFilename += ".map_counts"; //"map_counts_" + countsFilename;
	
	ofstream destUniqueValues(uniqueValuesFilename.c_str()); //stores unique values
	ofstream destCounts(countsFilename.c_str()); //stores counts in map
		
	for (map<int,int>::const_iterator it = freqMap.begin(); it != freqMap.end(); ++it)
	{
		//write unique value
		destUniqueValues << it->first << endl;
		//write frequency value, count
		destCounts << it->second << endl;
	}
	
	//close files
	destUniqueValues.close();
	destCounts.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reads frequency map from file, reads from two files, one for frequencies, one for unique values
void Compressor::ReadIntIntFreqMapFromFile(map<int,int> &freqMap, const char *filename)
{
	string uniqueValuesFilename(filename); uniqueValuesFilename += ".map_uniqueValues"; //"map_uniqueValues_" + uniqueValuesFilename;
	string countsFilename(filename); countsFilename += ".map_counts"; //"map_counts_" + countsFilename;
	
	//read ints from files
	vector<int> uniqueValues; //unique values
	vector<int> freqCounts; //frquency values, count
	FileToInts(uniqueValues,uniqueValuesFilename.c_str());
	FileToInts(freqCounts,countsFilename.c_str());
	
	//make map
	//clear map
	freqMap.clear();
	
	vector<int>::iterator uniqueValues_it = uniqueValues.begin();
	vector<int>::iterator freqCounts_it = freqCounts.begin();
	for ( ; uniqueValues_it != uniqueValues.end(); ++uniqueValues_it , ++freqCounts_it)
		freqMap.insert(make_pair(*uniqueValues_it,*freqCounts_it));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// capitalizes character
char Compressor::Capitalize(const char &charToCapitalize)
{
	char capitalized = charToCapitalize;
	switch (charToCapitalize)
	{
		case 't':
			capitalized = 'T';
			break;
		case 'a':
			capitalized = 'A';
			break;
		case 'g':
			capitalized = 'G';
			break;
		case 'c':
			capitalized = 'C';
			break;
		case 'n':
			capitalized = 'N';
			break;
	}
	
	return capitalized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// capitalizes string
string Compressor::Capitalize(const string &stringToCapitalize)
{
	string capitalized = stringToCapitalize;
	for (int i = 0 ; i < stringToCapitalize.length() ; ++i)
	{
		switch (stringToCapitalize[i])
		{
			case 't':
				capitalized[i] = 'T';
				break;
			case 'a':
				capitalized[i] = 'A';
				break;
			case 'g':
				capitalized[i] = 'G';
				break;
			case 'c':
				capitalized[i] = 'C';
				break;
			case 'n':
				capitalized[i] = 'N';
				break;
		}
	}
	
	return capitalized;
}

////////////////////////////////////
//free functions
////////////////////////////////////
