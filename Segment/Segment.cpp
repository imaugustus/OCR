// Segment.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <TM.h>
#include <string>
#include <VTM.h>
#include <EditDistance.h>

using namespace std;

int main()
{
	//VTM vtm;
	//string corpus_path_v = "d:\\OCR\\Segment\\new.txt";
	//bool res_v = vtm.load_dict(corpus_path_v);
	//string test_v = "iitwasabrightcolddayinaprilandtheclockswerestrikingthirteen";
	//string result = "";
	//double probability = 0;
	//vtm.segment(test_v, &result, probability);
	string t_left = "hpllo";
	string t_right = "hellw";
	EditDistance ed;
	int res = ed.VladimirLevenshteinDistance(t_left, t_right);
	int res_2 = ed.EditDistanceDp(t_left, t_right);
    return 0;
}

