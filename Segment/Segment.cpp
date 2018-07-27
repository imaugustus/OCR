// Segment.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <TM.h>
#include <string>
#include <VTM.h>
#include <EditDistance.h>
#include <vector>
#include <SuggestItem.h>
#include <ChunkArray.h>
#include <SuggestionStage.h>
#include <SymSpellCheck.h>


using namespace std;

int main()
{
	int maxEditDistance = 3;
	SymSpellCheck::Verbosity verbosity = SymSpellCheck::Closest;
	SymSpellCheck symSpell(-1, maxEditDistance, -1,10);
	string path = "d:\\OCR\\Segment\\new.txt";
	if (!symSpell.LoadDictionary(path)) {
		cout << "File not found" << endl;
	}
	vector<int> test = { 1674601435 ,50332431 ,78644415,10485899 ,1943041007 };
	for (auto i : test) {
		if (symSpell.deletes.count(i)) {
			cout << symSpell.deletes[i][0] << endl;
		}
		else{
			cout << "Not found" << endl;
		}
	}
	auto result = symSpell.Lookup("warmup", SymSpellCheck::All);
	//cout << "Type a work and hit enter key to get spelling suggestions:" << endl;
	string input="footboll";
	vector<SuggestItem> suggestions = symSpell.Lookup(input, verbosity);
    return 0;
}
