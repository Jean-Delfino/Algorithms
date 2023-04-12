#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>

using namespace std;

#define BYTE_SIZE 8
#define END_STRING "\0"

struct StringPercentage
{
	string value;
	int percentage;

	bool operator<(const StringPercentage &other) const
	{
		if (percentage < other.percentage)
		{
			return true;
		}

		return false;
	}
};

struct HuffmanTreeNode
{
	StringPercentage value;
	HuffmanTreeNode *left;
	HuffmanTreeNode *right;

	bool operator<(const HuffmanTreeNode &other) const
	{
		return value < other.value;
	}

	HuffmanTreeNode(string phrase, int percentage)
	{
		left = nullptr;
		right = nullptr;
		value.value = phrase;
		value.percentage = percentage;
	}

	HuffmanTreeNode(string phrase, int percentage, HuffmanTreeNode *newLeft, HuffmanTreeNode *newRight)
	{
		left = newLeft;
		right = newRight;
		value.value = phrase;
		value.percentage = percentage;
	}
};

class HuffmanTree
{
public:
	HuffmanTree(map<string, int> file)
	{
		priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, decltype(&HuffmanTree::CompareNodes)> values = MakeInitialNodes(file);
		root = BuildRoot(values);
	}

	map<string, string> GetTreeCodes();
	string CompressString(string toCompress, map<string, string> treeCodes);
	string DecompressString(string toDecompress);

	void PrintTree();

private:
	HuffmanTreeNode *root;

	static bool
	CompareNodes(const HuffmanTreeNode *left, const HuffmanTreeNode *right)
	{
		return *right < *left;
	}

	priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, decltype(&HuffmanTree::CompareNodes)> MakeInitialNodes(map<string, int> file);
	HuffmanTreeNode *BuildRoot(priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, decltype(&HuffmanTree::CompareNodes)> nodes);
};

map<string, int> PerCharacterDescription(string entry);

#endif
