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
#include <SymSpellCheckDemo.h>

using namespace std;

int main()
{
	//VTM vtm;
	//string corpus_path_v = "d:\\OCR\\Segment\\new.txt";
	//bool res_v = vtm.load_dict(corpus_path_v);
	////string test_v = "iitwasabrightcolddayinaprilandtheclockswerestrikingthirteen";
	//string test_v = "hel1o";
	//string result = "";
	//double probability = 0;
	//vtm.segment(test_v, &result, probability);


	//int a = 0;
	//ChunkArray<int> test(2);
	//cout << "chunkarray created" << endl;
	//int size = test.Count;
	//test.Add(2);
	//test.Add(3);
	//test.Add(4);
	//int res = test.getValues(2);
	//test.setValues(1, 9);

	//SuggestionStage test(10);
	//test.Add(1, "hello");
	//EditDistance test("hello", Damerau);
	//int res = test.compare("h1llw",8);

	cout << "Hello?" << endl;
	SymSpellCheckDemo demo(3);
	cout << "Algo object created" << endl;
	string inputTerm;
	cout << "This is test demo for spell checking" << endl;
	while (true) {
		cout << "Enter input" << endl;
		getline(cin, inputTerm);
		vector<SuggestItem> suggestions = demo.lookup(inputTerm);
		SuggestItem compund = demo.lookupCompound(inputTerm);
		for (SuggestItem suggestion : suggestions) {
			cout << "Lookup suggestion: " << suggestion.term << " " << suggestion.distance << " " << suggestion.count << endl;
		}
		cout << "LookupCompound: " << compund.term << endl;
	}
    return 0;
}

