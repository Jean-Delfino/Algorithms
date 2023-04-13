#include "AnagramVerifier.h"

string OrderLetterByLetter(string str) {
    sort(str.begin(), str.end()); 
    return str;
}

void AnagramVerifier::InsertWordInAnagram(string key, string word){
	if (anagramMap.find(key) == anagramMap.end()) {
		anagramMap[key] = set<string>();
	}
	
	anagramMap[key].insert(word);

	if(anagramMap[key].size() > biggestSequence) biggestSequence = anagramMap[key].size();
}

void AnagramVerifier::PrintAllBiggestAnagrams(){
	int i;

	cout << biggestSequence << endl;
	for (auto it = anagramMap.begin(); it != anagramMap.end(); it++) {
		if(it->second.size() == biggestSequence){
			for (string elemento : it->second) {
				std::cout << elemento << ", ";
			}
			cout << endl;
		}
    }
}
