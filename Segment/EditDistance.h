#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class EditDistance
{
public:
	EditDistance();
	~EditDistance();
	string left;
	string right;
	static int EditDistanceRe(string l, string r);
	static int EditDistanceDp(string left, string right);
	static int EidtDistanceOp(string left, string right);
};

