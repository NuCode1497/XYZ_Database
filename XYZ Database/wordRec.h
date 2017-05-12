#pragma once
#include "LineRec.h"
#include <string>
using namespace std;

class wordRec
{
public:
	wordRec();
	wordRec(string word, lineRec *pLineRec);
	~wordRec();

	//compare two wordRecs
	bool operator >(const wordRec & rhs) const {return word > rhs.word;}
	bool operator ==(const wordRec & rhs) const {return word == rhs.word;}
	bool operator >=(const wordRec & rhs) const {return ((*this) > rhs ) || ((*this) == rhs );}
	bool operator <(const wordRec & rhs)  const {return !((*this) >= rhs );}
	bool operator <=(const wordRec & rhs) const {return !((*this) > rhs );}
	bool operator !=(const wordRec & rhs) const {return !((*this) == rhs );}
	
	//compare wordRec to a string
	bool operator >(const string & rhs) const{return word > rhs;}
	bool operator ==(const string & rhs) const{return word == rhs;}
	bool operator >=(const string & rhs) const {return ((*this) > rhs ) || ((*this) == rhs );}
	bool operator <(const string & rhs)  const {return !((*this) >= rhs );}
	bool operator <=(const string & rhs) const {return !((*this) > rhs );}
	bool operator !=(const string & rhs) const {return !((*this) == rhs );}
	friend bool operator>(const string& lhs,const wordRec& rhs) {return lhs>rhs.word;}
	friend bool operator==(const string& lhs,const wordRec& rhs) {return lhs==rhs.word;}
	friend bool operator>=(const string& lhs,const wordRec& rhs) {return (lhs > rhs.word ) || (lhs == rhs.word );}
	friend bool operator<(const string& lhs,const wordRec& rhs) {return !(lhs >= rhs.word );}
	friend bool operator<=(const string& lhs,const wordRec& rhs) {return !(lhs > rhs.word );}
	friend bool operator!=(const string& lhs,const wordRec& rhs) {return !(lhs == rhs.word );}


	friend ostream& operator << (ostream& out, const wordRec& rhs);

	string getWord() {return word;}
	lineRec getLineRec() {return *pLineRec;}
public:
	bool defcon;
	string word;
	lineRec *pLineRec;
};