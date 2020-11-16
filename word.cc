#include <string>
#include <vector>
#include "word.h"

using std::vector;
using std::string;

Word::Word(const string& w, const vector<string>& t): word{w}, trigrams{t}
{

}

string Word::get_word() const
{
	return word;
}

unsigned int Word::get_matches(const vector<string>& t) const
{
	int sum = 0;
	for (const string& element : trigrams)
	{
		for (const string& e : t)
		{
			if (element.substr(0, 1) < e.substr(0, 1))
			{
				break;
			} 
			else
			{
				if (element == e)
				{
					++sum;
				}
			}
		}
		
	}
	
	return sum;
}
