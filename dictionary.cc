#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "word.h"
#include "dictionary.h"

using std::string;
using std::vector;
using std::unordered_set;
using std::ifstream;
using std::cout;

Dictionary::Dictionary() {
	ifstream file;
	string line;
	file.open("dict-words.txt");
	if (file.is_open()) {
		while (getline(file, line))
		{
			int word_size = line.size();
			if (word_size <= 25) 
			{
				vector<string> trigrams= getTrigrams(line);
				Word word {line, trigrams};
				vector<Word> vector_word = words[word_size - 1];
				vector_word.push_back(word);
				sort(vector_word.begin(), vector_word.end());
			}
			us.insert(line);
		}
		
	} else {
		cout << "File not found!\n";
	}
	
		cout<< us.size()<<std::endl;

	
}

bool Dictionary::contains(const string& word) const {
	for (const auto& element: us)
	{
		if (element== word)
		{
			return true;
		}
		
	}
	
	return false;
}

vector<string> Dictionary::get_suggestions(const string& word) const {
	vector<string> suggestions;
	add_trigram_suggestions(suggestions, word);
	rank_suggestions(suggestions, word);
	trim_suggestions(suggestions);
	return suggestions;
}

vector<string> Dictionary::getTrigrams(const string& word)
{
	vector<string> trigrams;
	for (size_t i = 0; i < word.size()-2; i++)
	{
		trigrams.push_back(word.substr(i, i+3));
	}
	sort(trigrams.begin(), trigrams.end()); 

	return trigrams;
}

void Dictionary::add_trigram_suggestions(  vector<string>& suggestions ,const string& word) const
{
	for(int i = word.size() - 2; i <= word.size(); i++) {
		vector<Word> word_vector= words[i];
		for(int k = 0; k < word_vector.size(); k++)
		{
			suggestions.push_back(word_vector.at(k).get_word());
		}
	}
}
void Dictionary::rank_suggestions( vector<string>& suggestions ,const string& word) const
{
vector<std::pair<unsigned int, string>> pairs;
	
	for (string& suggestion : suggestions)
	{
		int dist = count_dist(suggestion, word);
		pairs.push_back(std::make_pair(dist, suggestion));
	}

	std::sort(pairs.begin(), pairs.end());
	vector<string> newSuggestions;
	for (std::pair<unsigned int, string> pair : pairs)
		newSuggestions.push_back(pair.second);
     	suggestions = newSuggestions;
}
void Dictionary::trim_suggestions(  vector<string>& suggestions) const
{
	vector<string> suggestions_to_return;
	for (size_t i = 0; i < 5; i++)
	{
		suggestions_to_return.push_back(suggestions[i]);
	}
	suggestions= suggestions_to_return;
}

int Dictionary::count_dist(string& suggestion, const string& word) const {
	int count{0};

	if (suggestion.size() != word.size())
	{
		count++;
	}
	if (suggestion.size()> word.size())
	{
		for (size_t i = 0; i < word.size(); i++)
		{
			if (suggestion[i]!=word[i])
			{
				count++;
			}
			
		}
		
	}
		if (word.size()> suggestion.size())
	{
		for (size_t i = 0; i < suggestion.size(); i++)
		{
			if (suggestion[i]!=word[i])
			{
				count++;
			}
			
		}
		
	}

	return 0;
}