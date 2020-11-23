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
				words[word_size-1].push_back(word) ;
				
				//sort(vector_word.begin(), vector_word.end());
			}
			us.insert(line);
			
		}
		
	} else {
		cout << "File not found!\n";
	}
}

bool Dictionary::contains(const string& word) const {
	return us.count(word)>0;
}

vector<string> Dictionary::get_suggestions(const string& word) const {
	vector<string> suggestions;
	add_trigram_suggestions(suggestions, word);
	cout<<suggestions.size()<<std::endl;

	rank_suggestions(suggestions, word);
		cout<<suggestions.size()<<std::endl;

	trim_suggestions(suggestions);
		cout<<suggestions.size()<<std::endl;

	return suggestions;
}

vector<string> Dictionary::getTrigrams(const string& word) const
{
	vector<string> trigrams;
	if (word.size()>3)
	{
	for (size_t i = 0; i < word.size()-2; i++)
	{
		trigrams.push_back(word.substr(i, i+3));
	}
	sort(trigrams.begin(), trigrams.end()); 
	}

	return trigrams;
}

void Dictionary::add_trigram_suggestions(  vector<string>& suggestions ,const string& word) const
{
	for(size_t i = word.size() - 2; i <= word.size(); i++) {
		vector<Word> word_vector= words[i];
		for(size_t k = 0; k < word_vector.size(); k++)
		{
			if (word_vector.at(k).get_matches(getTrigrams(word))>=word.size()-2)
			{
			suggestions.push_back(word_vector.at(k).get_word());
			}
			
		}
			cout<<"word_vector size: "<<word_vector.size()<<std::endl;
	}
}
void Dictionary::rank_suggestions( vector<string>& suggestions ,const string& word) const
{
vector<std::pair<int, string>> pairs;
	
	for (string& suggestion : suggestions)
	{
		int dist = count_dist(word, suggestion);
		pairs.push_back(std::make_pair(dist, suggestion));
	}

	std::sort(pairs.begin(), pairs.end());
	vector<string> newSuggestions;
	for (std::pair<int, string> pair : pairs)
		newSuggestions.push_back(pair.second);
     	suggestions = newSuggestions;
}
void Dictionary::trim_suggestions(  vector<string>& suggestions) const
{
	suggestions.resize(5);
}

// första är vår ord, andra är destinationen eller hur?!
int Dictionary::count_dist(const string& word,const string& suggestion) const {

	int matrix[26][26]; 
	for (size_t i = 0; i <= word.size(); ++i)
		matrix[i][0] = i;
	for (size_t i = 0; i <= suggestion.size(); ++i)
		matrix[0][i] = i;

	for (size_t i = 1; i <= word.size(); ++i)
	{
		for (size_t j = 1; j <= suggestion.size(); ++j)
		{
			int left= matrix[i - 1][j];
			int up= matrix[i][j - 1];
			int corner=matrix[i - 1][j - 1];

			int min = (left < up ? left : up) > corner ? corner : (left < up ? left : up);
			
			if (word.at(i - 1) == suggestion.at(j - 1))
			{
			matrix[i][j]= min;
			}else
			{
			matrix[i][j]= min+1;
			}
	
			
		}
	}
	return matrix[word.size()][suggestion.size()];
}