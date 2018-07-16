// Segment.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <TM.h>
# include <string>

int main()
{
	TM tm;
	string corpus_path = "g:\\Netease\\MySolution\\Segment\\Segment\\frequency_dictionary_en_82_765.txt";
	tm.dict_iter = tm.dictionary.find("the");
	if (tm.dict_iter != tm.dictionary.end()) {
		cout << "the is in the dictionary" << endl;

	}
	else {
		cout << "the is not int the map" << endl;
	}
	cout << (tm.dictionary)["the"] << endl;
	bool res = tm.load_dict(corpus_path);
	string test = "bigdogjumpoverthefatfox";
	auto[str, prob] = tm.segment(test, 20);
    return 0;
}

