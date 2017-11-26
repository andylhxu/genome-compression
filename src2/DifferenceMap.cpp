//implementation

#include "DifferenceMap.h"

// ChromNames myChromNames; // declare global chromosome names

// bool Var::operator< (const Var &v_other) const
// {
	//return 1;//(this->position) < v_other.position;
	// return (this->position < v_other.position);
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Var
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Var::operator< (const Var &v_other) const
{
	if (this->position < v_other.position)
		return (true);
	else
	{
		if (this->position > v_other.position)
			return false;
		else
		{
			if ((this->varType == SNP_) && (v_other.varType != SNP_))
				return true;
			else
				return false;
		}
	}
}

bool Var::operator== (const Var &v_other) const
{ 
	return ((this->position == v_other.position) && 
			(this->varType == v_other.varType) && 
			(this->oldSeq == v_other.oldSeq) && 
			(this->newSeq == v_other.newSeq));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//VarMap
////////////////////////////////////////////////////////////////////////////////////////////////////////

VarMap::VarMap() //constructor
{
	//make empty var set for each chromosome
	varMap.resize(myChromNames.GetNumChromosomes());
	
	// for (int i = 0; i < myChromNames.GetNumChromosomes() ; ++i)
	// {
		// cout << myChromNames.IndexToChromName(i) << " has " << varMap[i].size() << " elements" << endl;
	// }	
}

VarMap::~VarMap() //destructor
{
}

void VarMap::Clear() // clears variation map
{
	// Var emptyVar(-1,-1,"Z","Z");
	

	for (int i = 0 ; i < myChromNames.GetNumChromosomes() ; ++i)
	{
		vector<Var> emptyVarVec;
		varMap[i].swap(emptyVarVec);
	}
	
	// vector< vector<Var> > emptyVarVecVec;
	
	// cout << "chrom names: " << endl;
	// myChromNames.ShowChromNames();
	
	// varMap.swap(emptyVarVecVec);
	// varMap.clear();
	// varMap.reserve(0);
}

// void VarMap::PushBackVar(const Var &varToInsert, const int &chromIndex)
// {
	// varMap[chromIndex].push_back(varToInsert);	
// }

void VarMap::ShowEntries(int chromIndex, int numEntriesShow) //shows entries
{
	int varsToShow = min(numEntriesShow,(int)(varMap[chromIndex].size()));
	cout << "first " << varsToShow << " entries of " << myChromNames.IndexToChromName(chromIndex) << ":" << endl;
	for (int j = 0 ; j < varsToShow ; j++)
	{
		varMap[chromIndex][j].Show();
		// cout << endl;
	}
}

bool IsSNP (Var const &var) {return (var.GetvarType() == SNP_);}
bool IsDel (Var const &var) {return (var.GetvarType() == DEL_);}
bool IsIns (Var const &var) {return (var.GetvarType() == INS_);}

void VarMap::WriteVarMapToFile(const char *outfilename)
{
	//reorder in order of SNPs, deletions, insertions
	vector< vector< vector<Var>::iterator > > partitionBounds(varMap.size()); //stores partition elements
	
	cout << "getting partition...";
	for (int chromIndex = 0 ; chromIndex < myChromNames.GetNumChromosomes() ; chromIndex++)
	{		
		//put SNPs first
		vector<Var>::iterator SNPDelBound = stable_partition(varMap[chromIndex].begin(),varMap[chromIndex].end(),IsSNP);
		//put deletions first in variations past SNPs
		vector<Var>::iterator DelInsBound = stable_partition(SNPDelBound,varMap[chromIndex].end(),IsDel);
		
		partitionBounds[chromIndex].push_back(SNPDelBound);
		partitionBounds[chromIndex].push_back(DelInsBound);
	}
	cout << "done" << endl;
	
	//write to file
	//open file
	ofstream dest(outfilename);	
	
	string line; //line to write
	
	string chromName; //current chromosome name
	
	ostringstream numstr; //holds position
	
	int position = 0; //variation position
	int prevPosition = 0; //previous position
	
	vector<Var>::iterator start_it; //start iterator for current variation type
	vector<Var>::iterator end_it; //end iterator for current variation type
	
	
	
	//write variations
	for (int j = 0; j < 3; ++j)
	{
		for (int chromIndex = 0 ; chromIndex < myChromNames.GetNumChromosomes() ; chromIndex++)
		{		
			if (j==0) //SNPs
			{	start_it = varMap[chromIndex].begin(); end_it = partitionBounds[chromIndex][0];}
			else if (j==1) //Dels
			{	start_it = partitionBounds[chromIndex][0]; end_it = partitionBounds[chromIndex][1];}
			else if (j==2) //Inss
			{	start_it = partitionBounds[chromIndex][1]; end_it = varMap[chromIndex].end();}
			
		
			chromName = myChromNames.IndexToChromName(chromIndex);
			for (vector<Var>::iterator it = start_it; it != end_it; ++it)
			{
				line = (it->GetvarType());
				line += ",";
				line += (chromName + ",");
				
				position = it->GetPosition();
				if (position < prevPosition) //reset line buffer text
					numstr.str("");
				prevPosition = position;
				
				numstr << position;
				numstr.seekp(0); //move put pointer back to front
								
				line += (numstr.str()+ ",");
				line += (it->GetOldSequence() + "/");
				line += (it->GetNewSequence());
				
				//write line
				dest << line << endl;
			}
		}
	}
	// close file
	dest.close();
}

void VarMap::WriteVarMapToFile2(const char *outfilename)
{
	//write to file
	//open file
	ofstream dest(outfilename);	
	
	string line; //line to write
	
	string chromName; //current chromosome name
	
	ostringstream numstr; //holds position
	
	int position = 0; //variation position
	int prevPosition = 0; //previous position
	
	vector<Var>::iterator start_it; //start iterator for current variation type
	vector<Var>::iterator end_it; //end iterator for current variation type
		
	//write variations
	for (int chromIndex = 0 ; chromIndex < myChromNames.GetNumChromosomes() ; chromIndex++)
	{		
		start_it = varMap[chromIndex].begin();
		end_it = varMap[chromIndex].end();
		
		chromName = myChromNames.IndexToChromName(chromIndex);
		for (vector<Var>::iterator it = start_it; it != end_it; ++it)
		{
			line = (it->GetvarType());
			line += ",";
			line += (chromName + ",");
			
			position = it->GetPosition();
			if (position < prevPosition) //reset line buffer text
				numstr.str("");
			prevPosition = position;
			
			numstr << position;
			numstr.seekp(0); //move put pointer back to front
							
			line += (numstr.str()+ ",");
			line += (it->GetOldSequence() + "/");
			line += (it->GetNewSequence());
			
			//write line
			dest << line << endl;
		}
	}
	// close file
	dest.close();
}

/*void VarMap::WriteVarMapToFile2(const char *outfilename)
{
	//write to file
	//open file
	ofstream dest(outfilename);	
	
	string line; //line to write
	
	string chromName; //current chromosome name
	
	ostringstream numstr; //holds position
	
	int position = 0; //variation position
	int prevPosition = 0; //previous position
	
	vector<Var>::iterator start_it; //start iterator for current variation type
	vector<Var>::iterator end_it; //end iterator for current variation type
		
	bool writeIndel = false; // true if writing indel
		
	//write variations
	for (int chromIndex = 0 ; chromIndex < myChromNames.GetNumChromosomes() ; chromIndex++)
	{		
		start_it = varMap[chromIndex].begin();
		end_it = varMap[chromIndex].end();
		
		chromName = myChromNames.IndexToChromName(chromIndex);
		vector<Var>::iterator next_it = start_it + 1;
		for (vector<Var>::iterator it = start_it; it != end_it; ++it)
		{
			//update next iterator
			next_it = it + 1;
		
			writeIndel = false;
			if (next_it != end_it)
				if (it->GetPosition() == next_it->GetPosition())
					if ((it->IsDel() && next_it->IsIns()) || (it->IsIns() && next_it->IsDel())) // write indel
						writeIndel = true;				
		
			if (writeIndel)
			{
				line = "3,";
				line += (chromName + ",");
				position = it->GetPosition();
				prevPosition = position;
				
				numstr << position;
				numstr.seekp(0); //move put pointer back to front
						
				line += (numstr.str()+ ",");
				if (it->IsDel())
				{
					line += (it->GetOldSequence() + "/");
					line += (next_it->GetNewSequence());
				} else
				{
					line += (next_it->GetOldSequence() + "/");
					line += (it->GetNewSequence());
				}
				
				//write line
				dest << line << endl;
				
				//increment iterator by 1, skip next entry
				++it;
			}
			else
			{
				line = (it->GetvarType());
				line += ",";
				line += (chromName + ",");
				
				position = it->GetPosition();
				if (position < prevPosition) //reset line buffer text
					numstr.str("");
				prevPosition = position;
				
				numstr << position;
				numstr.seekp(0); //move put pointer back to front
								
				line += (numstr.str()+ ",");
				line += (it->GetOldSequence() + "/");
				line += (it->GetNewSequence());
				
				//write line
				dest << line << endl;
			}
		}
	}
	// close file
	dest.close();
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////
//VarMapWatson : VarMap
////////////////////////////////////////////////////////////////////////////////////////////////////////

// bool cmp(const Var &v1, const Var &v2)
// {
	// return (v1.GetPosition() < v2.GetPosition());
// }

VarMapWatson::VarMapWatson(const char *Watsonfilename) // constructor, read Watson submission in shorter form
{
	vector< vector<Var> > SNPs; // SNPs by chromosome
	vector< vector<Var> > Dels; // Deletions by chromosome
	vector< vector<Var> > Inss; // Insertions by chromosome
	vector< vector<Var> > Indels; // Indels by chromosome
	// preallocate space for list for each chromosome
	SNPs.resize(myChromNames.GetNumChromosomes());
	Dels.resize(myChromNames.GetNumChromosomes());
	Inss.resize(myChromNames.GetNumChromosomes());
	Indels.resize(myChromNames.GetNumChromosomes());

	//input stream
	ifstream sourcefile(Watsonfilename);
	
	string line; //line read
	string stringVarType; //variation type 
	varType_ varType; //variation type 
	string chromName; //chromosome name
	string prevChromName; //previous chromosome name
	int position; //Var position
	stringstream numstr; //holds position number
	string oldSeq; //reference sequence
	string newSeq; //new sequence
	int temp;
	int chromIndex; //chromosome index
	int varNum = 0; //variation number
	while (getline(sourcefile,line))
	{
		stringVarType = strtok((char *)(line.c_str()),",");
		varType = (varType_)stringVarType[0];
		chromName = strtok(NULL,",");
		numstr.clear();
		numstr.str(strtok(NULL,","));
		numstr >> position;
		
		oldSeq = strtok(NULL,"/");
		newSeq = strtok(NULL,""); //end of line
		if (newSeq[newSeq.length()-1] == (char)13) //erase last character if new line
			newSeq.erase(newSeq.length()-1,1);
		
		//get chromosome index
		chromIndex = myChromNames.ChromNameToIndex(chromName);
		
		// newSeq = string();
		// oldSeq = string();
		
		if (varType == SNP_) // add to SNPs set			
			SNPs[chromIndex].push_back(Var(position,varType,oldSeq,newSeq));
		else if (varType == DEL_)
			Dels[chromIndex].push_back(Var(position,varType,oldSeq,newSeq));
		else if (varType == INS_)
			Inss[chromIndex].push_back(Var(position,varType,oldSeq,newSeq));
		else if (varType == INDEL_)
			Indels[chromIndex].push_back(Var(position,varType,oldSeq,newSeq));
		else
		{
			cout << "error: Unknown varType_" << endl;
			cin >> temp;
		}
		
		if (prevChromName != chromName)
			cout << "(var type, chr): (" << (char)varType << "," << chromName << ")" << endl;
		prevChromName = chromName;
	}
	sourcefile.close();
	
	// merge var lists
			
	Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
	// for (chromIndex = 12; chromIndex < 13 ; chromIndex++)
	for (chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; chromIndex++)
	{
		
		vector<Var> mergedSNPsDels;
		vector<Var> mergedSNPsDelsInss;
	
		// vector<Var> *SNPs_currentChrom = &SNPs[chromIndex];
	
		//merge SNPs and deletions
		mergedSNPsDels.resize(SNPs[chromIndex].size() + Dels[chromIndex].size(),padVar);
		
		// merge(SNPs[chromIndex].begin(),SNPs[chromIndex].end(),Dels[chromIndex].begin(),Dels[chromIndex].end(),varMap[chromIndex].begin(),cmp);
		merge(SNPs[chromIndex].begin(),SNPs[chromIndex].end(),
			  Dels[chromIndex].begin(),Dels[chromIndex].end(),
			  mergedSNPsDels.begin());
		
		//merge insertions
		mergedSNPsDelsInss.resize(mergedSNPsDels.size() + Inss[chromIndex].size(),padVar);
		merge(mergedSNPsDels.begin(),mergedSNPsDels.end(),
			  Inss[chromIndex].begin(),Inss[chromIndex].end(),
			  mergedSNPsDelsInss.begin());
		
		//merge deletions
		varMap[chromIndex].resize(mergedSNPsDelsInss.size() + Indels[chromIndex].size(),padVar);
		
		merge(mergedSNPsDelsInss.begin(),mergedSNPsDelsInss.end(),
			  Indels[chromIndex].begin(),Indels[chromIndex].end(),
			  varMap[chromIndex].begin());
	}
	
	/*//show first few entries
	for (int i = 0; i < 25 ; i++)
	{
		ShowEntries(i,10);
	}*/
}

