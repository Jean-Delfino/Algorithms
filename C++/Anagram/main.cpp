#include <iostream>
#include <fstream>

#include "AnagramVerifier.h"

using namespace std;

int main(int argc, char** argv) {
    AnagramVerifier anagramVerifier;
	string nameFile;
	getline(cin, nameFile);

	ifstream anagramFile("AnagramFiles/" + nameFile); 
	
    if (anagramFile.is_open()) {
        string line;

        while (getline(anagramFile, line)) {
            anagramVerifier.InsertWordInAnagram(OrderLetterByLetter(line), line);
        }

        anagramFile.close();
    } else {
        cout << "Couldn't open file" << endl;
    }

    anagramVerifier.PrintAllBiggestAnagrams();
	return 0;
}
