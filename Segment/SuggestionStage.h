#pragma once
#include <string>
#include <unordered_map>

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

};

