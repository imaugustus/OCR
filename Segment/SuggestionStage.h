#pragma once
#include <string>
#include <unordered_map>
#include <ChunkArray.h>


using namespace std;

class SuggestionStage
{
public:
	SuggestionStage();
	SuggestionStage(int initialCapacity);
	~SuggestionStage();
	int DeleteCount();
	int NodeCount();
	void Clear();
	void Add(int deleteHash, string suggestion);
	void CommitTo(unordered_map<int, vector<string>> *permanentDeletes);

	class Node
	{
	public:
		string suggestion;
		int next;
		Node();
		Node(string suggestion, int next);
	};

	class Entry
	{
	public:
		int count;
		int first;
		Entry();
		Entry(int count, int first);
	};

private:
	unordered_map<int, Entry> Deletes;
	ChunkArray<Node> Nodes;

};

