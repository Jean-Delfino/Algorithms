#include <iostream>
#include <map>

#include "HuffmanTree.h"

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char **argv) {
	string tryTest;

	cout << "Insert a phrase : ";
	getline(cin, tryTest);

	map<string, int> values = PerCharacterDescription(tryTest);

	HuffmanTree *tree = new HuffmanTree(values);

	map<string, string> codes = tree->GetTreeCodes();
	string compressedString = tree->CompressString(tryTest, codes);

	cout << "Your compressed phrase : " << compressedString << endl;
	cout << "Your decompressed phrase : " << tree->DecompressString(compressedString) << endl;
	
	return 0;
}