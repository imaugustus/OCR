#pragma once
#include <string>
#include <SymSpellCheck.h>
#include <exception>

using namespace std;

class SymSpellCheckDemo
{
public:
	SymSpellCheckDemo();
	SymSpellCheckDemo(int maxEditDistanceLookup);
	vector<SuggestItem> lookup(string input);
	SuggestItem lookupCompound(string input);
	~SymSpellCheckDemo();
	string path = "d:\\OCR\\Segment\\new.txt";
	SymSpellCheck::Verbosity suggestionVerbosity = SymSpellCheck::All;
	int maxEditDistanceLookup;
	SymSpellCheck symSpell;


};

