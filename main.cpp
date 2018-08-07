// Segment.cpp: 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>
#include "TM.h"
#include <string>
#include "VTM.h"
#include "EditDistance.h"
#include <vector>
#include "SuggestItem.h"
#include "ChunkArray.h"
#include "SuggestionStage.h"
#include "SymSpellCheck.h"


using namespace std;

int main()
{
	int maxEditDistance = 2;
	SymSpellCheck::Verbosity verbosity = SymSpellCheck::All;
	SymSpellCheck symSpell(-1, maxEditDistance, -1,10);
	string path = "d:\\OCR\\Segment\\new.txt";
	if (!symSpell.LoadDictionary(path)) {
		cout << "File not found" << endl;
	}
	string input="footboll";
	vector<SuggestItem> res = symSpell.Lookup(input, verbosity);
	for(auto temp:res)
	{
		cout << temp.term<<endl;	
	}
    return 0;
}
