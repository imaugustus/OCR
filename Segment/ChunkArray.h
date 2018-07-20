#pragma once
#include <SuggestionStage.h>

using namespace std;

template <class T>
class ChunkArray
{
public:
	ChunkArray();
	~ChunkArray();
	int ChunkSize;
	int DivShift;
	T Values[][];
	int Count;
	ChunkArray(int initialCapacity);
	int Add(T value);
	void Clear();
	T operator[](int index);

private:
	int Row(int index);
	int Col(int index);
	int Capacity();

};

