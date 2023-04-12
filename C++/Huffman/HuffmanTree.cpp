#include "HuffmanTree.h"

using namespace std;

// This codes char per char, which isn't the best implementation, but is the
// most basic one.
map<string, int> PerCharacterDescription(string entry)
{
	map<string, int> characters;
	map<string, int>::iterator it;
	string aux;
	int i;

	for (i = 0; i < entry.length(); i++)
	{
		aux = entry[i];
		it = characters.find(aux);

		if (it == characters.end())
		{
			characters[aux] = 0;
		}
		characters[aux]++;
	}

	characters[END_STRING] = 1;

	return characters;
}

void HuffmanTree::PrintTree()
{
	HuffmanTreeNode *runner = root;
	string left;
	string right;

	if (runner == nullptr)
		return;

	stack<HuffmanTreeNode *> nodes;

	cout << "Tree = " << endl;
	nodes.push(runner);

	while (nodes.size() > 0)
	{
		runner = nodes.top();
		nodes.pop();

		if (runner->left != nullptr)
		{
			nodes.push(runner->left);
			left = runner->left->value.value;
		}
		else
		{
			left = "";
		}
		if (runner->right != nullptr)
		{
			nodes.push(runner->right);
			right = runner->right->value.value;
		}
		else
		{
			right = "";
		}

		cout << "Node = " << runner->value.value << " "
			 << runner->value.percentage << "LEFT = " << left << "RIGHT "
			 << right << endl;
	}
}

priority_queue<HuffmanTreeNode *,vector<HuffmanTreeNode *>,decltype(&HuffmanTree::CompareNodes)> HuffmanTree::MakeInitialNodes(map<string, int> file)
{
	priority_queue<HuffmanTreeNode *,vector<HuffmanTreeNode *>, decltype(&HuffmanTree::CompareNodes)>values(&HuffmanTree::CompareNodes);

	for (const auto &kv : file)
	{
		HuffmanTreeNode *node = new HuffmanTreeNode(kv.first, kv.second);
		values.push(node);
	}

	return values;
}

HuffmanTreeNode *HuffmanTree::BuildRoot(priority_queue<HuffmanTreeNode *,vector<HuffmanTreeNode *>,decltype(&HuffmanTree::CompareNodes)>nodes)
{
	while (nodes.size() > 1)
	{
		HuffmanTreeNode *left = nodes.top();
		nodes.pop();

		HuffmanTreeNode *right = nodes.top();
		nodes.pop();

		HuffmanTreeNode *combined = new HuffmanTreeNode(
			"-", left->value.percentage + right->value.percentage, left, right);

		nodes.push(combined);
	}

	HuffmanTreeNode *root = nodes.top();
	nodes.pop();

	return root;
}

void TravelTree(HuffmanTreeNode *node,map<string, string> &codeMap,string actualString)
{
	if (node == nullptr)
		return;

	if (node->left == nullptr && node->right == nullptr)
	{
		codeMap[node->value.value] = actualString;
	}

	TravelTree(node->left, codeMap, actualString + "0");
	TravelTree(node->right, codeMap, actualString + "1");
}

map<string, string> HuffmanTree::GetTreeCodes()
{
	map<string, string> codeMap;

	TravelTree(root, codeMap, "");

	return codeMap;
}

string GetLargestKey(map<string, string> treeCodes)
{
	string largestKey = "";
	for (auto const &pair : treeCodes)
	{
		if (pair.first.length() > largestKey.length())
		{
			largestKey = pair.first;
		}
	}
	return largestKey;
}

void AddEncodedString(char &bytecode,int &actualSize,string toAdd,string &res)
{
	int i;

	for (i = 0; i < toAdd.length(); i++)
	{
		if (actualSize == BYTE_SIZE)
		{
			res += bytecode;

			bytecode = 0;
			actualSize = 0;
		}
		bytecode = bytecode << 1;

		if (toAdd[i] == '1')
		{
			bytecode = bytecode + 1;
		}

		actualSize = actualSize + 1;
	}
}

void PrintAllTreeCodes(map<string, string> treeCodes)
{
	for (auto it = treeCodes.begin(); it != treeCodes.end(); it++)
	{
		cout << it->first << " : " << it->second << endl;
	}
}

string HuffmanTree::CompressString(string toCompress, map<string, string> treeCodes)
{
	int i, j, largestSubString;
	string res;
	char bytecode = 0;
	int actualSize = 0;

	largestSubString = GetLargestKey(treeCodes).length();

	for (i = 0; i < toCompress.length(); i++)
	{
		for (j = i + largestSubString; j > i; j--)
		{
			if (treeCodes.find(toCompress.substr(i, j - i)) !=
				treeCodes.end())
			{
				break;
			}
		}
		if (j > i)
		{
			AddEncodedString(
				bytecode,
				actualSize,
				treeCodes[toCompress.substr(i, j - i)],
				res);
			i = j - 1;
		}
		else
		{
			cout << "Impossible codification with the actual codes" << endl;
			break;
		}
	}

	AddEncodedString(bytecode, actualSize, treeCodes[END_STRING], res);
	bytecode = bytecode << (BYTE_SIZE - actualSize);

	res += bytecode;

	return res;
}

string HuffmanTree::DecompressString(string toDecompress)
{
	HuffmanTreeNode *runner;
	char aux;
	bool leftMost;
	string res;
	int bitsLeftsInChar = BYTE_SIZE;
	int i;

	aux = toDecompress[0];
	for (i = 0; i < toDecompress.length();)
	{
		runner = root;

		while (runner != nullptr && runner->left != nullptr &&
			   runner->right != nullptr)
		{
			leftMost = (aux >> 7) & 1;
			aux = aux << 1;
			if (leftMost == true)
			{
				runner = runner->right;
			}
			else
			{
				runner = runner->left;
			}

			bitsLeftsInChar--;

			if (bitsLeftsInChar == 0)
			{
				bitsLeftsInChar = BYTE_SIZE;
				i++;
				aux = toDecompress[i];
			}
		}

		if (runner == nullptr)
		{
			cout << "Impossible decodification with the actual codes" << endl;
			break;
		}

		res += runner->value.value;

		if (runner->value.value == END_STRING)
			break;
	}

	return res;
}