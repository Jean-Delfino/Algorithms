#include "Arithmetics.h"
#include "Travel.h"
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#define SEPARATOR ' '

int *divideString(string numbers, int numberAmount, char separator) {
  int i, cnt = 0;
  int *res = new int[numberAmount];
  string value;
  for (i = 0; i < numbers.length(); i++) {
    if (numbers[i] == separator) {
      res[cnt] = stoi(value);
      cnt++;
      value = "";
      continue;
    }
    value += numbers[i];
  }
  res[cnt] = stoi(value);
  return res;
}

int main(int argc, char **argv) {
  string nameFile;
  ifstream travelFile;
  int lin, col;
  int endLin, endCol;

  do {
    cout << "File name : ";
    getline(cin, nameFile);
    travelFile.open("Travel/" + nameFile);
  } while (!travelFile.good());

  travelFile >> lin >> col;

  int **matriz = new int *[lin];
  string numbers;

  getline(travelFile, numbers); //\n garbage
  for (int i = 0; i < lin; i++) {
    getline(travelFile, numbers);
    matriz[i] = divideString(numbers, col, SEPARATOR);
  }

  for (int i = 0; i < lin; i++) {
    for (int j = 0; j < col; j++) {
      cout << matriz[i][j] << " ";
    }
    cout << endl;
  }

  travelFile >> endLin >> endCol;
  cout << endLin << " " << endCol << endl;

  Travel minePath;
  cout << minePath.TravelPointGraph(matriz, col, lin, Point(0, 0),
                                    Point(endCol, endLin), EuclidianDistance)
       << endl;

  for (int i = 0; i < lin; i++) {
    delete[] matriz[i];
  }

  delete[] matriz;
  return 0;
}
