#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <vector>
#include <unordered_set>
#include "word.h"

class Dictionary {
public:
	Dictionary();
	bool contains(const std::string& word) const;
	std::vector<std::string> get_suggestions(const std::string& word) const;
private:
	std::unordered_set<std::string> us;
	std::vector<Word> words[25];
	std::vector<string> getTrigrams(const string& word) ;
	void add_trigram_suggestions( vector<string>& suggestions, const std::string& word) const;
	void rank_suggestions( vector<string>& suggestions, const std::string& word) const;
	void trim_suggestions( vector<string>& suggestions) const;
	int count_dist(string& suggestion, const string& word) const;
};

#endif
