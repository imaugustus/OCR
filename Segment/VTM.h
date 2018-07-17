#pragma once
#include <string>
#include <map>
#include <tuple>

using namespace std;
class VTM
{
public:
	VTM();
	~VTM();
	map<string, long long int> dictionary;
	map<string, long long int> ::iterator dict_iter;
	int max_dict_word_length;
	long long int total_count;
	bool load_dict(string corpus_path);
	tuple<string, double> segment(string input);
	tuple<string, double> segment(string input, int max_dict_word_length);
};

