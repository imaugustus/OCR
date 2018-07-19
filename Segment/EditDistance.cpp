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

int EditDistance::EditDistanceRe(string left, string right)
{
	int l_length = left.length();
	int r_length = right.length();

	if (left.length() == 0) return r_length;
	else if (right.length() == 0) return l_length;
	else if (left.substr(l_length - 1) == right.substr(r_length - 1)) {
		//cout << "Last char maches" << endl;
		return EditDistanceRe(left.substr(0, l_length - 1), right.substr(0, r_length - 1));
	}
	else{
		int a = EditDistanceRe(left.substr(0, l_length - 1), right) + 1;
		int b = EditDistanceRe(left, right.substr(0, r_length - 1)) + 1;
		int c = EditDistanceRe(left.substr(0, l_length - 1), right.substr(0, r_length - 1)) +1;
		return min(a, min(b, c));
	}

}

int EditDistance::EditDistanceDp(string left, string right)
{
	int l_length = left.length();;
	int r_length = right.length();
	vector< vector<int>> cache(l_length+1, vector<int>(r_length+1));
	int i, j;
	for (i = 0; i <= l_length; i++) {
		cache[i][0] = i;
	}
	for (j = 0; j <= r_length; j++) {
		cache[0][j] = j;
	}

	for (i = 1; i <= l_length; i++) {
		for (j = 1; j <= r_length; j++) {
			if (left.at(i-1) == right.at(j-1)) {
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
	return cache[l_length][r_length];
}
