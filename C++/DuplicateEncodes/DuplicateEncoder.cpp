#include "DuplicateEncoder.h"

string DuplicateEncoder::EncodeString(string toEncode) {
  int i;
  string res;
  map<char, bool> repetitionMap;

  for (i = 0; i < toEncode.length(); i++) {
    repetitionMap[toEncode[i]] = (repetitionMap.find(toEncode[i]) != repetitionMap.end());
  }

  for (i = 0; i < toEncode.length(); i++) {
    if (repetitionMap[toEncode[i]] == true) {
      res += repetition;
    } else {
      res += noRepetition;
    }
  }

  return res;
}
