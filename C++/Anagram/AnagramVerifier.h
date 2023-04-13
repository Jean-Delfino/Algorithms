#ifndef ANAGRAMVERIFIER_H
#define ANAGRAMVERIFIER_H

#include <iostream>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

class AnagramVerifier
{
	public:
		void InsertWordInAnagram(string key, string word);
		void PrintAllBiggestAnagrams();

	private:
		map<string, set<string>> anagramMap;
		int biggestSequence = 0;
};

string OrderLetterByLetter(string str);

#endif
