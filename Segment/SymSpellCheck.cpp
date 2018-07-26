#include "stdafx.h"
#include "SymSpellCheck.h"
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>


using namespace std;

SymSpellCheck::SymSpellCheck()
{
}

SymSpellCheck::SymSpellCheck(int initialCapacity, int maxDictionaryEditDistance, int prefixLength, int countThreshold)
{
	if (initialCapacity < 0) initialCapacity = defaultInitialCapacity;
	if (maxDictionaryEditDistance < 0) maxDictionaryEditDistance = defaultMaxEditDistance;
	if (prefixLength < 1 || prefixLength <= maxDictionaryEditDistance) prefixLength = defaultPrefixLength;
	if (countThreshold < 0) countThreshold = defaultCountThreshold;

	this->initialCapacity = initialCapacity;
	//this->words = unordered_map<string, unsigned long long int>();
	this->maxDictionaryEditDistance = maxDictionaryEditDistance;
	this->prefixLength = prefixLength;
	this->countThreshold = defaultCountThreshold;
	this->compactMask = (0xffffffff >> (3 + defaultCompactLevel)) << 2;
}

SymSpellCheck::~SymSpellCheck()
{
}

bool SymSpellCheck::CreateDictionaryEntry(string key, unsigned long long int count, SuggestionStage *staging)
{
	if (count <= 0) {
		if (this->countThreshold > 0) return false;
		count = 0;
	}
	unsigned long long int countPrevious;
	if (countThreshold > 1 && belowThresholdWords.count(key)) {
		countPrevious = belowThresholdWords[key];
		// calculate new count for below threshold word
		count = ((numeric_limits<unsigned long long int>::max() - countPrevious) > count) ? (countPrevious + count) : numeric_limits<unsigned long long int>::max();
		// has reached threshold - remove from below threshold collection (it will be added to correct words below)
		if (count >= countThreshold) {
			belowThresholdWords.erase(key);
		}
		else {
			belowThresholdWords[key]=count; // = count;
			return false;
		}
	}
	else if (words.count(key)) {
		countPrevious = words[key];
		// just update count if it's an already added above threshold word
		count = ((numeric_limits<unsigned long long int>::max() - countPrevious) > count) ? (countPrevious + count) : numeric_limits<unsigned long long int>::max();
		words[key]=count;
		return false;
	}
	else if (count < countThreshold) {
		// new or existing below threshold word
		belowThresholdWords[key]=count;
		return false;
	}

	// what we have at this point is a new, above threshold word
	words[key]=count;
	if (key == "can't") cout << "Added to words..!" << endl;

	//edits/suggestions are created only once, no matter how often word occurs
	//edits/suggestions are created only as soon as the word occurs in the corpus,
	//even if the same term existed before in the dictionary as an edit from another word
	if (key.length() > maxDictionaryWordLength) maxDictionaryWordLength = key.length();

	//create deletes
	unordered_set<string> edits = EditsPrefix(key);

	// if not staging suggestions, put directly into main data structure
	if (staging!=NULL) {
		for (auto delete_str:edits)
		{
			(*staging).Add(GetStringHash(delete_str), key);
		};
	}
	else {
		//if (!&deletes) this->deletes = unordered_map<int, vector<string>>(); //initialisierung

		for(string delete_str : edits){
			int deleteHash = GetStringHash(delete_str);
			vector<string> suggestions;
			if (deletes.count(deleteHash)) {
				suggestions = deletes[deleteHash];
				vector<string> newSuggestions(suggestions.size()+1);
				newSuggestions.insert(newSuggestions.begin(), suggestions.begin(), suggestions.end());
				deletes[deleteHash]=newSuggestions;
				suggestions = newSuggestions;
			}
			else {
				suggestions = vector<string>(1);
				deletes[deleteHash]=suggestions;
			}
			suggestions[suggestions.size() - 1] = key;
		};
	}
	return true;
}

bool SymSpellCheck::LoadDictionary(string corpus)
{
	ifstream infile(corpus);
	if (!infile.good()) return false;
	SuggestionStage staging(16384);
	string key;
	string count;
	stringstream ss;
	unsigned long long int num;
	while (infile>>key>>count) {
		if (count!="") {
			ss<<count;
			ss >> num;
			ss.str("");
			ss.clear();
			cout << "num " << num << endl;
			cout << "key " << key << endl;
			CreateDictionaryEntry(key, num, &staging);
		}
	}
	//if (!&(this->deletes)) this->deletes = unordered_map<int, vector<string>>();
	CommitStaged(&staging);
	return true;
}

