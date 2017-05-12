#include "BST.h"
#include "wordRec.h"
#include "DataBase.h"
#include <string>
#include <iostream>
using namespace std;

void CommandLineInterface(DataBase & DB)
{
	string command;
	cout<<DB.transducer((string)"HELP")<<endl;
	do
	{
		cout<<">";
		getline(cin,command);
		cout<<DB.transducer(command)<<endl;
	} while (command != "QUIT");
}


int main()
{
	////BST test
	//BST<string> b;
	//b.add("tacos");
	//b.add("forks");
	//b.add("carrots");
	//cout<<"Before removing data"<<endl;
	//b.print();
	//b.remove("forks");
	//cout<<"After removing data"<<endl;
	//b.print();

	DataBase DB;
	CommandLineInterface(DB);
	return 0;
}

