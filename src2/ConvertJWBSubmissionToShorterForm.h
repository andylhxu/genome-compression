#include <iostream>
#include <fstream>
#include <regex.h>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

void ConvertJWBSubmissionToShortForm(char *infileSNPs, char *infileInDels, char *outfile)
{
	//input and output stream
	ifstream sourceSNPs(infileSNPs);
	ifstream sourceInDels(infileInDels);
	ofstream dest(outfile);	
	
	string line; //line to load
	string lineToWrite; //line to write
		
	int numVariations = 0; //number of SNPS, deletions, insertions	
	string positionToken; //position string
	string prevPositionToken;  //previous position string
	char *valueToken; //pointer to token string, value string, line split into tokens
	
	char variationType = '0'; //0 - SNP, 1 - deletion, 2 - insertion
	bool switchVariationTypeOnThisIteration = false; //true if variation type switched on this iteration
		
	//SNPs/////////////////////////////		
	variationType = '0'; //SNP
	getline(sourceSNPs,line); //ignore first line
	while (getline(sourceSNPs,line))
	{			
		lineToWrite = variationType;
		lineToWrite += ",";
		strtok((char *)(line.c_str()),","); //ignore first token
		lineToWrite += strtok(NULL,","); //chromosome name
		lineToWrite += ",";
		positionToken = strtok(NULL,","); //position
		lineToWrite += positionToken; //position
		lineToWrite += ",";
		valueToken = strtok(NULL,","); //value
		lineToWrite += valueToken;
		
		//check if duplicate previous position
		if (positionToken.compare(prevPositionToken) != 0) //different from previous string
		{
			//write line
			dest << lineToWrite << endl;
			
			++numVariations; //increment number of variations
		}
		else
		{
			// cout << "DUPLICATE" << endl;
			// cout << lineToWrite << endl;
		}
		
		prevPositionToken = positionToken;
	}
	// cout << numVariations << endl;
	sourceSNPs.close();
	
	//InDels///////////////////////////
	getline(sourceInDels,line); //ignore first line
	while (getline(sourceInDels,line))
	{	
		//cout << "line: " << line << endl;
		//prepare line to write
		
		lineToWrite = variationType;
		lineToWrite += ",";
		strtok((char *)(line.c_str()),","); //ignore first token
		lineToWrite += strtok(NULL,","); //chromosome name
		lineToWrite += ",";
		positionToken = strtok(NULL,","); //position
		lineToWrite += positionToken; //position
		lineToWrite += ",";
		strtok(NULL,","); //ignore end position token
		valueToken = strtok(NULL,","); //value
		lineToWrite += valueToken;
		
		//check if switched SNP to deletion, deletion to insertion
		// if !switchVariationTypeOnThisIteration
		if (lineToWrite.at(lineToWrite.length()-1) == '-') // deletion
		{
			if (variationType != '1') //change variation type to deletion
			{
				variationType = '1';
				switchVariationTypeOnThisIteration = true;
			}
		}
		else
		{
			if (valueToken[0] == '-') // insertion
			{
				if (variationType != '2') //change variation type to insertion
				{
					variationType = '2';
					switchVariationTypeOnThisIteration = true;
				}
			}
			else //SNP
			{
				if (variationType != '0') //change variation type to SNP
				{
					variationType = '0';
					switchVariationTypeOnThisIteration = true;
				}
			}
		}
		if (switchVariationTypeOnThisIteration) //update string to write
		{
			lineToWrite[0] = variationType;
			
			// cout << "SWITCHED to " << variationType << endl;
			
			switchVariationTypeOnThisIteration = false; //do not update by default on next iteration
		}
		
		//check if duplicate previous position
		if (positionToken.compare(prevPositionToken) != 0) //different from previous string
		{
			//write line
			dest << lineToWrite << endl;
			
			++numVariations; //increment number of variations
		}
		else
		{
			// cout << "DUPLICATE" << endl;
			// cout << lineToWrite << endl;
		}
		
		prevPositionToken = positionToken;
	}
	// cout << numVariations << endl;
	sourceInDels.close();
	
	dest.close();
}
