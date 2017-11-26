//difference map class

#ifndef __DifferenceMap__
#define __DifferenceMap__

#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <fstream>
#include <string.h>
#include <algorithm>

#include "ChromNames.h"
// #include "Hist.h"

using namespace std;

enum varType_ {SNP_ = '0', DEL_ = '1', INS_ = '2', INDEL_ = '3'};

class Var //variation class
{
private:
	int position; //position in chromosome
	varType_ varType; //SNP, DEL, or INS
	string oldSeq; //reference sequence
	string newSeq; //new sequence
	
	// char* oldSeq_; //reference sequence
	// char* newSeq_; //new sequence
public:
	// string oldSeq__(){return string(oldSeq_);};
	// string newSeq__(){return string(newSeq_);};
	
	// void SetOldSeq(string seq){seq.copy(oldSeq_,seq.size());};
	// void SetNewSeq(string seq){seq.copy(newSeq_,seq.size());};

	Var(int position, varType_ varType, string oldSeq, string newSeq) : 
		position(position), varType(varType), oldSeq(oldSeq), newSeq(newSeq) {} //constructor
	
	int GetPosition() const {return position;}
	varType_ GetvarType() const {return varType;}
	string GetOldSequence() const {return oldSeq;}
	string GetNewSequence() const {return newSeq;}
	
	bool IsSNP () const {return (varType == SNP_);}
	bool IsDel () const {return (varType == DEL_);}
	bool IsIns () const {return (varType == INS_);}
	bool IsIndel () const {return (varType == INDEL_);}
	
	// void SetPosition(int position) {this->position = position;}
	void SetOldSequence(string oldSeq) {this->oldSeq = oldSeq;}
	void SetNewSequence(string newSeq) {this->newSeq = newSeq;}
		
	void Show() const {cout << "(" << (char)varType << "," << position << "," << oldSeq << "," << newSeq << ")" << endl;}
	
	bool operator< (const Var &v_other) const;// {return ((this->position < v_other.position) || ((this->position == v_other.position) && (this->IsSNP())));} //((this->position < v_other.position) ? (true) : ((this->position > v_other.position) ? (false) : (this->IsSNP())));
	bool operator== (const Var &v_other) const;/* {return ((this->position == v_other.position) && 
														(this->varType == v_other.varType) && 
														(this->oldSeq == v_other.oldSeq) && 
														(this->newSeq == v_other.newSeq));}*/
	
	// comparison operator
	// struct classcomp
	// {
		// bool operator() (const Var &v1, const Var &v2) const
		// {	
			// return (0 < 1);
		// }
	// };
};

class VarMap //variation set by chromosomes
{
protected:
	ChromNames myChromNames;
public:
	vector< vector<Var> > varMap;
	VarMap(); //constructor
	// VarMap(const VarMap &addMap, const vector< vector<int> > &matchIndices, const vector< vector<int> > &partialMatchIndices); //constructor, takes 
	// VarMap(const DifferenceMap &, const VarMap &);
	// VarMap(const sdgsadg &, const int &);
	// VarMap(const VarMap &); //copy constructor
	~VarMap(); //destructor
	
	// void PushBackVar(const Var &varToInsert, const int &chromIndex);
	void ShowEntries(int chromIndex, int numEntriesShow); //shows entries
	const vector< vector<Var> > &GetVarMapRef() const {return varMap;}
	
	void WriteVarMapToFile(const char *outfilename); //writes Watson VarMap in short form to file with name outfilename
	void WriteVarMapToFile2(const char *outfilename); //writes Watson VarMap in short form to file with name outfilename, no stable partition by variation type
	
	void Clear(); // clears variation map{varMap.clear();}
};

class VarMapWatson : public VarMap //variation map from Watson
{
private:
public:
	VarMapWatson(const char *Watsonfilename); //constructor
	~VarMapWatson(); //destructor
	
	void ClearWatson();
};

class VarMapdbSNP : public VarMap //variation map from dbSNP
{
private:
public:
	// VarMapdbSNP(); //constructor
	VarMapdbSNP(char *dbSNPDirectory); //constructor
	VarMapdbSNP(char *dbSNPDirectory, string chromName, bool usedbSNP); //constructor, read single chromosome with name chromName
	~VarMapdbSNP(); //destructor
	
	// void 
};

class DifferenceMap : public VarMap //holds difference map between two instances of VarMap
{
private:
public:
	DifferenceMap(); //constructor
	DifferenceMap(const VarMap &varMap1, const VarMap &varMap2); //constructor, computes map1 - map2, indices in map2
	DifferenceMap(const VarMap &varMap1, const VarMap &varMap2, const string chromName); //constructor, computes map1 - map2, indices in map2
	void Update(const VarMap &varMap1, const VarMap &varMap2, const string chromName);
	~DifferenceMap(); //destructor	
	
	vector< vector<int> > matchIndices; //indices of matches in map2
	vector< vector<int> > matchVarTypes; //var types of matches in map2
	
	vector< vector<int> > partialMatchIndices; //indices of partial matches in map2
	vector< vector<varType_> > partialMatchVarTypes; //var types of partial matches in map1
	vector< vector<string> > partialMatchOldSequences;  //old sequences of partial matches in map1
	vector< vector<string> > partialMatchNewSequences; //new sequences of partial matches in map1
	
