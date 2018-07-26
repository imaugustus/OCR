#include "stdafx.h"
#include "SymSpellCheckDemo.h"


SymSpellCheckDemo::SymSpellCheckDemo()
{
}

SymSpellCheckDemo::SymSpellCheckDemo(int maxEditDistanceLookup)
{
	symSpell = SymSpellCheck(-1, maxEditDistanceLookup, -1, 10);
	this->maxEditDistanceLookup = maxEditDistanceLookup;
	if (!symSpell.LoadDictionary(path)) cout << "File not exists"<<endl;
}

vector<SuggestItem> SymSpellCheckDemo::lookup(string input)
{
	return symSpell.Lookup(input, suggestionVerbosity, maxEditDistanceLookup);
}

SuggestItem SymSpellCheckDemo::lookupCompound(string input)
{
	return symSpell.lookupCompound(input, maxEditDistanceLookup)[0];
}


SymSpellCheckDemo::~SymSpellCheckDemo()
{
}
