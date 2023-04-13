#ifndef DUPLICATEENCODER_H
#define DUPLICATEENCODER_H

#define DEFAULT_NO_REPETITION "("
#define DEFAULT_REPETITION ")"

#include <iostream>
#include <map>
using namespace std;

class DuplicateEncoder
{
	public:
		DuplicateEncoder(){
			noRepetition = DEFAULT_NO_REPETITION;
			repetition = DEFAULT_REPETITION;
		}
		
		DuplicateEncoder(string newNoRepetition, string newRepetition){
			noRepetition = newNoRepetition;
			repetition = newRepetition;
		}
		
		string EncodeString(string toEncode);
	private:
		string noRepetition;
		string repetition;
};

#endif
