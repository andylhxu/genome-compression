//ChromNames implementation

#include "ChromNames.h"

////////////////////////////////////////////////////
//ChromNames
////////////////////////////////////////////////////

ChromNames::ChromNames() //constructor
{
	ostringstream numstr; //holds chromosome number
	for (int i = 1 ; i <= 22 ; i++)
	{
		numstr << i;
		numstr.seekp(0); //move put pointer back to front		
		nameToIndexMap.insert(make_pair("chr" + numstr.str(),i-1));
	}
	
	nameToIndexMap.insert(make_pair("chrM",23));
	nameToIndexMap.insert(make_pair("chrX",24));
	nameToIndexMap.insert(make_pair("chrY",25));
		
	// give index in string order	
	map<string,int,classcompString> nameToIndexMap_copy;
	
	int i = 0;
	for (it_nameToIndexMap = nameToIndexMap.begin(); it_nameToIndexMap != nameToIndexMap.end() ; ++it_nameToIndexMap)
	{
		nameToIndexMap_copy.insert(make_pair(it_nameToIndexMap->first,i));
		++i;
	}
	nameToIndexMap = nameToIndexMap_copy;
	
	// make inverse map
	for (it_nameToIndexMap = nameToIndexMap.begin() ; it_nameToIndexMap != nameToIndexMap.end() ; ++it_nameToIndexMap)
	{
		indexToNameMap.insert(make_pair(it_nameToIndexMap->second,it_nameToIndexMap->first));
	}
}

ChromNames::~ChromNames() //destructor
{
}

void ChromNames::ShowChromNames()
{
	cout << "Chromosome names:" << endl << "(index, name)" << endl;
	cout << "----------------" << endl;
	cout << "size: " << nameToIndexMap.size() << endl;
	cout << "size: " << indexToNameMap.size() << endl;
	for (it_nameToIndexMap = nameToIndexMap.begin() ; it_nameToIndexMap != nameToIndexMap.end() ; ++it_nameToIndexMap)
	{
		cout << "(" << (*it_nameToIndexMap).second << "," << (*it_nameToIndexMap).first << ")" << endl;
	}
	cout << "----------------" << endl;
}

int ChromNames::ChromNameToIndex(string chromName)
{
	it_nameToIndexMap = nameToIndexMap.find(chromName);
	if (it_nameToIndexMap == nameToIndexMap.end())
		return -1;
	else
		return it_nameToIndexMap->second;
}

string ChromNames::IndexToChromName(int index)
{
	it_indexToNameMap = indexToNameMap.find(index);
	if (it_indexToNameMap == indexToNameMap.end())
		return "empty";
	else
		return it_indexToNameMap->second;
	
	// for (it = nameSet.begin(); it != nameSet.end(); ++it)
	// {
		// if (it->second == index)
			// chromName = it->first;
	// }
	
	// return chromName;
}
