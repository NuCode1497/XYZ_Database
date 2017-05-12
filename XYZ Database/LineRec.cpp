#include "LineRec.h"

lineRec::lineRec()
{
	line = "";
	pFilename = NULL;
	lineNumber = 0;
}

lineRec::lineRec(string line, string *pFilename, int lineNumber)
{
	(*this).line = line;
	(*this).pFilename = pFilename;
	(*this).lineNumber = lineNumber;
}

ostream& operator << (ostream& out, const lineRec& rhs)
{
	out<<rhs.line;
	return out;
}