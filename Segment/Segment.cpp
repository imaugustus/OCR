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
	SymSpellCheck::Verbosity verbosity = SymSpellCheck::All;
	SymSpellCheck symSpell(-1, maxEditDistance, -1,10);
	string path = "d:\\OCR\\Segment\\new.txt";
	if (!symSpell.LoadDictionary(path)) {
		cout << "File not found" << endl;
	}
	string input="footboll";
	//vector<string> res = symSpell.deletes[-368299363];
	vector<SuggestItem> suggestions = symSpell.Lookup(input, verbosity);
	cout << suggestions[0].term << endl;
	//system("pause");
	
	
	//int a = 0xffffffff;
	//SymSpellCheck demo(-1, 3, -1, 10);
	//vector<string> test = { "football", "education", "hah", "keyboard", "fkuc" };
	//vector<int> hash;
	//for (int i = 0; i<test.size(); i++) {
	//	int testhash = demo.GetStringHash(test[i]);
	//	hash.push_back(testhash);
	//}
    return 0;
}
//to 12136980858
//a 9081174698
//in 8469404971
//for 5933321709
//is 4705743816
//on 3750423199
//that 3400031103
//by 3350048871