VarMapWatson::~VarMapWatson() // destructor
{
}

void VarMapWatson::ClearWatson()
{
	for (int i = 0 ; i < myChromNames.GetNumChromosomes() ; ++i)
	{
		vector<Var> emptyVarVec;
		varMap[i].swap(emptyVarVec);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//VarMapdbSNP : VarMap
////////////////////////////////////////////////////////////////////////////////////////////////////////

VarMapdbSNP::VarMapdbSNP(char *dbSNPDirectory) //constructor, read dbSNP from directory with name dbSNP directory with one file per chromosome name in myChromNames
{
	string dbSNPchromfilename; //filename of current chromosome
	
	int numValidEntries = 0;
	int numIgnoredEntries = 0;
	int numSNPs = 0;
	int numDels = 0;
	int numInss = 0;
	
	int repPos = 0; //number repeated positions
	int lessThanPos = 0; //number position less than previous position
		
	// int chromIndex = myChromNames.ChromNameToIndex(chromName);
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		dbSNPchromfilename = dbSNPDirectory + string("/");
		dbSNPchromfilename += myChromNames.IndexToChromName(chromIndex);
		dbSNPchromfilename += ".txt";
				
		//input stream
		ifstream sourcefile(dbSNPchromfilename.c_str());
		
		string line; //line read
		int position; //Var position, starting position, second column in dbSNP
		int prevPosition = 0; //previous Var position
		char strand; //+ or -, strand direction
		
		string stringVarType; //variation type 
		varType_ varType; //variation type 
		
		stringstream numstr; //holds position number
		string oldSeq; //reference sequence
		string newSeq; //new sequence
		
		string obs1; //observed field value 1 (s1/s2)
		string obs2; //observed field value 2 (s1/s2)
		
		bool ignoreThisLine; //true if should ignore current line
		
		getline(sourcefile,line); //skip first line		
		while (getline(sourcefile,line))
		{
			if (line.find("/") != string::npos) // if contains at least one '/' character
			{
				//check if should ignore line
				ignoreThisLine = false;
								
				if (line.find("single") == string::npos) //criterion used in Christley et al. to keep dbSNP entry
					ignoreThisLine = true;
					
				// if ((line.find("single") == string::npos) && (line.find("deletion") == string::npos)) //criterion used in Christley et al. to keep dbSNP entry
					// ignoreThisLine = true;
					
				// if (line.find("single") == string::npos) //criterion used in Christley et al. to keep dbSNP entry
					// ignoreThisLine = true;
				
				// if (line.find("single") == string::npos){
					// if (line.find("deletion") == string::npos){
						// if (line.find("insertion") == string::npos)
							// ignoreThisLine = true;}}
							
				// if (line.find("insertion") != string::npos)
					// ignoreThisLine = true;
				// else if (line.find("deletion") != string::npos)
					// ignoreThisLine = true;
			
				strtok((char *)(line.c_str()),"\t"); //ignore #bin field of dbSNP
				strtok(NULL,"\t"); //ignore chromosome name field of dbSNP
				strtok(NULL,"\t"); //ignore chromStart field of dbSNP
				
				numstr.clear();
				numstr.str(strtok(NULL,"\t"));
				numstr >> position;
				
				if (position == prevPosition) //ignore lines that have same position as previous lines
					{ignoreThisLine = true; ++repPos;}
				if (position < prevPosition)
					{ignoreThisLine = true; ++lessThanPos;}
				prevPosition = max(prevPosition,position);
				
				strtok(NULL,"\t"); //ignore name field of dbSNP
				strtok(NULL,"\t"); //ignore score field of dbSNP
				
				strand = strtok(NULL,"\t")[0]; //strand direction
				
				oldSeq = strtok(NULL,"\t"); //refNCBI
				strtok(NULL,"\t"); //ignore refUCSC field of dbSNP
				
				obs1 = strtok(NULL,"/");  //observed field value 1 (s1/s2)
				
				// if (line.find("/") != string::npos) //if contains more than one '/' character
					// ignoreThisLine = true;
				
				obs2 = strtok(NULL,"\t"); //observed field value 2 (s1/s2)
				
				//if '-' direction, reverse complement obs sequence
				if (strand == '-')
				{
					ReverseComplement(obs1);
					ReverseComplement(obs2);
				} else if (strand == '+') {}
				else //ignore line if direction neither '+' nor '-'
					ignoreThisLine = true;
				
				if (obs1[0] != '-') // if SNP
				{
					// if (((obs1.length() != 1) || (obs2.length() != 1)) || (oldSeq.length() != 1))
					// if (!((obs1.length() == 1) && (obs2.length() == 1) && (oldSeq.length() == 1)))
						// ignoreThisLine = true;
					// varType = SNP_;
					// if (obs1[0] == oldSeq[0])
						// newSeq = obs2;
					// else if (obs2[0] == oldSeq[0])
						// newSeq = obs1;
					// else //one of observed values must match reference value
						// ignoreThisLine = true;
						
					varType = SNP_;
					if (obs1[0] == oldSeq[0])
						{newSeq = obs2[0]; oldSeq = obs1[0];}
					else if (obs2[0] == oldSeq[0])
						{newSeq = obs1[0]; oldSeq = obs2[0];}
					else //one of observed values must match reference value
						ignoreThisLine = true;	
				} else //deletion or insertion
				{
					ignoreThisLine = true; // added 9:15pm, 3/7/13
					if (oldSeq[0] == '-') // insertion
					{
						varType = INS_;
						newSeq = obs2; //insertion sequence
						oldSeq = string(newSeq.length(),'-'); //repeat '-' length(old sequence) times
					} else // deletion
					{
						varType = DEL_;
						oldSeq = obs2;
						newSeq = string(oldSeq.length(),'-'); //repeat '-' length(old sequence) times
					}
				}
				
				if (!(ignoreThisLine))
				{
					varMap[chromIndex].push_back(Var(position,varType,oldSeq,newSeq));
					
					++numValidEntries;
					if (varType == SNP_)
						++numSNPs;
					else if (varType == DEL_)
						++numDels;
					else if (varType == INS_)
						++numInss;
				}
				else
					++numIgnoredEntries;
			} else
				++numIgnoredEntries;
		}
		sourcefile.close();
		
		cout << "finished reading dbSNP " << myChromNames.IndexToChromName(chromIndex) << endl;
	}
	
	cout << "  valid entries: " << numValidEntries << endl
		 // << "       num SNPs: " << numSNPs << endl
		 // << "       num Dels: " << numDels << endl
		 // << "       num Inss: " << numInss << endl
		 << "ignored entries: " << numIgnoredEntries << endl;
	
	// cout << "rep pos: " << repPos << ", less than pos: " << lessThanPos << endl;
	
	numValidEntries = 0;
	numIgnoredEntries = 0;
	
	/*
	// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	// {
		numSNPs = 0;
		numDels = 0;
		numInss = 0;
		numValidEntries += varMap[chromIndex].size();
		for (int i = 0 ; i < varMap[chromIndex].size() ; ++i)
		{
			// cout << (char)varMap[chromIndex][i].GetvarType() << endl;
			switch (varMap[chromIndex][i].GetvarType())
			{
				case SNP_:
					++numSNPs;
					break;
				case DEL_:
					++numDels;
					break;
				case INS_:
					++numInss;
					break;
			}
		}
		cout << myChromNames.IndexToChromName(chromIndex) << ": "
			 << "(SNP, ins, del): (" << numSNPs << "," << numDels << "," << numInss << ")" << endl;
	// }
	*/
	
	//check if sorted
	int minDiffPos = 0;
	int numviolatedposition = 0;
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		for (int i = 1 ; i < varMap[chromIndex].size() ; ++i)
		{
			if (varMap[chromIndex][i-1].GetPosition() >= varMap[chromIndex][i].GetPosition())
			{
				++numviolatedposition;
				// cout << myChromNames.IndexToChromName(chromIndex) << endl;
				// varMap[chromIndex][i-1].Show();
				// cout << endl;
				// varMap[chromIndex][i].Show();
				// cout << endl;
			}
		}
	}
	
	cout << "num violated position requirement: " << numviolatedposition << endl;
		
	// ShowEntries(0,10);
}

