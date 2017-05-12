#pragma once
#include "BST.h"
#include "LineRec.h"
#include "wordRec.h"
#include <string>
#include <list>
#include <iostream>
using namespace std;

class DataBase
{
public:
	DataBase();
	string transducer(string command);
private:
	bool addFile(string filename);
	bool findWord(const string &word);
	bool query(const string &expression);
	vector<lineRec *>findSet(const string &expression);
	void removeDuplicates(vector<wordRec *> &words);
	vector<lineRec *> wordsetToLineset(vector<wordRec *> &words);
	void clearDB();
	void saveDB();
	bool isValidChar(char c);

	list<string> filenames;
	list<lineRec> lineRecs;
	xBST<wordRec, string> wordRecs;
};