bool SymSpellCheck::CreateDictionary(string corpus)
{
	ifstream infile(corpus);
	if (!infile.good()) return false;
	SuggestionStage staging(16384);
	string line;
	while (getline(infile, line))
	{
		for (string key : ParseWords(line)) {
			CreateDictionaryEntry(key, 1, &staging);
		}
	}
	//if (!(&(this->deletes))) this->deletes = unordered_map<int, vector<string>>(staging.DeleteCount);
	CommitStaged(&staging);
	return true;
}

void SymSpellCheck::PurgeBelowThresholdWords()
{
	belowThresholdWords = unordered_map<string, unsigned long long int>();
}

void SymSpellCheck::CommitStaged(SuggestionStage *staging)
{
	(*staging).CommitTo(deletes);
}

vector<SuggestItem> SymSpellCheck::Lookup(string input, Verbosity verbosity)
{
	return Lookup(input, verbosity, this->maxDictionaryEditDistance, false);
}

vector<SuggestItem> SymSpellCheck::Lookup(string input, Verbosity verbosity, int maxEditDistance)
{
	return Lookup(input, verbosity, maxEditDistance, false);
}

vector<SuggestItem> SymSpellCheck::Lookup(string input, Verbosity verbosity, int maxEditDistance, bool includeUnknown)
{
	if (maxEditDistance > this->maxDictionaryEditDistance) throw invalid_argument("Dist to big:" + maxEditDistance);
	vector<SuggestItem> suggestions;
	int inputLen = input.length();
	if ((inputLen - maxEditDistance) > maxDictionaryWordLength) return suggestions;
	unordered_set<string> consideredDeletes;
	unordered_set<string> consideredSuggestions;
	long int suggestionCount;

	if (words.count(input)) {
		suggestionCount = words[input];
		suggestions.push_back(SuggestItem(input, 0, suggestionCount));
		if (verbosity != All) return suggestions;
	}
	consideredSuggestions.insert(input);
	
	int maxEditDistance2 = maxEditDistance;
	//int candidatePointer = 0;
	vector<string> candidates;
	vector<string>::iterator candidatePointer = candidates.begin();

	int inputPrefixLen = inputLen;
	if (inputPrefixLen > prefixLength) {
		inputPrefixLen = prefixLength;
		candidates.push_back(input.substr(0, inputPrefixLen));
	}
	else {
		candidates.push_back(input);
	}

	EditDistance distanceComparer(input, this->distanceAlgorithm);
	while (candidatePointer != candidates.end()) {
		string candidate = *candidatePointer++;
		int candidateLen = candidate.length();
		int lengthDiff = inputPrefixLen - candidateLen;

		if (lengthDiff > maxEditDistance2) {
			if (verbosity == All) continue;
			break;
		}

		if (deletes.count(GetStringHash(candidate))) {
			vector<string> dictSuggestions = deletes[GetStringHash(candidate)];
			for (string suggestion : dictSuggestions) {
				if (suggestion == input) continue;
				int suggestionLen = suggestion.length();

				if (abs(suggestionLen - inputLen) > maxEditDistance2
					|| (suggestionLen < candidateLen)
					|| ((suggestionLen == candidateLen) && (suggestion != candidate)))
					continue;
				int suggPrefixLen = min(suggestionLen, prefixLength);
				if (suggPrefixLen > inputPrefixLen && (suggPrefixLen - candidateLen) > maxEditDistance2) continue;
				int distance;
				int min_t = 0;
				if (candidateLen == 0) {
					distance = max(inputLen, suggestionLen);
					if (distance > maxEditDistance2 || !consideredSuggestions.insert(suggestion).second) continue;
				}
				else if (suggestionLen==1) {
					if (input.find(suggestion.at(0)) < 0) distance = inputLen;
					else distance = inputLen - 1;
					if (distance > maxEditDistance2 || !consideredSuggestions.insert(suggestion).second) continue;
				}
				else{ 
					if ((prefixLength - maxEditDistance == candidateLen)
						&& (((min_t = min(inputLen, suggestionLen) - prefixLength) > 1)
							&& !(input.substr(inputLen + 1 - min_t)==(suggestion.substr(suggestionLen + 1 - min_t))))
						|| ((min_t > 0) && (input.at(inputLen - min_t) != suggestion.at(suggestionLen - min_t))
							&& ((input.at(inputLen - min_t - 1) != suggestion.at(suggestionLen - min_t))
								|| (input.at(inputLen - min_t) != suggestion.at(suggestionLen - min_t - 1))))) {
						continue;
					}
					else {
						// deleteInSuggestionPrefix is somewhat expensive, and only pays off when verbosity is Top or Closest.
						if ((verbosity != All && !DeleteInSuggestionPrefix(candidate, candidateLen, suggestion, suggestionLen))
							|| !consideredSuggestions.insert(suggestion).second) continue;
						distance = distanceComparer.compare(suggestion, maxEditDistance2);
						if (distance < 0) continue;
					}
					if (distance <= maxEditDistance2) {
						suggestionCount = words[suggestion];
						SuggestItem si(suggestion, distance, suggestionCount);
						if (suggestions.size() > 0) {
							switch (verbosity) {
							case Closest:
								//we will calculate DamLev distance only to the smallest found distance so far
								if (distance < maxEditDistance2) suggestions.clear();
								break;
							case Top:
								if (distance < maxEditDistance2 || suggestionCount > suggestions[0].count) {
									maxEditDistance2 = distance;
									suggestions[0]=si;
								}
								continue;
							}
						}
						if (verbosity != All) maxEditDistance2 = distance;
						suggestions.push_back(si);
					}
				}
			}
			if ((lengthDiff < maxEditDistance) && (candidateLen <= prefixLength))
			{
				//save some time
				//do not create edits with edit distance smaller than suggestions already found
				if (verbosity != All && lengthDiff >= maxEditDistance2) continue;

				for (int i = 0; i < candidateLen; i++)
				{
					string delete_str = candidate.erase(i, 1);

					if (consideredDeletes.insert(delete_str).second) { candidates.push_back(delete_str); }
				}
			}
		}
	}
	if (suggestions.size() > 1) sort(suggestions.begin(), suggestions.end(), [](const SuggestItem &lhs, SuggestItem &rhs)->bool {
		if (lhs.distance == rhs.distance) return lhs.count>rhs.count;
		return lhs.distance>rhs.distance;
	});
	return suggestions;
}