// read only chromosome with name chromName
VarMapdbSNP::VarMapdbSNP(char *dbSNPDirectory, string chromName, bool usedbSNP) //constructor, read dbSNP from directory with name dbSNP directory with one file per chromosome name in myChromNames
{
	if (usedbSNP)
	{
			string dbSNPchromfilename; //filename of current chromosome
		
		int numValidEntries = 0;
		int numIgnoredEntries = 0;
		int numSNPs = 0;
		int numDels = 0;
		int numInss = 0;
		
		int repPos = 0; //number repeated positions
		int lessThanPos = 0; //number position less than previous position
		
		// for (int chromIndex = 12; chromIndex < 13 ; ++chromIndex)
		// for (int chromIndex = 19; chromIndex < 20 ; ++chromIndex)
		// for (int chromIndex = 0; chromIndex < 1 ; ++chromIndex)
		// for (int chromIndex = 1; chromIndex < 2 ; ++chromIndex)
		
		int chromIndex = myChromNames.ChromNameToIndex(chromName);
		
		// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
		// {
			dbSNPchromfilename = dbSNPDirectory + string("/");
			dbSNPchromfilename += myChromNames.IndexToChromName(chromIndex);
			dbSNPchromfilename += ".txt";
			
			// cout << dbSNPchromfilename << endl;
			
			//input stream
			ifstream sourcefile(dbSNPchromfilename.c_str());
			
			string line; //line read
			int position; //Var position, starting position, second column in dbSNP
			int prevPosition = 0; //previous Var position
			char strand; //+ or -, strand direction
			
			string stringVarType; //variation type 
			varType_ varType; //variation type 
			
			stringstream numstr; //holds position number
			string oldSeq; //reference sequence
			string newSeq; //new sequence
			
			string obs1; //observed field value 1 (s1/s2)
			string obs2; //observed field value 2 (s1/s2)
			// int temp;
			
			bool ignoreThisLine; //true if should ignore current line
			
			getline(sourcefile,line); //skip first line		
			while (getline(sourcefile,line))
			{
				if (line.find("/") != string::npos) // if contains at least one '/' character
				{
					//check if should ignore line
					ignoreThisLine = false;
					
					/*
					//line should contain "single", "deletion", or "insertion"
					if (line.find("single") == string::npos) //if contains more than one '/' character
					{
						// if (line.find("deletion") == string::npos)
						// {
							if (line.find("insertion") == string::npos)
								ignoreThisLine = true;
						// }
					}
					if (line.find("deletion") != string::npos)
						ignoreThisLine = true;
					*/
					
					if (line.find("single") == string::npos) //criterion used in Christley et al. to keep dbSNP entry
						ignoreThisLine = true;
						
					// if ((line.find("single") == string::npos) && (line.find("deletion") == string::npos)) //criterion used in Christley et al. to keep dbSNP entry
						// ignoreThisLine = true;
						
					// if (line.find("single") == string::npos) //criterion used in Christley et al. to keep dbSNP entry
						// ignoreThisLine = true;
					
					// if (line.find("single") == string::npos){
						// if (line.find("deletion") == string::npos){
							// if (line.find("insertion") == string::npos)
								// ignoreThisLine = true;}}
								
					// if (line.find("insertion") != string::npos)
						// ignoreThisLine = true;
					// else if (line.find("deletion") != string::npos)
						// ignoreThisLine = true;
				
					strtok((char *)(line.c_str()),"\t"); //ignore #bin field of dbSNP
					strtok(NULL,"\t"); //ignore chromosome name field of dbSNP
					strtok(NULL,"\t"); //ignore chromStart field of dbSNP
					
					numstr.clear();
					numstr.str(strtok(NULL,"\t"));
					numstr >> position;
					
					if (position == prevPosition) //ignore lines that have same position as previous lines
						{ignoreThisLine = true; ++repPos;}
					if (position < prevPosition)
						{ignoreThisLine = true; ++lessThanPos;}
					prevPosition = max(prevPosition,position);
					
					strtok(NULL,"\t"); //ignore name field of dbSNP
					strtok(NULL,"\t"); //ignore score field of dbSNP
					
					strand = strtok(NULL,"\t")[0]; //strand direction
					
					oldSeq = strtok(NULL,"\t"); //refNCBI
					strtok(NULL,"\t"); //ignore refUCSC field of dbSNP
					
					obs1 = strtok(NULL,"/");  //observed field value 1 (s1/s2)
					
					// if (line.find("/") != string::npos) //if contains more than one '/' character
						// ignoreThisLine = true;
					
					obs2 = strtok(NULL,"\t"); //observed field value 2 (s1/s2)
					
					//if '-' direction, reverse complement obs sequence
					if (strand == '-')
					{
						ReverseComplement(obs1);
						ReverseComplement(obs2);
					} else if (strand == '+') {}
					else //ignore line if direction neither '+' nor '-'
						ignoreThisLine = true;
					
					if (obs1[0] != '-') // if SNP
					{
						// if (((obs1.length() != 1) || (obs2.length() != 1)) || (oldSeq.length() != 1))
						// if (!((obs1.length() == 1) && (obs2.length() == 1) && (oldSeq.length() == 1)))
							// ignoreThisLine = true;
						// varType = SNP_;
						// if (obs1[0] == oldSeq[0])
							// newSeq = obs2;
						// else if (obs2[0] == oldSeq[0])
							// newSeq = obs1;
						// else //one of observed values must match reference value
							// ignoreThisLine = true;
							
						varType = SNP_;
						if (obs1[0] == oldSeq[0])
							{newSeq = obs2[0]; oldSeq = obs1[0];}
						else if (obs2[0] == oldSeq[0])
							{newSeq = obs1[0]; oldSeq = obs2[0];}
						else //one of observed values must match reference value
							ignoreThisLine = true;	
					} else //deletion or insertion
					{
						ignoreThisLine = true; // added 9:15pm, 3/7/13
						if (oldSeq[0] == '-') // insertion
						{
							varType = INS_;
							newSeq = obs2; //insertion sequence
							oldSeq = string(newSeq.length(),'-'); //repeat '-' length(old sequence) times
						} else // deletion
						{
							varType = DEL_;
							oldSeq = obs2;
							newSeq = string(oldSeq.length(),'-'); //repeat '-' length(old sequence) times
						}
					}
					
					if (!(ignoreThisLine))
					{
						varMap[chromIndex].push_back(Var(position,varType,oldSeq,newSeq));
						
						++numValidEntries;
						if (varType == SNP_)
							++numSNPs;
						else if (varType == DEL_)
							++numDels;
						else if (varType == INS_)
							++numInss;
					
						// cout << myChromNames.IndexToChromName(chromIndex) << ", " << 
						// "(" << position << "," << (char)varType << "," << strand << "," << (strand == '+') << "," << oldSeq << "," << newSeq << ")" << endl;
						// "(" << position << "," << (char)varType << "," << strand << "," << oldSeq << "," << newSeq << ")" << endl;
						
						// if (position > 4580)
						// {
							// int i = 1;
							// cin >> i;
						// }
					}
					else
						++numIgnoredEntries;
				} else
					++numIgnoredEntries;
				// for (int j = 0 ; j < 100 ; j++)
					// getline(sourcefile,line);
			}
			sourcefile.close();
			
			cout << "finished reading " << myChromNames.IndexToChromName(chromIndex) << endl;
		// }
		
		cout << "  valid entries: " << numValidEntries << endl
			 << "       num SNPs: " << numSNPs << endl
			 << "       num Dels: " << numDels << endl
			 << "       num Inss: " << numInss << endl
			 << "ignored entries: " << numIgnoredEntries << endl;
		
		cout << "rep pos: " << repPos << ", less than pos: " << lessThanPos << endl;
		// int i = 0;
		// cin >> i;
		
		numValidEntries = 0;
		numIgnoredEntries = 0;
		
		// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
		// {
			numSNPs = 0;
			numDels = 0;
			numInss = 0;
			numValidEntries += varMap[chromIndex].size();
			for (int i = 0 ; i < varMap[chromIndex].size() ; ++i)
			{
				// cout << (char)varMap[chromIndex][i].GetvarType() << endl;
				switch (varMap[chromIndex][i].GetvarType())
				{
					case SNP_:
						++numSNPs;
						break;
					case DEL_:
						++numDels;
						break;
					case INS_:
						++numInss;
						break;
				}
			}
			cout << myChromNames.IndexToChromName(chromIndex) << ": "
				 << "(SNP, ins, del): (" << numSNPs << "," << numDels << "," << numInss << ")" << endl;
		// }
		
		//check if sorted
		int minDiffPos = 0;
		int numviolatedposition = 0;
		// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
		// {
			for (int i = 1 ; i < varMap[chromIndex].size() ; ++i)
			{
				if (varMap[chromIndex][i-1].GetPosition() >= varMap[chromIndex][i].GetPosition())
				{
					++numviolatedposition;
					cout << myChromNames.IndexToChromName(chromIndex) << endl;
					varMap[chromIndex][i-1].Show();
					cout << endl;
					varMap[chromIndex][i].Show();
					cout << endl;
				}
			}
		// }
		
		cout << "num violated position requirement: " << numviolatedposition << endl;
			
		// ShowEntries(0,10);
	}
}

