#pragma once
#include <string>
#include <unordered_map>
#include <ChunkArray.h>

using namespace std;

class SuggestionStage
{
public:
	SuggestionStage();
	~SuggestionStage();
	typedef struct Node
	{
	public:
		string suggestion;
		int next;
	}Node;
	typedef struct  Entry {
	public:
		int count;
		int first;

	}Entry;

private:
	unordered_map<int, Entry> Deletes;
	ChunkArray<Node> Nodes;

};