	// const vector< vector<int> > &GetMatchIndicesRef() const {return matchIndices;}
	// const vector< vector<int> > &GetPartialMatchIndicesRef() const {return partialMatchIndices;}
	// const vector< vector<string> > &GetPartialMatchOldSequencesRef() const {return partialMatchOldSequences;}
	// const vector< vector<string> > &GetPartialMatchNewSequencesRef() const {return partialMatchNewSequences;}
	
	DifferenceMap operator= (const DifferenceMap &other);
	
	void ClearDifferenceMap();
};

class SumMap : public VarMap //adds difference map map1-map2 and map2
{
private:
	// VarMap mergeMatchesMap;
	// VarMap mergePartialMatchesMap;
public:
	SumMap(){}; // constructor
	SumMap(const DifferenceMap &, const VarMap &); //constructor, computes (map1 - map2) + map2
	
	void Update(const DifferenceMap &, const VarMap &, string chromName); //computes (map1 - map2) + map2 , computes for single chromosome with name chromName
	~SumMap(); //destructor
};

class DifferenceMapData //holds difference map data between two instances of VarMap
{
private:
	ChromNames myChromNames;
public:
	vector< vector<int> > matchIndices; //indices of matches in map2
	vector< vector<int> > matchVarTypes; //var types of matches in map2
	vector< vector<int> > partialMatchIndices; //indices of partial matches in map2
	vector< vector<varType_> > partialMatchVarTypes; //var types of partial matches in map1
	vector< vector<string> > partialMatchOldSequences;  //old sequences of partial matches in map1
	vector< vector<string> > partialMatchNewSequences; //new sequences of partial matches in map1
	
	DifferenceMapData(); //constructor
	~DifferenceMapData(); //destructor
};

	

// class DifferenceMap
// {
// private:
	// vector<SNP> SNPs; //SNPs
	// vector<Del> dels; //deletions
	// vector<Ins> inss; //insertions
	
	// iterators
	// vector<SNP>::iterator itSNP; 
	// vector<Del>::iterator itDel;
	// vector<Ins>::iterator itIns;
// public:
	// DifferenceMap();
	// ~DifferenceMap();
	
	// void InitializeFromLongForm(char *filename); //initialize from JWB submission format
	// void InitializeFromShortForm(char *filename); //initialize from short form parsed JWB format
	
	// void AddSNP(const SNP &SNPToAdd);
	// void AddDel(const Del &DelToAdd);
	// void AddIns(const Ins &InsToAdd);
// };

////////////////////////////////////////////////////////////////////////////////////////////////////////
//free functions
////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsSNP (Var const &var);
bool IsDel (Var const &var);
bool IsIns (Var const &var);

void GetDifferenceMap(const VarMap &varMap1, const VarMap &varMap2, VarMap &diffMap, DifferenceMapData &diffMapData); //computes difference map map1 - map2, stores results in diffMap, diffMapData

void ReverseComplement(string &s); //reverses, complements nucleotide string

//enum nuc {A,C,G,T}; //nucleotides
////////////////////////////////////////////////////
class SNP //SNP class
{
private:
	int position; //position in chromosome
	string chromName; //chromosome name
	char oldVal; //previous value, ACGT
	char newVal; //new value, ACGT
public:
	SNP(int position, string chromName, char oldVal, char newVal) : 
		position(position), chromName(chromName), oldVal(oldVal), newVal(newVal) {} //constructor
	
	int GetPosition() const {return position;}
	string GetChromName() const {return chromName;}
	char GetOldVal() const {return oldVal;}
	char GetNewVal() const {return newVal;}
	
	void SetPosition(int &position) {this->position = position;}
	void SetChromName(string &chromName) {this->chromName = chromName;}
	void SetOldVal(char &oldVal) {this->oldVal = oldVal;}
	void SetNewVal(char &newVal) {this->newVal = newVal;}
};

////////////////////////////////////////////////////
class Del //deletion class
{
private:
	int position; //position in chromosome
	string chromName; //chromosome name
	string delSequence; //deletion sequence
public:
	Del(int position, string chromName, string delSequence) : 
		position(position), chromName(chromName), delSequence(delSequence) {} //constructor
	
	int GetPosition() const {return position;}
	string GetChromName() const {return chromName;}
	string GetDelSequence() const {return delSequence;}
	
	void SetPosition(int &position) {this->position = position;}
	void SetChromName(string &chromName) {this->chromName = chromName;}
	void SetDelSequence(string &delSequence) {this->delSequence = delSequence;}
};

////////////////////////////////////////////////////
class Ins //insertion class
{
private:
	int position; //position in chromosome
	string chromName; //chromosome name
	string insSequence; //insertion sequence
public:
	Ins(int position, string chromName, string insSequence) : 
		position(position), chromName(chromName), insSequence(insSequence) {} //constructor
	
	int GetPosition() const {return position;}
	string GetChromName() const {return chromName;}
	string GetInsSequence() const {return insSequence;}
	
	void SetPosition(const int &position) {this->position = position;}
	void SetChromName(const string &chromName) {this->chromName = chromName;}
	void SetInsSequence(const string &insSequence) {this->insSequence = insSequence;}
};

////////////////////////////////////////////////////
//dbSNP entry
////////////////////////////////////////////////////

class dbSNPEntry
{
};

class dbSNP
{
private:
	vector<SNP> SNPs; //SNPs
public:
	dbSNP(char *dbSNPDirectory); //initialize from dbSNP directory, read all .txt files in directory

};

#endif
