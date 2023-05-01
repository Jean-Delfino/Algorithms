#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
	string nameFile;
	getline(cin, nameFile);
	ifstream travelFile("Travel/" + nameFile); 

  	int N;
  	travelFile >> N;

  	int** graph = new int*[N];
  	for (int i = 0; i < N; i++) {
    	graph[i] = new int[N];
  	}

  	for (int i = 0; i < N; i++) {
    	for (int j = 0; j < N; j++) {
      		travelFile >> graph[i][j];
    	}
  	}	
  	
	return 0;
}
