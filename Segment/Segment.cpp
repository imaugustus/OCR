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
	SuggestItem a("hello", 23, 233333333);
	SuggestItem b("hell1", 24, 233333333);
	cout << a.count << endl;
	cout << (a == b) << endl;
	cout << a.CompareTo(b) << endl;
    return 0;
}

