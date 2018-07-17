// Segment.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <TM.h>
#include <string>
#include <VTM.h>

int main()
{
	//TM tm;
	//string corpus_path = "d:\\OCR\\Segment\\new.txtt";
	//bool res = tm.load_dict(corpus_path);
	//string test = "bigdogjumpoverthefatfox";
	//auto[str, prob] = tm.segment(test);
	VTM vtm;
	string corpus_path_v = "d:\\OCR\\Segment\\new.txt";
	bool res_v = vtm.load_dict(corpus_path_v);
	//string test_v = "bigdogjumpoverthefatfox";
	string test_v = "iitwasabrightcolddayinaprilandtheclockswerestrikingthirteen";
	auto[str_v, prob_v] = vtm.segment(test_v);
    return 0;
}

