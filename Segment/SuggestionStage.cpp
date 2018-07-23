#include "stdafx.h"
#include "SuggestionStage.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

SuggestionStage::SuggestionStage()
{
}

SuggestionStage::SuggestionStage(int initialCapacity)
{
	Deletes = unordered_map<int, Entry>(initialCapacity);
	ChunkArray<Node> temp(initialCapacity * 2);
	Nodes = temp;
}


SuggestionStage::~SuggestionStage()
{
}

int SuggestionStage::DeleteCount()
{
	return	Deletes.size();
}

int SuggestionStage::NodeCount()
{
	return Nodes.Count;
}

void SuggestionStage::Clear()
{
	Deletes.clear();
	Nodes.Clear();
}

void SuggestionStage::Add(int deleteHash, string suggestion)
{
	unordered_map<int, Entry>::iterator iter= Deletes.find(deleteHash);
	Entry entry;
	if (iter != Deletes.end()) {
		entry =iter->second;
	}
	else {
		entry = { 0,-1 };
	}
	int next = entry.first;
	entry.count++;
	entry.first = Nodes.Count;
	Deletes[deleteHash] = entry;;
	Nodes.Add(Node{suggestion, next});
}

void SuggestionStage::CommitTo(unordered_map<int, vector<string>> permanentDeletes)
{
	unordered_map<int, Entry>::iterator iter_out;
	unordered_map<int, vector<string>>::iterator iter_in;
	for(iter_out=Deletes.begin(); iter_out!=Deletes.end(); iter_out++)
	{
		int i;
		int key = iter_out->first;
		Entry value = iter_out->second;
		vector<string> suggestion;
		iter_in = permanentDeletes.find(key);
		if (iter_in!=permanentDeletes.end()) {
			suggestion = iter_in->second;
			i = suggestion.size();
			vector<string> newSuggestion(i + value.count);
			newSuggestion.insert(newSuggestion.end(), suggestion.begin(),suggestion.end());
			permanentDeletes[key] = newSuggestion;
		}
		else {
			i = 0;
			vector< string> temp =vector<string>(value.count);
			suggestion = temp;
			permanentDeletes[key] = suggestion;
		}
		int next = value.first;
		Node node;
		while (next >= 0) {
			node = Nodes.getValues(next);
			suggestion[i] = node.suggestion;
			next = node.next;
			i++;
		}
	}
}

SuggestionStage::Node::Node()
{
}

SuggestionStage::Entry::Entry()
{
}