vector<SuggestItem> SymSpellCheck::lookupCompound(string input, int maxEditDistance)
{
	if(maxEditDistance>maxDictionaryEditDistance) throw invalid_argument("Dist to big " + maxEditDistance);
	vector<string> termList1 = ParseWords(input);
	vector<SuggestItem> suggestions;
	vector<SuggestItem> suggestionParts;
	vector<SuggestItem> suggestionsCombi;
	EditDistance editDistance;

	bool lastCombi = false;
	for (int i = 0; i < termList1.size(); i++) {
		suggestions = Lookup(termList1[i], Top, maxEditDistance);
		if ((i > 0) && !lastCombi) {
			suggestionsCombi = Lookup(termList1[i - 1] + termList1[i], Top, maxEditDistance);
			if (!suggestionsCombi.empty()) {
				SuggestItem best1 = suggestionParts[suggestionParts.size() - 1];
				SuggestItem best2;
				if (!suggestions.empty()) best2 = suggestions[0];
				else best2 = SuggestItem(termList1[i], maxEditDistance + 1, 0);

				editDistance = EditDistance(termList1[i - 1] + " " + termList1[i], Damerau);
				if ((suggestionsCombi[0].distance + 1) < editDistance.DamerauLevenshteinDistance(best1.term + " " + best2.term, maxEditDistance)) {
					(suggestionsCombi[0].distance)++;
					suggestionParts[suggestionParts.size() - 1]=suggestionsCombi[0];
					lastCombi = true;
					continue;
				}
			}
		}

		lastCombi = false;
		if (!suggestions.empty() && ((suggestions[0].distance == 0) || (termList1[i].length() == 1))) 
		{
			suggestionParts.push_back(suggestions[0]);
		}
		else 
		{
			vector<SuggestItem> suggestionsSplit;
			if (!suggestions.empty()) suggestionsSplit.push_back(suggestions[0]);
			if(termList1[i].length()>1){
				for (int j = 1; j < termList1[i].length(); j++) {
					string part1 = termList1[i].substr(0, j);
					string part2 = termList1[i].substr(j);
					SuggestItem suggestionSplit;
					vector<SuggestItem> suggestions1 = Lookup(part1, Top, maxEditDistance);
					if (!suggestions1.empty()) {
						if (!suggestions.empty() && (suggestions[0] == suggestions1[0])) continue;
						vector<SuggestItem> suggestions2 = Lookup(part2, Top, maxEditDistance);

						if (!suggestions2.empty()) {
							if (!suggestions.empty() && (suggestions[0] == suggestions2[0])) continue;
							string split = suggestions1[0].term + " " + suggestions2[0].term;
							editDistance = EditDistance(termList1[i], Damerau);
							suggestionSplit = SuggestItem(split, 
								editDistance.DamerauLevenshteinDistance(split, maxEditDistance), min(suggestions1[0].count, suggestions2[0].count));
							if (suggestionSplit.distance >= 0) suggestionsSplit.push_back(suggestionSplit);
							if (suggestionSplit.distance == 1) break;
						}
					}
				}

				if (!suggestionsSplit.empty()) {
					sort(suggestionsSplit.begin(), suggestionsSplit.end(), [](const SuggestItem &lhs, SuggestItem &rhs)->bool {
						if (lhs.distance == rhs.distance) return lhs.count>rhs.count;
						return lhs.distance>rhs.distance;
					});
					suggestionParts.push_back(suggestionsSplit[0]);
				}
			} 
			else{
				SuggestItem si(termList1[i], 0, maxEditDistance + 1);
				suggestionParts.push_back(si);
			}
		}
	}

	SuggestItem suggestion("", numeric_limits<int>::min(), numeric_limits<long int>::max());
	stringstream ss;
	for(auto si:suggestionParts)
	{
		ss << si.term;
		suggestion.count = min(suggestion.count, si.count);
	}
	
	string tmp = ss.str();
	regex rgx_rpl("\\s+$");
	tmp = regex_replace(tmp,rgx_rpl, "");
	suggestion.term = tmp;
	editDistance = EditDistance(suggestion.term, Damerau);
	suggestion.distance = editDistance.DamerauLevenshteinDistance(input, maxDictionaryEditDistance);
	vector<SuggestItem> suggestionLine;
	suggestionLine.push_back(suggestion);
	return suggestionLine;
}

