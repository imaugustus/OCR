#pragma once
#include <string>
#include <map>
#include <tuple>

using namespace std;
class TM
{
public:
	TM();
	~TM();
	map<string, float> dictionary;
	map<string, float> ::iterator dict_iter;
	int max_dict_word_length;
	float total_count;
	bool load_dict(string corpus_path);
	int min(int x, int y);
	tuple<string, float> segment(string input, int max_dict_word_length);
};

