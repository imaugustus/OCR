#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <EditDistance.h>
#include <vector>
#include <set>
#include <SuggestItem.h>
#include <SuggestionStage.h>

using namespace std;

class SymSpellCheck
{
public:
	SymSpellCheck(int initialCapacity, int maxDictionaryEditDistance, int prefixLength, int countThreshold, byte compactLevel);
	~SymSpellCheck();
	enum Verbosity
	{
		Top,
		Closest,
		All,
	};
	bool CreateDictionaryEntry(string key, unsigned long long int count, SuggestionStage staging);
	bool LoadDictionary(string corpus);
	bool CreateDictionary(string corpus);
	void PurgeBelowThresholdWords();
	void CommitStaged(SuggestionStage staging);
	vector<SuggestItem> Lookup(string input, Verbosity verbosity);
	vector<SuggestItem> Lookup(string input, Verbosity verbosity, int maxEditDistance);
	vector<SuggestItem> Lookup(string input, Verbosity verbosity, int maxEditDistance, bool includeUnknown);


private:
	const int defaultMaxEditDistance = 2;
	const int defaultPrefixLength = 7;
	const int defaultCountThreshold = 1;
	const int defaultInitialCapacity = 16;
	const int defaultCompactLevel = 5;
	int initialCapacity;
	int maxDictionaryEditDistance;
	int prefixLength; //prefix length  5..7
	unsigned long long int countThreshold; //a treshold might be specifid, when a term occurs so frequently in the corpus that it is considered a valid word for spelling correction
	unsigned int compactMask;
	int maxDictionaryWordLength;
	map<int, vector<string>> deletes;
	map<string, unsigned long long int> words;
	map<string, unsigned long long int> belowThresholdWords;
	bool DeleteInSuggestionPrefix(string delete_str, int deleteLen, string suggestion, int suggestionLen);
	vector<string> ParseWords(string test);
	set<string> Edits(string word, int editDistance, set<string> deleteWords);
	set<string> EditsPrefix(string key);
	int GetStringHash(string s);
};

