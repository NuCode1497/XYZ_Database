#pragma once
#include <string>
using namespace std;

class lineRec
{
public:
	lineRec();
	lineRec(string line, string *pFilename, int lineNumber);
	string getLine(){return line;}
	string getFilename(){return *pFilename;}
	int getLineNum(){return lineNumber;}

	friend ostream& operator << (ostream& out, const lineRec& rhs);
private:
	string line;
	string *pFilename;
	int lineNumber;
};