//histogram class, histogram for objects with <, == operators
//objects must be orderable

#ifndef __Hist__
#define __Hist__

#include <vector>
#include <map>

// using std::vector;
// using std::map;
// using namespace std;
using std::vector;
using std::map;

template <class T>
class Hist
{
private:
public:
	map<T,int> histMap;
	
	Hist(); //constructor
	Hist(const vector <T> &); //computes histogram, calls UpdateHist
	~Hist(); //destructor
	
	void UpdateHist (const vector <T> &); //computes histogram
};

// template <class T>
// void Hist<T>::UpdateHist (vector <T> & vec);
// template <class T>
// void Hist<T>::UpdateHist2();

#include "Hist.cpp"

#endif
