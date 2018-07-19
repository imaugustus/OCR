#pragma once
#include <map>
#include <string>
#include <EditDistance.h>

using namespace std;

class SymSpellCheck
{
public:
	SymSpellCheck();
	~SymSpellCheck();
	const int defaultMaxEditDistance = 2;
	const int defaultPrefixLength = 7;
	const int defaultCountThreshold = 1;
	const int defaultInitialCapacity = 16;
	const int defaultCompactLevel = 5;
	int max_dict_word_length;
	map<string, unsigned long long int> dictionary;
	map<string, unsigned long long int>::iterator dict_iter;

private:
	int initialCapacity;
	int maxDictionaryEditDistance;
	int prefixLength; //prefix length  5..7
	unsigned long long int countThreshold; //a treshold might be specifid, when a term occurs so frequently in the corpus that it is considered a valid word for spelling correction
	unsigned int compactMask;
};

