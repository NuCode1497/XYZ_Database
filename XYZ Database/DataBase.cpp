#include "DataBase.h"
#include <fstream> 
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

string toLower(const string & s)
{
	string result=s;
	for(unsigned int i=0;i<result.length();i++)result[i]=tolower(result[i]);
	return result;
}


DataBase::DataBase()
{
	string filename="";
	ifstream in;
	in.open("database.txt");
	char c;
	while(!in.eof())
	{
		filename="";
		do{
			in.get(c);
			if(in.eof() || c=='\n')break;
			filename.push_back(c);
		}while(!in.eof() && c != '\n');
		if(filename != "" && !in.eof())addFile(filename);
	}
	in.close();
}


bool DataBase::addFile(string filename)
{
	ifstream in;
	in.open(filename.c_str());
	if (in.fail()){cout<<"ERROR:Failed to add "<<filename<<endl; return 0;}

	lineRec * pLineRec;
	wordRec * pWordRec;
	string * pFilename;
	string line="", word="";
	vector<string> words;
	int lineNumber=0;

	pFilename = new string;
	(*pFilename) = filename;
	filenames.push_back(*pFilename);
	while(!in.eof())
	{
		//get a line
		lineNumber++;
		line="", word="";
		char c;
		do{
			in.get(c);
			if(in.eof() || c=='\n')break;
			line.push_back(c);
		}while(!in.eof() && c != '\n');

		//create lineRec
		pLineRec = new lineRec(line, pFilename, lineNumber);
		lineRecs.push_back(*pLineRec);

		//find words in line
		unsigned int S=0, E=0;
		while(S<line.length() && E<line.length())
		{
			while(S<line.length() && !isValidChar(line[S]))S++;
			E=S;
			while(E<line.length() && isValidChar(line[E]))E++;
			words.push_back(line.substr(S, E-S));
			S=E;
		}

		//create and add wordRec to BST
		for(unsigned int i=0;i<words.size();i++)
		{
			word = toLower(words[i]);
			pWordRec = new wordRec(word, pLineRec);
			wordRecs.add(*pWordRec);
		}
		words.clear();
	}

	in.close();
	return 1;
}

bool DataBase::isValidChar(char c)
{
	return ( (c>='a'&&c<='z') || (c>='A'&&c<='Z') );
}

bool DataBase::findWord(const string &word)
{
	vector<wordRec *>hits = wordRecs.findMultiple(word);
	if(hits.empty()){ return 0; }
	removeDuplicates(hits);
	vector<lineRec *>Hits = wordsetToLineset(hits);

	lineRec * result;
	vector<lineRec *>::iterator iter;
	iter=Hits.begin();
	while(iter!=Hits.end())
	{
		result = (*iter);
		cout<<endl;
		cout<<(*result).getLine()<<endl;
		cout<<(*result).getFilename();
		cout<<" ["<<(*result).getLineNum()<<"]"<<endl;
		iter++;
	}
	return 1;
}

bool DataBase::query(const string &expression)
{
	vector<lineRec *>hits = findSet(expression);
	if(hits.empty()){ return 0; }

	lineRec * result;
	vector<lineRec *>::iterator iter;
	iter=hits.begin();
	while(iter!=hits.end() && (*iter) != NULL)
	{
		result = (*iter);
		cout<<endl;
		cout<<(*result).getLine()<<endl;
		cout<<(*result).getFilename();
		cout<<" ["<<(*result).getLineNum()<<"]"<<endl;
		iter++;
	}
	return 1;
}

void DataBase::removeDuplicates(vector<wordRec *> &words)
{
	wordRec * result;
	wordRec * prev;
	wordRec * temp = new wordRec();
	if(words.empty()){delete temp; return; }

	vector<wordRec *>::iterator iter;
	iter=words.begin();
	prev = temp;
	while(iter!=words.end())
	{
		result = (*iter);
		if ((*prev).pLineRec == (*result).pLineRec){iter=words.erase(iter); continue;}
		prev = (*iter);
		iter++;
	}

	delete temp;
}

//translate word-set to line-set
vector<lineRec *> DataBase::wordsetToLineset(vector<wordRec *> &words)
{
	vector<wordRec *>::iterator iter;
	vector<lineRec *> result;
	if(words.empty()){ return result; }

	iter=words.begin();
	while(iter!=words.end())
	{
		result.push_back((*(*iter)).pLineRec);
		iter++;
	}
	return result;
}

bool isOperation(char c)
{
	if( c == '|' ||
		c == '&' )
	{
		return 1;
	}
	return 0;
}

