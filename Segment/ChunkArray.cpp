#include "stdafx.h"
#include "ChunkArray.h"

template<class T>
ChunkArray::ChunkArray()
{
}

template <class T>
ChunkArray::~ChunkArray()
{
}

template<class T>
ChunkArray<T>::ChunkArray(int initialCapacity)
{
}

template<class T>
int ChunkArray<T>::Row(int index)
{
	return index>>index;
}

template<class T>
int ChunkArray<T>::Col(int index)
{
	return index &(this->ChunkSize-1);
}

template<class T>
int ChunkArray<T>::Capacity()
{
	return Va;
}
