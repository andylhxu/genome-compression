//defines chromosome names set

#ifndef __ChromNames__
#define __ChromNames__

#include <set>
#include <map>
#include <sstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <utility>

using namespace std;

class ChromNames
{
private:
	// struct classcomp
	// {
		// bool operator() (const pair<string,int> &p1, const pair<string,int> &p2) const
		// {	
			// return (p1.first < p2.first);
		// }
	// };
	
	struct classcompString
	{
		bool operator() (const string &p1, const string &p2) const
		{	
			return (p1 < p2);
		}
	};
	
	struct classcompIndex
	{
		bool operator() (const int &p1, const int &p2) const
		{	
			return (p1 < p2);
		}
	};
	
	// bool fncomp (const pair<string,int> &p1, const pair<string,int> &p2) {return (p1.first < p2.first);}
	// bool(*fn_pt)(const pair<string,int>, const pair<string,int>) = fncomp;
	
	// set< pair<string,int> , bool(*)(pair<string,int>,pair<string,int>) > nameSet; //string name, numerical index
	// set< pair<string,int> , bool(*)(pair<string,int>,pair<string,int>) >::iterator it; //iterator
		
	// set< pair<string,int> , classcomp> nameSet; //string name, numerical index
	// set< pair<string,int> , classcomp>::iterator it; //iterator
	
	map<string,int,classcompString> nameToIndexMap;
	map<string,int,classcompString>::iterator it_nameToIndexMap;
	map<int,string,classcompIndex> indexToNameMap;
	map<int,string,classcompIndex>::iterator it_indexToNameMap;
	
	/*bool fncmp(string s1, string s2) {return (s1 < s2);}
	//bool (*fn_pt)(string, string) = fncmp;	
	map<string,int,bool(*)(string,string)> nameToIndexMap2(fncmp);*/
	
public:
	ChromNames();
	~ChromNames();
	
	void ShowChromNames();
	
	int ChromNameToIndex(string chromName);
	string IndexToChromName(int index);
	const int GetNumChromosomes() const {return nameToIndexMap.size();}
	
	//void speak() {cout << stringSet.size() << endl;}
};

#endif
