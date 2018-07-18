#include "stdafx.h"
#include "EditDistance.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

EditDistance::EditDistance()
{
}


EditDistance::~EditDistance()
{
}

int EditDistance::VladimirLevenshteinDistance(string left, string right)
{
	int l_length = left.length();
	int r_length = right.length();
	if (left.length() == 0) return r_length;
	else if (right.length() == 0) return l_length;
	else if (left.substr(l_length - 1) == right.substr(r_length - 1)) {
		cout << "Last char maches" << endl;
		return VladimirLevenshteinDistance(left.substr(0, l_length - 1), right.substr(0, r_length - 1));
	}
	else{
		int a = VladimirLevenshteinDistance(left.substr(0, l_length - 1), right) + 1;
		int b = VladimirLevenshteinDistance(left, right.substr(0, r_length - 1)) + 1;
		int c = VladimirLevenshteinDistance(left.substr(0, l_length - 1), right.substr(0, r_length - 1)) +1;
		return min(a, min(b, c));
	}

}

int EditDistance::EditDistanceDp(string left, string right)
{
	int l_length = left.length();
	int r_length = right.length();
	if (left.length() == 0) return r_length;
	else if (right.length() == 0) return l_length;
	vector<vector<int>> cache(l_length, vector<int>(r_length, 0));
	if (left.substr(0,1) == right.substr(0,1))
	{
		cache[0][0] = 0;
	}
	else {
		cache[0][0] = 1;
	}
	for (int i = 1; i < l_length; i++) {
		for (int j = 1; j < r_length; j++) {
			if (left.substr(i, 0) == right.substr(j, 0)) {
				cache[i][j] = cache[i - 1][j - 1];
			}
			else {
				int a = cache[i - 1][j] + 1;
				int b = cache[i][j - 1] + 1;
				int c = cache[i - 1][j - 1] + 1;
				cache[i][j] = min(a, min(b, c));
			}
		}
	}
	return cache[l_length-1][r_length-1];
}