vector<SuggestItem> SymSpellCheck::lookupCompound(string input)
{
	return lookupCompound(input, this->maxDictionaryEditDistance);
}

bool SymSpellCheck::DeleteInSuggestionPrefix(string delete_str, int deleteLen, string suggestion, int suggestionLen)
{
	if (deleteLen == 0) return true;
	if (prefixLength < suggestionLen) suggestionLen = prefixLength;
	int j = 0;
	for (int i = 0; i < deleteLen; i++)
	{
		char delChar = delete_str.at(i);
		while (j < suggestionLen && delChar != suggestion.at(j)) j++;
		if (j == suggestionLen) return false;
	}
	return true;
}

vector<string> SymSpellCheck::ParseWords(string text)
{
	string origin = text;
	regex rgx("['¡¯\\p{L}-[_]]+");
	transform(text.begin(), text.end(), text.begin(), ::tolower);
	vector<string> result;
	for (sregex_iterator iter(text.begin(), text.end(), rgx); iter != sregex_iterator(); iter++)
	{
		smatch tmp = *iter;
		result.push_back(tmp.str());

	}
	return result;
}

unordered_set<string> SymSpellCheck::Edits(string word, int editDistance, unordered_set<string> deleteWords)
{
	editDistance++;
	if (word.length() > 1) {
		for (int i = 0; i < word.length(); i++) {
			string deleted_word = word.erase(i, 1);
			if ((deleteWords.insert(deleted_word)).second) {
				//recursion, if maximum edit distance not yet reached
				if(editDistance < this->maxDictionaryEditDistance) Edits(deleted_word, editDistance, deleteWords);
			}
		}
	}
	return deleteWords;
}

unordered_set<string> SymSpellCheck::EditsPrefix(string key)
{
	unordered_set<string> hashSet;
	if (key.length() <= maxDictionaryEditDistance) hashSet.insert("");
	if (key.length() > prefixLength) key = key.substr(0, prefixLength);
	hashSet.insert(key);
	return Edits(key, 0, hashSet);
}

int SymSpellCheck::GetStringHash(string s)
{
	int len = s.length();
	int lenMask = len;
	if (lenMask > 3) lenMask = 3;

	long int hash = 2166136261L;
	for (int i = 0; i < len; i++) {
		hash ^= s.at(i);
		hash *= 16777619;
	}

	hash &= this->compactMask;
	hash |= (long)lenMask;
	return (int)hash;
}
