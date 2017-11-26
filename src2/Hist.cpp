//implementation for Hist class

#include "Hist.h"

template <class T>
Hist<T>::Hist() //constructor
{
}

template <class T>
Hist<T>::Hist(const vector <T> &vec) //computes histogram, calls UpdateHist
{
	UpdateHist(vec);
}

template <class T>
Hist<T>::~Hist() //destructor
{
}

template <class T>
void Hist<T>::UpdateHist (const vector <T> &vec)
{
	// loop over vector	
	typename map<T,int>::iterator map_it;
	for (typename vector<T>::const_iterator vec_it = vec.begin() ; vec_it != vec.end() ; ++vec_it)
	{
		map_it = histMap.find(*vec_it);	
		if (map_it == histMap.end()) //new value, add to map, 1 count
			histMap.insert(make_pair(*vec_it,1));
		else //increment count
			map_it->second += 1;
	}
}
