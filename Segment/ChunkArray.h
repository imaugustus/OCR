#pragma once
#include <vector>

using namespace std;

template <class T>
class ChunkArray
{
public:
	ChunkArray();
	ChunkArray(int initialCapacity);
	~ChunkArray();
	const int ChunkSize=4096;
	const int DivShift=12;
	vector< vector<T>> Values;
	int Count;
	int Add(T value);
	void Clear();
	T& getValues(int index);
	void setValues(int index, T value);
	void operator=(ChunkArray<T> right);

private:
	int Row(int index);
	int Col(int index);
	int Capacity();
};

template<class T>
ChunkArray<T>::ChunkArray()
{
}

template<class T>
ChunkArray<T>::ChunkArray(int initialCapacity)
{
	int chunks = (initialCapacity + ChunkSize - 1) / ChunkSize;
	for (int i = 0; i < chunks; i++) {
		vector<T> col_vec(ChunkSize);
		Values.push_back(col_vec);
	}
};


template <class T>
ChunkArray<T>::~ChunkArray()
{
}

template<class T>
int ChunkArray<T>::Add(T value)
{
	if (Count == Capacity()) {
		//vector< vector<T>> newValues(Values);
		//newValues.push_back(vector<T>(ChunkSize));
		//Values = newValues;
		Values.push_back(vector<T>(ChunkSize));
	}
	Values[Row(Count)][Col(Count)] = value;
	Count++;
	return Count - 1;
}

template<class T>
void ChunkArray<T>::Clear()
{
	Count = 0;
}

template<class T>
T& ChunkArray<T>::getValues(int index)
{
	return Values[Row(index)][Col(index)];
}

template<class T>
void ChunkArray<T>::setValues(int index, T value)
{
	Values[Row(index)][Col(index)] = value;
}

template<class T>
void ChunkArray<T>::operator=(ChunkArray<T> right)
{
	//ChunkSize = right.ChunkSize;
	// = right.DivShift;
	Values = right.Values;
	Count = right.Count;
}


template<class T>
int ChunkArray<T>::Row(int index)
{
	return index >> DivShift;
}

template<class T>
int ChunkArray<T>::Col(int index)
{
	return index & (ChunkSize - 1);
}

template<class T>
int ChunkArray<T>::Capacity()
{
	return Values.size()*ChunkSize;
}