vector<lineRec *> DataBase::findSet(const string &expression)
{
	//s-expression tree
	string operation, Lexpression, Rexpression;
	vector<lineRec *> Lset, Rset, result;
	vector<wordRec *> hits;
	int Oparens=0, Cparens=0, parens=0;

	unsigned int i=0;
	bool LorR=0;
	char c;
	while(i<expression.length())
	{
		c = expression[i];

		if(c == '('){ Oparens++; parens = Oparens - Cparens; }
		if(c == ')'){ Cparens++; parens = Oparens - Cparens; }
		if(parens < 0){ cout<<"ERROR: missing ("<<endl; return result; }  // expression )
		if(parens == 0 && c == ' '){ i++; continue; }
		if(LorR==0)
		{
			//build left expression
			if(parens > 0)
			{
				Lexpression.push_back(c);
			}
			else if ( !isOperation(c) )
			{
				Lexpression.push_back(c);
			}
			else
			{
				operation.push_back(c);
				LorR=1;
			}
		}
		else
		{
			//build right expression
			if(parens > 0)
			{
				Rexpression.push_back(c);
			}
			else if ( isOperation(c) && Rexpression.empty() )
			{
				operation.push_back(c);
			}
			else
			{
				Rexpression.push_back(c);
			}
		}

		i++;
	}

	//missing expression
	if(!operation.empty() && Rexpression.empty())
	{ // expression &
		cout<<"WARNING: missing right expression"<<endl;
		return findSet(Lexpression.substr(0,Lexpression.size()-1));
	}
	if(!operation.empty() && Lexpression.empty())
	{ // & expression
		cout<<"WARNING: missing left expression"<<endl;
		return findSet(Rexpression.substr(1,Rexpression.size()-1));
	}
	//lone expression
	if(operation.empty() && Rexpression.empty())
	{
		//evaluate inside expressions
		if(parens>0) return findSet(Lexpression.substr(1,Lexpression.size()-1));  // ( expression
		else if (Oparens>0 && Cparens>0) return findSet(Lexpression.substr(1,Lexpression.size()-2)); // ( expression )

		//base expression
		hits = wordRecs.findMultiple(expression);
		removeDuplicates(hits);
		return wordsetToLineset(hits);
	}
	
	Lset = findSet(Lexpression);
	Rset = findSet(Rexpression);

	//evaluate this expression
	sort(Lset.begin(), Lset.end());
	sort(Rset.begin(), Rset.end());
	vector<lineRec *>::iterator iter;

	if(operation == "&")
	{
		result.resize( min(Lset.size(), Rset.size()) );
		iter = set_intersection(Lset.begin(), Lset.end(), Rset.begin(), Rset.end(), result.begin());
	}
	if(operation == "|")
	{
		result.resize( Lset.size() + Rset.size() );
		iter = set_union(Lset.begin(), Lset.end(), Rset.begin(), Rset.end(), result.begin());
	}

	return result;

}



void DataBase::clearDB()
{
	filenames.clear();
	lineRecs.clear();
	wordRecs.clear();
}


void DataBase::saveDB()
{
	string filename = "database.txt";
	ofstream out(filename.c_str(), ios::trunc);
	if (out.fail()){cout<<"ERROR: Could not save to "<<filename<<endl;}

	list<string>::iterator iter;
	iter=filenames.begin();
	while(iter!=filenames.end())
	{
		out<<(*iter)<<endl;
		iter++;
	}
	out.close();
}


string DataBase::transducer(string command)
{
	string result;
	if(command.substr(0,8)=="ADD_FILE")
	{
		if( command.length() <=9)
		{
			result = "Enter a file name.";
		}else
		{
			string filename = command.substr(9,9999);

			if(addFile(filename))
			{
				result="Added ";
				result+=filename;
				result+=" to the database.";
			}else{
				result="ERROR: Failed to add " + filename;
			}
		}
	}
	else if(command.substr(0,9)=="FIND_WORD")
	{
		if( command.length() <=10)
		{
			result = "Enter a word.";
		}else{
			string word = command.substr(10,9999);

			if(!findWord(word))
			{
				result = "\"" + word + "\" not found.";
			}
		}
	}
	else if(command.substr(0,5)=="QUERY")
	{
		if( command.length() <=6)
		{
			result = "Enter an expression.";
		}else{
			string word = command.substr(6,9999);

			if(!query(word))
			{
				result = "Could not find anything";
			}
		}
	}
	else if(command.substr(0,5)=="CLEAR")
	{
		clearDB();
	}
	else if(command.substr(0,9)=="SAVE")
	{
		saveDB();
	}
	else if(command.substr(0,4)=="QUIT")
	{
		saveDB();
		clearDB();
		result="Goodbye";
	}
	else if(command.substr(0,4)=="HELP")
	{
		result="The following commands are available:\n";
		result+="\tADD_FILE <filename>\n";
		result+="\tFIND_WORD <word>\n";
		result+="\tQUERY <expression>\n";
		result+="\tCLEAR\n";
		result+="\tHELP\n";
		result+="\tSAVE\n";
		result+="\tQUIT\n";
	}
	else
	{
		result="ERROR: The command <" + command + "> is not recognized.";
	}
	return result;
}