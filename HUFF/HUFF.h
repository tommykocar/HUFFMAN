#pragma once


#include <fstream>
#include <string>
#include <iostream>
#include <ctime>


using namespace std;


class HUFF
{
public:

	HUFF();
	
	
	void DisplayHelp();
	void EncodeFile(string inputFile, string OutputFile);
	void DecodeFile(string inputFile, string OutputFile);
	void MakeTreeBuilder(string inputFile, string OutputFile);
	void EncodeFileWithTree(string inputFile, string TreeFile, string outputFile);


private:

	struct node
	{

		unsigned char letter = NULL;
		unsigned int  weight = 0;
		node* LChildPointer = NULL;
		node* RChildPointer = NULL;
	};

	node* tree = NULL;

	
	void PathFinder(node* treeNode, string paths, string arrayOfPaths[256]);
};