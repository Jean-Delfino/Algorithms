#include <iostream>
#include "DuplicateEncoder.h"
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	string tryTest;
	DuplicateEncoder *encoder = new DuplicateEncoder();
	getline(cin, tryTest);

	cout << encoder->EncodeString(tryTest) << endl;
	return 0;
}