//input stream
	// ifstream sourcefile(Watsonfilename);
VarMapdbSNP::~VarMapdbSNP() //destructor
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//DifferenceMap
////////////////////////////////////////////////////////////////////////////////////////////////////////
// DifferenceMap::DifferenceMap() //constructor
// {
// }

DifferenceMap DifferenceMap::operator= (const DifferenceMap &other)
{
	//inherited from VarMap
	this->myChromNames = other.myChromNames;
	this->varMap = other.varMap;
	
	//added in this class
	this->matchIndices = other.matchIndices;
	this->matchVarTypes = other.matchVarTypes;
	this->partialMatchIndices = other.partialMatchIndices;
	this->partialMatchVarTypes = other.partialMatchVarTypes;
	this->partialMatchOldSequences = other.partialMatchOldSequences;
	this->partialMatchNewSequences = other.partialMatchNewSequences;
}

// bool Var::operator== (const Var &v_other) const
// {
	// return ((this->position == v_other.position) && 
			// (this->varType == v_other.varType) && 
			// (this->oldSeq == v_other.oldSeq) && 
			// (this->newSeq == v_other.newSeq));
// }

DifferenceMap::DifferenceMap(const VarMap &varMap1, const VarMap &varMap2) //constructor, computes map1 - map2, indices in map2
{
	//resize match, partial match indices vectors
	// preallocate space for list for each chromosome
	matchIndices.resize(myChromNames.GetNumChromosomes());
	matchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchIndices.resize(myChromNames.GetNumChromosomes());
	partialMatchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchOldSequences.resize(myChromNames.GetNumChromosomes());
	partialMatchNewSequences.resize(myChromNames.GetNumChromosomes());

	cout << "HERE 1" << endl;

	//iterators
	vector<Var>::const_iterator it1;
	vector<Var>::const_iterator it2;
	int it1_int; //counts entries in map1
	int it2_int; //counts entries in map2
	int numMatches;
	int numPartialMatches;
	int numSNPMatches = 0; //counts total number of SNP matches
	
	cout << "HERE 2" << endl;
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		cout << "chrom: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl;
		//resize difference map
		Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
		varMap[chromIndex].resize(varMap1.GetVarMapRef()[chromIndex].size() , padVar);
		
		// cout << "HERE 3" << endl;
		
		// cout << varMap[chromIndex].size() << endl;
		// cout << varMap2.GetVarMapRef()[chromIndex].size() << endl;
		
		matchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		matchVarTypes[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		// matchIndices[chromIndex].resize(0);
		
		// cout << "HERE 4" << endl;
		
		partialMatchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchVarTypes[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchOldSequences[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchNewSequences[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
	
		// cout << "HERE 5" << endl;
	
		it1 = varMap1.GetVarMapRef()[chromIndex].begin();
		it2 = varMap2.GetVarMapRef()[chromIndex].begin();
		
		// it1 = varMap1.varMap[chromIndex].begin();
		// it2 = varMap2.varMap[chromIndex].begin();
		
		it1_int = 0;
		it2_int = 0;
		numMatches = 0;
		numPartialMatches = 0;
		
		/////////////////////////////////////////////////////////////////////////////////
		// int i = 0;
		
		// int initSNPs = 0;
		// int initDels = 0;
		// int initInss = 0;
		
		/////////////////////////////////////////////////////////////////////////////////
		// for (vector<Var>::const_iterator it1_ = varMap1.GetVarMapRef()[chromIndex].begin() ; it1_ != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1_)
		// {
			// if (IsSNP(*it1_))
				// ++initSNPs;
			// else if (IsDel(*it1_))
				// ++initDels;
			// else if (IsIns(*it1_))
				// ++initInss;
		// }
		// cout << "map1: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl
		 // << "SNPs: " << initSNPs << endl
		 // << "Dels: " << initDels << endl
		 // << "Inss: " << initInss << endl;
		 // int iii = 0;
		 // cin >> iii;
		 /////////////////////////////////////////////////////////////////////////////////
		
		//compute set difference
		while ((it1 != varMap1.GetVarMapRef()[chromIndex].end()) && (it2 != varMap2.GetVarMapRef()[chromIndex].end()))
		// while ((it1 != varMap1.varMap[chromIndex].end()) && (it2 != varMap2.varMap[chromIndex].end()))
		{		
			// it1->Show();
			// int ii;
			// cin >> ii;
			
			// if (!IsSNP(*it1) /*&& !IsSNP(*it2)*/)
			// {
				// cout << "non SNP:" << endl;
				// it1->Show();
				// it2->Show();
				// cout << (*it1 < *it2) << "," << (*it2 < *it1) << "," << (*it1 == *it2) << endl;
				// int ii;
				// cout << "HERE" << endl;
				// cin >> ii;
			// }
		
			if (*it1 < *it2) //compares position
			{			
				varMap[chromIndex][it1_int] = *it1; //add variation to difference map
				++it1_int;
				
				++it1; // ++i;
			}
			else if (*it2 < *it1)
			{
				++it2; ++it2_int;
			}
			else //positions equal
			{			
				//check sequences, variation types match
				if (*it1 == *it2)
				{	
					// matchIndices[chromIndex].push_back(it2_int);
					matchIndices[chromIndex][numMatches] = it2_int;
					matchVarTypes[chromIndex][numMatches] = it1->GetvarType();
					++numMatches;
					
					if (IsSNP(*it1))
						++numSNPMatches;
				}
				else //partial match (variation types mismatch?)
				{	
					if (((IsSNP(*it1) && IsSNP(*it2))) && (2 > 1))// && (it1->GetOldSequence() == it2->GetOldSequence()))
					{
					// if (it1->GetvarType() == it2->GetvarType())
					
						// partialMatchIndices[chromIndex].push_back(it2_int);
						partialMatchIndices[chromIndex][numPartialMatches] = it2_int;
						partialMatchVarTypes[chromIndex][numPartialMatches] = it1->GetvarType();
						partialMatchOldSequences[chromIndex][numPartialMatches] = it1->GetOldSequence();
						partialMatchNewSequences[chromIndex][numPartialMatches] = it1->GetNewSequence();
						++numPartialMatches;
						// cout << "partial match:" << endl;
						// it1->Show(); cout << endl; it2->Show(); cout << endl << "******************" << endl;
						// int iii = 0; cin >> iii;
					}
					else ////add variation to difference map
					{
						varMap[chromIndex][it1_int] = *it1; //add variation to difference map
						++it1_int;
					}
				}
				++it1;// ++i;
				++it2; ++it2_int;
			}
		}
		//put remaining elements in map1 in diff map
		while (it1 != varMap1.GetVarMapRef()[chromIndex].end())
		{
			varMap[chromIndex][it1_int] = *it1; //add variation to difference map
			++it1; ++it1_int;
		}
			
		
		// cout << "------" << chromIndex << ", " << "i = " << i << endl;
		
		// cout << "HERE 6" << endl;
		// cout << "matches: " << numMatches << endl;
		//resize difference map
		varMap[chromIndex].resize(it1_int,padVar);
		// cout << "last element: " << endl;
		// if (varMap[chromIndex].size() > 0)
			// varMap[chromIndex][varMap[chromIndex].size()-1].Show();
		matchIndices[chromIndex].resize(numMatches);
		matchVarTypes[chromIndex].resize(numMatches);
		partialMatchIndices[chromIndex].resize(numPartialMatches);
		partialMatchVarTypes[chromIndex].resize(numPartialMatches);
		partialMatchOldSequences[chromIndex].resize(numPartialMatches);
		partialMatchNewSequences[chromIndex].resize(numPartialMatches);
	}
	
	//count matches
	//map1 counts
	int initSNPs = 0;
	int initDels = 0;
	int initInss = 0;
	//difference map counts
	int diffSNPs = 0;
	int diffDels = 0;
	int diffInss = 0;
	//total match counts
	int totalMatches = 0;
	int totalPartialMatches = 0;
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		for (vector<Var>::const_iterator it1 = varMap1.GetVarMapRef()[chromIndex].begin() ; it1 != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1)
		{
			if (IsSNP(*it1))
				++initSNPs;
			else if (IsDel(*it1))
				++initDels;
			else if (IsIns(*it1))
				++initInss;
		}
		
		for (vector<Var>::iterator itd = varMap[chromIndex].begin() ; itd != varMap[chromIndex].end() ; ++itd)
		{
			if (IsSNP(*itd))
				++diffSNPs;
			else if (IsDel(*itd))
				++diffDels;
			else if (IsIns(*itd))
				++diffInss;
		}
		
		totalMatches += matchIndices[chromIndex].size();
		totalPartialMatches += partialMatchIndices[chromIndex].size();
	}
	
	cout << "map1:" << endl
		 << "SNPs: " << initSNPs << endl
		 << "Dels: " << initDels << endl
		 << "Inss: " << initInss << endl;
	
	cout << "map diff:" << endl
		 << "SNPs: " << diffSNPs << endl
		 << "Dels: " << diffDels << endl
		 << "Inss: " << diffInss << endl;
	
	cout << "total matches: " << totalMatches << endl;
	cout << "total partial matches: " << totalPartialMatches << endl;
	cout << endl << "total SNP matches: " << numSNPMatches << endl << endl;
}

DifferenceMap::DifferenceMap() //constructor, computes map1 - map2, indices in map2
{
	//resize match, partial match indices vectors
	// preallocate space for list for each chromosome
	matchIndices.resize(myChromNames.GetNumChromosomes());
	matchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchIndices.resize(myChromNames.GetNumChromosomes());
	partialMatchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchOldSequences.resize(myChromNames.GetNumChromosomes());
	partialMatchNewSequences.resize(myChromNames.GetNumChromosomes());
}

void DifferenceMap::Update(const VarMap &varMap1, const VarMap &varMap2, const string chromName) //constructor, computes map1 - map2, indices in map2
{
	// cout << "HERE 1" << endl;

	//iterators
	vector<Var>::const_iterator it1;
	vector<Var>::const_iterator it2;
	int it1_int; //counts entries in map1
	int it2_int; //counts entries in map2
	int numMatches;
	int numPartialMatches;
	int numSNPMatches = 0; //counts total number of SNP matches
	
	// cout << "HERE 2" << endl;
	int chromIndex = myChromNames.ChromNameToIndex(chromName);
	
	{
		// cout << "chrom: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl;
		//resize difference map
		Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
		varMap[chromIndex].resize(varMap1.GetVarMapRef()[chromIndex].size() , padVar);
		
		// cout << "HERE 3" << endl;
		
		// cout << varMap[chromIndex].size() << endl;
		// cout << varMap2.GetVarMapRef()[chromIndex].size() << endl;
		
		matchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		matchVarTypes[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		// matchIndices[chromIndex].resize(0);
		
		// cout << "HERE 4" << endl;
		
		partialMatchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchVarTypes[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchOldSequences[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchNewSequences[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
	
		// cout << "HERE 5" << endl;
	
		it1 = varMap1.GetVarMapRef()[chromIndex].begin();
		it2 = varMap2.GetVarMapRef()[chromIndex].begin();
		
		// it1 = varMap1.varMap[chromIndex].begin();
		// it2 = varMap2.varMap[chromIndex].begin();
		
		it1_int = 0;
		it2_int = 0;
		numMatches = 0;
		numPartialMatches = 0;
		
		/////////////////////////////////////////////////////////////////////////////////
		// int i = 0;
		
		// int initSNPs = 0;
		// int initDels = 0;
		// int initInss = 0;
		
		/////////////////////////////////////////////////////////////////////////////////
		// for (vector<Var>::const_iterator it1_ = varMap1.GetVarMapRef()[chromIndex].begin() ; it1_ != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1_)
		// {
			// if (IsSNP(*it1_))
				// ++initSNPs;
			// else if (IsDel(*it1_))
				// ++initDels;
			// else if (IsIns(*it1_))
				// ++initInss;
		// }
		// cout << "map1: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl
		 // << "SNPs: " << initSNPs << endl
		 // << "Dels: " << initDels << endl
		 // << "Inss: " << initInss << endl;
		 // int iii = 0;
		 // cin >> iii;
		 /////////////////////////////////////////////////////////////////////////////////
		
		//compute set difference
		while ((it1 != varMap1.GetVarMapRef()[chromIndex].end()) && (it2 != varMap2.GetVarMapRef()[chromIndex].end()))
		// while ((it1 != varMap1.varMap[chromIndex].end()) && (it2 != varMap2.varMap[chromIndex].end()))
		{	
			if (*it1 < *it2) //compares position
			{			
				varMap[chromIndex][it1_int] = *it1; //add variation to difference map
				++it1_int;
				
				++it1; // ++i;
			}
			else if (*it2 < *it1)
			{
				++it2; ++it2_int;
			}
			else //positions equal
			{			
				//check sequences, variation types match
				if (*it1 == *it2)
				{	
					// matchIndices[chromIndex].push_back(it2_int);
					matchIndices[chromIndex][numMatches] = it2_int;
					matchVarTypes[chromIndex][numMatches] = it1->GetvarType();
					++numMatches;
					
					if (IsSNP(*it1))
						++numSNPMatches;
				}
				else //partial match (variation types mismatch?)
				{	
					if (((IsSNP(*it1) && IsSNP(*it2))) && (2 > 1))// && (it1->GetOldSequence() == it2->GetOldSequence()))
					{
					// if (it1->GetvarType() == it2->GetvarType())
					
						// partialMatchIndices[chromIndex].push_back(it2_int);
						partialMatchIndices[chromIndex][numPartialMatches] = it2_int;
						partialMatchVarTypes[chromIndex][numPartialMatches] = it1->GetvarType();
						partialMatchOldSequences[chromIndex][numPartialMatches] = it1->GetOldSequence();
						partialMatchNewSequences[chromIndex][numPartialMatches] = it1->GetNewSequence();
						++numPartialMatches;
						// cout << "partial match:" << endl;
						// it1->Show(); cout << endl; it2->Show(); cout << endl << "******************" << endl;
						// int iii = 0; cin >> iii;
					}
					else ////add variation to difference map
					{
						varMap[chromIndex][it1_int] = *it1; //add variation to difference map
						++it1_int;
					}
				}
				++it1;// ++i;
				++it2; ++it2_int;
			}
		}
		//put remaining elements in map1 in diff map
		while (it1 != varMap1.GetVarMapRef()[chromIndex].end())
		{
			varMap[chromIndex][it1_int] = *it1; //add variation to difference map
			++it1; ++it1_int;
		}
			
		
		// cout << "------" << chromIndex << ", " << "i = " << i << endl;
		
		// cout << "HERE 6" << endl;
		// cout << "matches: " << numMatches << endl;
		//resize difference map
		varMap[chromIndex].resize(it1_int,padVar);
		// cout << "last element: " << endl;
		// if (varMap[chromIndex].size() > 0)
			// varMap[chromIndex][varMap[chromIndex].size()-1].Show();
		matchIndices[chromIndex].resize(numMatches);
		matchVarTypes[chromIndex].resize(numMatches);
		partialMatchIndices[chromIndex].resize(numPartialMatches);
		partialMatchVarTypes[chromIndex].resize(numPartialMatches);
		partialMatchOldSequences[chromIndex].resize(numPartialMatches);
		partialMatchNewSequences[chromIndex].resize(numPartialMatches);
	}
	
	//count matches
	//map1 counts
	int initSNPs = 0;
	int initDels = 0;
	int initInss = 0;
	//difference map counts
	int diffSNPs = 0;
	int diffDels = 0;
	int diffInss = 0;
	//total match counts
	int totalMatches = 0;
	int totalPartialMatches = 0;
	
	// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	do
	{
		for (vector<Var>::const_iterator it1 = varMap1.GetVarMapRef()[chromIndex].begin() ; it1 != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1)
		{
			if (IsSNP(*it1))
				++initSNPs;
			else if (IsDel(*it1))
				++initDels;
			else if (IsIns(*it1))
				++initInss;
		}
		
		for (vector<Var>::iterator itd = varMap[chromIndex].begin() ; itd != varMap[chromIndex].end() ; ++itd)
		{
			if (IsSNP(*itd))
				++diffSNPs;
			else if (IsDel(*itd))
				++diffDels;
			else if (IsIns(*itd))
				++diffInss;
		}
		
		totalMatches += matchIndices[chromIndex].size();
		totalPartialMatches += partialMatchIndices[chromIndex].size();
	} while( false );
	
	cout << "map1:" << endl
		 << "SNPs: " << initSNPs << endl
		 << "Dels: " << initDels << endl
		 << "Inss: " << initInss << endl;
	
	cout << "map diff:" << endl
		 << "SNPs: " << diffSNPs << endl
		 << "Dels: " << diffDels << endl
		 << "Inss: " << diffInss << endl;
	
	cout << "total matches: " << totalMatches << endl;
	cout << "total partial matches: " << totalPartialMatches << endl;
	cout << endl << "total SNP matches: " << numSNPMatches << endl << endl;
}

DifferenceMap::DifferenceMap(const VarMap &varMap1, const VarMap &varMap2, const string chromName) //constructor, computes map1 - map2, indices in map2
{
	//resize match, partial match indices vectors
	// preallocate space for list for each chromosome
	matchIndices.resize(myChromNames.GetNumChromosomes());
	matchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchIndices.resize(myChromNames.GetNumChromosomes());
	partialMatchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchOldSequences.resize(myChromNames.GetNumChromosomes());
	partialMatchNewSequences.resize(myChromNames.GetNumChromosomes());

	cout << "HERE 1" << endl;

	//iterators
	vector<Var>::const_iterator it1;
	vector<Var>::const_iterator it2;
	int it1_int; //counts entries in map1
	int it2_int; //counts entries in map2
	int numMatches;
	int numPartialMatches;
	int numSNPMatches = 0; //counts total number of SNP matches
	
	cout << "HERE 2" << endl;
	int chromIndex = myChromNames.ChromNameToIndex(chromName);
	do
	{
		cout << "chrom: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl;
		//resize difference map
		Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
		varMap[chromIndex].resize(varMap1.GetVarMapRef()[chromIndex].size() , padVar);
		
		// cout << "HERE 3" << endl;
		
		// cout << varMap[chromIndex].size() << endl;
		// cout << varMap2.GetVarMapRef()[chromIndex].size() << endl;
		
		matchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		matchVarTypes[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		// matchIndices[chromIndex].resize(0);
		
		// cout << "HERE 4" << endl;
		
		partialMatchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchVarTypes[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchOldSequences[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		partialMatchNewSequences[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
	
		// cout << "HERE 5" << endl;
	
		it1 = varMap1.GetVarMapRef()[chromIndex].begin();
		it2 = varMap2.GetVarMapRef()[chromIndex].begin();
		
		// it1 = varMap1.varMap[chromIndex].begin();
		// it2 = varMap2.varMap[chromIndex].begin();
		
		it1_int = 0;
		it2_int = 0;
		numMatches = 0;
		numPartialMatches = 0;
		
		/////////////////////////////////////////////////////////////////////////////////
		// int i = 0;
		
		// int initSNPs = 0;
		// int initDels = 0;
		// int initInss = 0;
		
		/////////////////////////////////////////////////////////////////////////////////
		// for (vector<Var>::const_iterator it1_ = varMap1.GetVarMapRef()[chromIndex].begin() ; it1_ != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1_)
		// {
			// if (IsSNP(*it1_))
				// ++initSNPs;
			// else if (IsDel(*it1_))
				// ++initDels;
			// else if (IsIns(*it1_))
				// ++initInss;
		// }
		// cout << "map1: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl
		 // << "SNPs: " << initSNPs << endl
		 // << "Dels: " << initDels << endl
		 // << "Inss: " << initInss << endl;
		 // int iii = 0;
		 // cin >> iii;
		 /////////////////////////////////////////////////////////////////////////////////
		
		//compute set difference
		while ((it1 != varMap1.GetVarMapRef()[chromIndex].end()) && (it2 != varMap2.GetVarMapRef()[chromIndex].end()))
		// while ((it1 != varMap1.varMap[chromIndex].end()) && (it2 != varMap2.varMap[chromIndex].end()))
		{		
			// it1->Show();
			// int ii;
			// cin >> ii;
			
			// if (!IsSNP(*it1) /*&& !IsSNP(*it2)*/)
			// {
				// cout << "non SNP:" << endl;
				// it1->Show();
				// it2->Show();
				// cout << (*it1 < *it2) << "," << (*it2 < *it1) << "," << (*it1 == *it2) << endl;
				// int ii;
				// cout << "HERE" << endl;
				// cin >> ii;
			// }
		
			if (*it1 < *it2) //compares position
			{			
				varMap[chromIndex][it1_int] = *it1; //add variation to difference map
				++it1_int;
				
				++it1; // ++i;
			}
			else if (*it2 < *it1)
			{
				++it2; ++it2_int;
			}
			else //positions equal
			{			
				//check sequences, variation types match
				if (*it1 == *it2)
				{	
					// matchIndices[chromIndex].push_back(it2_int);
					matchIndices[chromIndex][numMatches] = it2_int;
					matchVarTypes[chromIndex][numMatches] = it1->GetvarType();
					++numMatches;
					
					if (IsSNP(*it1))
						++numSNPMatches;
				}
				else //partial match (variation types mismatch?)
				{	
					if (((IsSNP(*it1) && IsSNP(*it2))) && (2 > 1))// && (it1->GetOldSequence() == it2->GetOldSequence()))
					{
					// if (it1->GetvarType() == it2->GetvarType())
					
						// partialMatchIndices[chromIndex].push_back(it2_int);
						partialMatchIndices[chromIndex][numPartialMatches] = it2_int;
						partialMatchVarTypes[chromIndex][numPartialMatches] = it1->GetvarType();
						partialMatchOldSequences[chromIndex][numPartialMatches] = it1->GetOldSequence();
						partialMatchNewSequences[chromIndex][numPartialMatches] = it1->GetNewSequence();
						++numPartialMatches;
						// cout << "partial match:" << endl;
						// it1->Show(); cout << endl; it2->Show(); cout << endl << "******************" << endl;
						// int iii = 0; cin >> iii;
					}
					else ////add variation to difference map
					{
						varMap[chromIndex][it1_int] = *it1; //add variation to difference map
						++it1_int;
					}
				}
				++it1;// ++i;
				++it2; ++it2_int;
			}
		}
		//put remaining elements in map1 in diff map
		while (it1 != varMap1.GetVarMapRef()[chromIndex].end())
		{
			varMap[chromIndex][it1_int] = *it1; //add variation to difference map
			++it1; ++it1_int;
		}
			
		
		// cout << "------" << chromIndex << ", " << "i = " << i << endl;
		
		// cout << "HERE 6" << endl;
		// cout << "matches: " << numMatches << endl;
		//resize difference map
		varMap[chromIndex].resize(it1_int,padVar);
		// cout << "last element: " << endl;
		// if (varMap[chromIndex].size() > 0)
			// varMap[chromIndex][varMap[chromIndex].size()-1].Show();
		matchIndices[chromIndex].resize(numMatches);
		matchVarTypes[chromIndex].resize(numMatches);
		partialMatchIndices[chromIndex].resize(numPartialMatches);
		partialMatchVarTypes[chromIndex].resize(numPartialMatches);
		partialMatchOldSequences[chromIndex].resize(numPartialMatches);
		partialMatchNewSequences[chromIndex].resize(numPartialMatches);
	} while (false);
	
	//count matches
	//map1 counts
	int initSNPs = 0;
	int initDels = 0;
	int initInss = 0;
	//difference map counts
	int diffSNPs = 0;
	int diffDels = 0;
	int diffInss = 0;
	//total match counts
	int totalMatches = 0;
	int totalPartialMatches = 0;
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		for (vector<Var>::const_iterator it1 = varMap1.GetVarMapRef()[chromIndex].begin() ; it1 != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1)
		{
			if (IsSNP(*it1))
				++initSNPs;
			else if (IsDel(*it1))
				++initDels;
			else if (IsIns(*it1))
				++initInss;
		}
		
		for (vector<Var>::iterator itd = varMap[chromIndex].begin() ; itd != varMap[chromIndex].end() ; ++itd)
		{
			if (IsSNP(*itd))
				++diffSNPs;
			else if (IsDel(*itd))
				++diffDels;
			else if (IsIns(*itd))
				++diffInss;
		}
		
		totalMatches += matchIndices[chromIndex].size();
		totalPartialMatches += partialMatchIndices[chromIndex].size();
	}
	
	cout << "map1:" << endl
		 << "SNPs: " << initSNPs << endl
		 << "Dels: " << initDels << endl
		 << "Inss: " << initInss << endl;
	
	cout << "map diff:" << endl
		 << "SNPs: " << diffSNPs << endl
		 << "Dels: " << diffDels << endl
		 << "Inss: " << diffInss << endl;
	
	cout << "total matches: " << totalMatches << endl;
	cout << "total partial matches: " << totalPartialMatches << endl;
	cout << endl << "total SNP matches: " << numSNPMatches << endl << endl;
}

DifferenceMap::~DifferenceMap() //destructor
{	/*
	for (int i = 0 ; i < myChromNames.GetNumChromosomes() ; ++i)
	{
		vector<Var> emptyVarVec;
		varMap[i].swap(emptyVarVec);
		
		vector<int> v1;
		matchIndices[i].swap(v1);
		vector<int> v2;
		matchVarTypes[i].swap(v2);
		vector<int> v3;
		partialMatchIndices[i].swap(v3);
		vector<varType_> v4;
		partialMatchVarTypes[i].swap(v4);
		vector<string> v5;
		partialMatchOldSequences[i].swap(v5);
		vector<string> v6;
		partialMatchNewSequences[i].swap(v6);
	}*/
}

void DifferenceMap::ClearDifferenceMap()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//SumMap
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SumMap::Update(const DifferenceMap &diffMap, const VarMap &addMap, string chromName) //constructor, computes (map1 - map2) + map2
{
	cout << "S1";
	//construct map to add
	VarMap mergeMatchesMap; //submap of addMap to add to diffMap
	VarMap mergePartialMatchesMap;
	
	cout << "S2";
	
	Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
	// for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	int chromIndex = myChromNames.ChromNameToIndex(chromName);
	{
		cout << "S3";
		//preallocate space for matches map
		mergeMatchesMap.varMap[chromIndex].resize(diffMap.matchIndices[chromIndex].size(),padVar);
		cout << "S4";
		//preallocate space for partial matches map
		mergePartialMatchesMap.varMap[chromIndex].resize(diffMap.partialMatchIndices[chromIndex].size(),padVar);
		
		cout << "S5";
		
		//fill matches map
		int offset; //stores vector index, difference of iterators in vector
		for (vector<int>::const_iterator it1 = diffMap.matchIndices[chromIndex].begin() ; it1 != diffMap.matchIndices[chromIndex].end(); ++it1)
		{
			offset = it1 - diffMap.matchIndices[chromIndex].begin();
			mergeMatchesMap.varMap[chromIndex][offset] = addMap.varMap[chromIndex][*it1];
		}
		cout << "S6";
		//fill partial matches map
		for (vector<int>::const_iterator it1 = diffMap.partialMatchIndices[chromIndex].begin() ; it1 != diffMap.partialMatchIndices[chromIndex].end() ; ++it1)
		{
			offset = it1-diffMap.partialMatchIndices[chromIndex].begin();
			mergePartialMatchesMap.varMap[chromIndex][offset] = addMap.varMap[chromIndex][*it1];
			//change new, old sequences
			mergePartialMatchesMap.varMap[chromIndex][offset].SetOldSequence(
				diffMap.partialMatchOldSequences[chromIndex][offset]);
			mergePartialMatchesMap.varMap[chromIndex][offset].SetNewSequence(
				diffMap.partialMatchNewSequences[chromIndex][offset]);
		}
		cout << "S7";
		//merge maps
		vector<Var> mergedMatchesAndPartialMatches; //merge matches and partial matches
		mergedMatchesAndPartialMatches.resize(mergeMatchesMap.varMap[chromIndex].size() + mergePartialMatchesMap.varMap[chromIndex].size(),padVar);
		cout << "S8";
		merge(mergeMatchesMap.varMap[chromIndex].begin(),	    mergeMatchesMap.varMap[chromIndex].end(),
			  mergePartialMatchesMap.varMap[chromIndex].begin(),mergePartialMatchesMap.varMap[chromIndex].end(),
			  mergedMatchesAndPartialMatches.begin());
		cout << "S9";
		varMap[chromIndex].resize(
			diffMap.varMap[chromIndex].size() + mergedMatchesAndPartialMatches.size(),padVar);
		merge(diffMap.varMap[chromIndex].begin(),    diffMap.varMap[chromIndex].end(),
			  mergedMatchesAndPartialMatches.begin(), mergedMatchesAndPartialMatches.end(),
			  varMap[chromIndex].begin());		
		cout << "S10";
		if (chromIndex == 1)
		{
			// ShowEntries(chromIndex,10);
			// cout << "num entries: " << varMap[chromIndex].size() << endl;
		}
		cout << "S11";
	}
	cout << "S12";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
SumMap::SumMap(const DifferenceMap &diffMap, const VarMap &addMap) //constructor, computes (map1 - map2) + map2
{
	cout << "S1";
	//construct map to add
	VarMap mergeMatchesMap; //submap of addMap to add to diffMap
	VarMap mergePartialMatchesMap;
	
	cout << "S2";
	
	Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		cout << "S3";
		//preallocate space for matches map
		mergeMatchesMap.varMap[chromIndex].resize(diffMap.matchIndices[chromIndex].size(),padVar);
		cout << "S4";
		//preallocate space for partial matches map
		mergePartialMatchesMap.varMap[chromIndex].resize(diffMap.partialMatchIndices[chromIndex].size(),padVar);
		
		cout << "S5";
		
		//fill matches map
		int offset; //stores vector index, difference of iterators in vector
		for (vector<int>::const_iterator it1 = diffMap.matchIndices[chromIndex].begin() ; it1 != diffMap.matchIndices[chromIndex].end(); ++it1)
		{
			offset = it1 - diffMap.matchIndices[chromIndex].begin();
			mergeMatchesMap.varMap[chromIndex][offset] = addMap.varMap[chromIndex][*it1];
		}
		cout << "S6";
		//fill partial matches map
		for (vector<int>::const_iterator it1 = diffMap.partialMatchIndices[chromIndex].begin() ; it1 != diffMap.partialMatchIndices[chromIndex].end() ; ++it1)
		{
			offset = it1-diffMap.partialMatchIndices[chromIndex].begin();
			mergePartialMatchesMap.varMap[chromIndex][offset] = addMap.varMap[chromIndex][*it1];
			//change new, old sequences
			mergePartialMatchesMap.varMap[chromIndex][offset].SetOldSequence(
				diffMap.partialMatchOldSequences[chromIndex][offset]);
			mergePartialMatchesMap.varMap[chromIndex][offset].SetNewSequence(
				diffMap.partialMatchNewSequences[chromIndex][offset]);
		}
		cout << "S7";
		//merge maps
		vector<Var> mergedMatchesAndPartialMatches; //merge matches and partial matches
		mergedMatchesAndPartialMatches.resize(mergeMatchesMap.varMap[chromIndex].size() + mergePartialMatchesMap.varMap[chromIndex].size(),padVar);
		cout << "S8";
		merge(mergeMatchesMap.varMap[chromIndex].begin(),	    mergeMatchesMap.varMap[chromIndex].end(),
			  mergePartialMatchesMap.varMap[chromIndex].begin(),mergePartialMatchesMap.varMap[chromIndex].end(),
			  mergedMatchesAndPartialMatches.begin());
		cout << "S9";
		varMap[chromIndex].resize(
			diffMap.varMap[chromIndex].size() + mergedMatchesAndPartialMatches.size(),padVar);
		merge(diffMap.varMap[chromIndex].begin(),    diffMap.varMap[chromIndex].end(),
			  mergedMatchesAndPartialMatches.begin(), mergedMatchesAndPartialMatches.end(),
			  varMap[chromIndex].begin());		
		cout << "S10";
		if (chromIndex == 1)
		{
			// ShowEntries(chromIndex,10);
			// cout << "num entries: " << varMap[chromIndex].size() << endl;
		}
		cout << "S11";
	}
	cout << "S12";
}

SumMap::~SumMap() //destructor
{
}

DifferenceMapData::DifferenceMapData() //constructor
{
	//resize match, partial match indices vectors
	// preallocate space for list for each chromosome
	matchIndices.resize(myChromNames.GetNumChromosomes());
	matchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchIndices.resize(myChromNames.GetNumChromosomes());
	partialMatchVarTypes.resize(myChromNames.GetNumChromosomes());
	partialMatchOldSequences.resize(myChromNames.GetNumChromosomes());
	partialMatchNewSequences.resize(myChromNames.GetNumChromosomes());
}

DifferenceMapData::~DifferenceMapData() //destructor
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//free functions
////////////////////////////////////////////////////////////////////////////////////////////////////////

void GetDifferenceMap(const VarMap &varMap1, const VarMap &varMap2, VarMap &diffMap, DifferenceMapData &diffMapData) //computes difference map map1 - map2, stores results in diffMap, diffMapData
{	
	// cout << "HERE 1" << endl;
/*
	//iterators
	vector<Var>::const_iterator it1;
	vector<Var>::const_iterator it2;
	int it1_int; //counts entries in map1
	int it2_int; //counts entries in map2
	int numMatches;
	int numPartialMatches;
	
	// cout << "HERE 2" << endl;
	ChromNames myChromNames;
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		cout << "chrom: " << chromIndex << "," << myChromNames.IndexToChromName(chromIndex) << endl;
		//resize difference map
		Var padVar = Var(-1,SNP_,"Z","Z"); // used to pad vector of variations
		varMap[chromIndex].resize(varMap1.GetVarMapRef()[chromIndex].size() , padVar);
		
		// cout << "HERE 3" << endl;
		
		// cout << varMap[chromIndex].size() << endl;
		// cout << varMap2.GetVarMapRef()[chromIndex].size() << endl;
		
		matchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
		// matchIndices[chromIndex].resize(0);
		
		// cout << "HERE 4" << endl;
		
		partialMatchIndices[chromIndex].resize(varMap2.GetVarMapRef()[chromIndex].size());
	
		// cout << "HERE 5" << endl;
	
		it1 = varMap1.GetVarMapRef()[chromIndex].begin();
		it2 = varMap2.GetVarMapRef()[chromIndex].begin();
		it1_int = 0;
		it2_int = 0;
		numMatches = 0;
		numPartialMatches = 0;
		
		//compute set difference
		while (it1 != varMap1.GetVarMapRef()[chromIndex].end() && it2 != varMap2.GetVarMapRef()[chromIndex].end())
		{
			if (*it1 < *it2) //compares position
			{
				// varMap[chromIndex].push_back(*it1); //add variation to difference map
				varMap[chromIndex][it1_int] = *it1; //add variation to difference map
				
				// it1->Show();
				// cout << endl;
				// varMap[chromIndex][it1_int].Show();
				// cout << endl;
				// cout << (IsSNP(*it1)) << "," << (IsDel(*it1)) << "," << (IsIns(*it1)) << endl;
				// cout << (IsSNP(varMap[chromIndex][it1_int])) << "," << (IsDel(varMap[chromIndex][it1_int])) << "," << (IsIns(varMap[chromIndex][it1_int])) << endl;
				// int ii = 0;
				// cin >> ii;
				// varMap[chromIndex][it1_int] = *it1;
				++it1; ++it1_int;
			}
			else if (*it2 < *it1)
			{
				++it2; ++it2_int;
			}
			else //positions equal
			{
				// it1->Show(); cout << endl;
				// it2->Show(); cout << endl;
				
				// cout << it1->GetOldSequence().size() << "," << it1->GetNewSequence().size() << "," 
					 // << it2->GetOldSequence().size() << "," << it2->GetNewSequence().size() << endl;
					 
				// cout << "---------------------" << endl;
				
				// cout << (int)it1->GetNewSequence()[it1->GetNewSequence().size()-1] << endl;
				// cout << (int)it2->GetNewSequence()[it2->GetNewSequence().size()-1] << endl;
				
				// cout << (*it1 == *it2) << endl;
				// cout << "&&&&&&&&&&&&&&&&&&&&&" << endl;
				
				// int ii = 0;
				// cin >> ii;
			
				//check sequences, variation types match
				if (*it1 == *it2)
				{
					// matchIndices[chromIndex].push_back(it2_int);
					matchIndices[chromIndex][numMatches] = it2_int;
					++numMatches;
					// cout << "match:" << endl;
					// it1->Show(); cout << endl; it2->Show(); cout << endl << "******************" << endl;
				}
				else //partial match (variation types mismatch?)
				{
					if (it1->GetvarType() == it2->GetvarType())
					{
						// partialMatchIndices[chromIndex].push_back(it2_int);
						partialMatchIndices[chromIndex][numPartialMatches] = it2_int;
						partialMatchOldSequences[chromIndex][numPartialMatches] = it1->GetOldSequence();
						partialMatchNewSequences[chromIndex][numPartialMatches] = it1->GetNewSequence();
						++numPartialMatches;
						// cout << "partial match:" << endl;
						// it1->Show(); cout << endl; it2->Show(); cout << endl << "******************" << endl;
						// int iii = 0; cin >> iii;
					}
				}
				++it1;
				++it2;
			}
		}
		
		// cout << "HERE 6" << endl;
		cout << "matches: " << numMatches << endl;
		//resize difference map
		varMap[chromIndex].resize(it1_int,padVar);
		matchIndices[chromIndex].resize(numMatches);
		partialMatchIndices[chromIndex].resize(numPartialMatches);
	}
	
	//count matches
	//map1 counts
	int initSNPs = 0;
	int initDels = 0;
	int initInss = 0;
	//difference map counts
	int diffSNPs = 0;
	int diffDels = 0;
	int diffInss = 0;
	//total match counts
	int totalMatches = 0;
	int totalPartialMatches = 0;
	
	for (int chromIndex = 0; chromIndex < myChromNames.GetNumChromosomes() ; ++chromIndex)
	{
		for (vector<Var>::const_iterator it1 = varMap1.GetVarMapRef()[chromIndex].begin() ; it1 != varMap1.GetVarMapRef()[chromIndex].end() ; ++it1)
		{
			if (IsSNP(*it1))
				++initSNPs;
			else if (IsDel(*it1))
				++initDels;
			else if (IsIns(*it1))
				++initInss;
		}
		
		for (vector<Var>::iterator itd = varMap[chromIndex].begin() ; itd != varMap[chromIndex].end() ; ++itd)
		{
			if (IsSNP(*itd))
				++diffSNPs;
			else if (IsDel(*itd))
				++diffDels;
			else if (IsIns(*itd))
				++diffInss;
		}
		
		totalMatches += matchIndices[chromIndex].size();
		totalPartialMatches += partialMatchIndices[chromIndex].size();
	}
	
	cout << "map1:" << endl
		 << "SNPs: " << initSNPs << endl
		 << "Dels: " << initDels << endl
		 << "Inss: " << initInss << endl;
	
	cout << "map diff:" << endl
		 << "SNPs: " << diffSNPs << endl
		 << "Dels: " << diffDels << endl
		 << "Inss: " << diffInss << endl;
	
	cout << "total matches: " << totalMatches << endl;
	cout << "total partial matches: " << totalPartialMatches << endl;
	*/
}

void ReverseComplement(string &s) //reverses, complements nucleotide string
{
	//reverse string
	char temp; //holds value for swapping
	int length = s.length();
	for (int i = 0 ; i < length/2 ; ++i) //reverse only if at least two characters, don't need to reverse single character
	{
		temp = (s[length-i-1]);
		s[length-i-1] = s[i];
		s[i] = temp;
	}
	
	//complement string
	for (int i = 0 ; i < length ; ++i)
	{
		switch (s[i])
		{
			case 'A':
				s[i] = 'T';
				break;
			case 'T':
				s[i] = 'A';
				break;
			case 'C':
				s[i] = 'G';
				break;
			case 'G':
				s[i] = 'C';
				break;
		}
	}
}

//SNP::SNP(int position, string chromName, char oldVal, char



// void DifferenceMap::AddSNP(const SNP &SNPToAdd)
// {
	// SNPs.push_back(SNPToAdd);
// }

// void DifferenceMap::AddDel(const Del &DelToAdd)
// {
	// dels.push_back(DelToAdd);
// }

// void DifferenceMap::AddIns(const Ins &InsToAdd)
// {
	// inss.push_back(InsToAdd);
// }
