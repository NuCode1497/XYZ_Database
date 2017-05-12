#include "wordRec.h"

wordRec::wordRec()
{
	word = "";
	defcon=1;
	pLineRec = new lineRec();
}

wordRec::~wordRec()
{
	if(defcon) delete pLineRec;
}

wordRec::wordRec(string word, lineRec *pLineRec)
{
	defcon=0;
	(*this).word = word;
	(*this).pLineRec = pLineRec;
}

ostream& operator << (ostream& out, const wordRec& rhs)
{
	out<<rhs.word;
	return out;
}
