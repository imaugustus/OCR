#pragma once
#include <iostream>
#include <string>

using namespace std;

class EditDistance
{
public:
	EditDistance();
	~EditDistance();
	string left;
	string right;
	int VladimirLevenshteinDistance(string l, string r);
	int EditDistanceDp(string left, string right);
};

