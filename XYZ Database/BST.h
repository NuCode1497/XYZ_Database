//Code by Cody Neuburger
//
//stand-alone Binary Search Tree
//syntax:
//BST<Type> bst;
//-or-
//xBST<Type, sType> bst;
//where Type is the type of the data pointed to by a node and sType is the
//type of data the tree is sorted by

#pragma once
#include <stdlib.h>
#include <vector>
using namespace std;

template <typename Type, typename sType>
class xBST;

template <typename T>
class BSTnode
{
	BSTnode(){pData=NULL;pRight=NULL;pLeft=NULL;}
	~BSTnode(){delete pData; pData=NULL;}
	template <typename Type, typename sType> friend class xBST;

	T * pData;
	BSTnode<T> * pLeft;
	BSTnode<T> * pRight;
};

//syntax: <type of BSTnode's data, sort-by type>
//make sure logical expressions are overloaded to test Type with sType
//for example: Type x; sType y; x == y; //must work
template <typename Type, typename sType>
class xBST
{
public:
	xBST(){pRoot=NULL;}
	void add(Type const & x);
	void remove(sType data);
	void clear();
	Type * find(sType data);
	vector<Type *> findMultiple(sType data);
	int count();
	void print();

	static BSTnode<Type> * pNull;
private:

	BSTnode<Type> * & findNodePtrRef(sType data);
	static BSTnode<Type> * & findNodePtrRef(sType data, BSTnode<Type> * & pParent);
	static BSTnode<Type> * & findLMNPtrRef(BSTnode<Type> * & pParent);

	static void remove(sType data, BSTnode<Type> * & pParent);
	static void add(Type const & x, BSTnode<Type> * & pParent);
	static Type * find(sType data, BSTnode<Type> * & pParent);
	vector<Type *> findMultiple(sType data, BSTnode<Type> * & pParent);
	static void addVectors(vector<Type *> &v1, vector<Type *> &v2);
	static void clear(BSTnode<Type> * & pParent);
	static void print(BSTnode<Type> * pParent);
	static int count(BSTnode<Type> * pParent);

	BSTnode<Type> * pRoot;
	BSTnode<Type> * pFind;
};

//use if the sort-by type is the same as the BSTnode's data's type
template <typename Type>
class BST
{
public:
	void add(Type const & x){bst.add(x);}
	void remove(Type data){bst.remove(data);}
	void clear(){bst.clear();}
	Type * find(Type data){return bst.find(data);}
	vector<Type *> findMultiple(Type data){return bst.findNext(data);}
	int count(){return bst.count();}
	void print(){bst.print();}
private:
	xBST<Type, Type> bst;
};





//declarations
//template declarations must be in header file
template <typename Type, typename sType>
BSTnode<Type> * xBST<Type, sType>::pNull=NULL;



template <typename Type, typename sType>
void xBST<Type, sType>::clear()
{
	clear(pRoot);
}

template <typename Type, typename sType>
void xBST<Type, sType>::clear(BSTnode<Type> * & pParent)
{
	if(pParent==NULL)
	{
		return;
	}
	clear((*pParent).pLeft);
	clear((*pParent).pRight);
	delete (*pParent).pData;
	(*pParent).pData = NULL;
	delete pParent;
	pParent = pNull;
}


template <typename Type, typename sType>
Type * xBST<Type, sType>::find(sType data)
{
	return find(data, pRoot);
}


//if data not found, returns a null Type*
template <typename Type, typename sType>
Type * xBST<Type, sType>::find(sType data, BSTnode<Type> * & pParent)
{
	if(pParent==NULL)
	{
		return NULL;
	}

	if( (*(*pParent).pData) == data )
	{
		//found
		return (*pParent).pData;
	}

	if( (*(*pParent).pData) < data )
	{
		//go left
		return find(data, (*pParent).pLeft);
	}else{
		//go right
		return find(data, (*pParent).pRight);
	}
}

template <typename Type, typename sType>
vector<Type *> xBST<Type, sType>::findMultiple(sType data)
{
	return findMultiple(data, pRoot);
}

template <typename Type, typename sType>
vector<Type *> xBST<Type, sType>::findMultiple(sType data, BSTnode<Type> *&pParent)
{
	vector<Type *> result;
	if(pParent==NULL) return result;

	if( (*(*pParent).pData) == data )
	{
		result.push_back((*pParent).pData);
		addVectors(result,findMultiple(data, (*pParent).pRight));
		return result;
	}
	if( (*(*pParent).pData) < data )
	{
		return findMultiple(data, (*pParent).pLeft);
	}else{
		return findMultiple(data, (*pParent).pRight);
	}
}


template <typename Type, typename sType>
void xBST<Type, sType>::addVectors(vector<Type *> &v1, vector<Type *> &v2)
{
	for(unsigned int i=0;i<v2.size();i++)
	{
		v1.push_back(v2[i]);
	}
}

template <typename Type, typename sType>
void xBST<Type, sType>::add(Type const &x)
{
	add(x, pRoot);
}

template <typename Type, typename sType>
void xBST<Type, sType>::add(Type const &x, BSTnode<Type> *&pParent)
{
	if(pParent==NULL)
	{
		//tree is empty
		//base case
		pParent = new BSTnode<Type>;
		(*pParent).pData = new Type;
		(*(*pParent).pData) = x;
		return;
	}
	//tree not empty
	if( (*(*pParent).pData) < x)
	{
		//bounce left
		add(x, (*pParent).pLeft);
	}else{
		add(x, (*pParent).pRight);
	}
}


template <typename Type, typename sType>
void xBST<Type, sType>::remove(sType data)
{
	remove(data, pRoot);
}

template <typename Type, typename sType>
void xBST<Type, sType>::remove(sType data, BSTnode<Type> *& pParent)
{
	BSTnode<Type> * & refpdel = findNodePtrRef(data, pParent);
	if(refpdel==NULL) return;

	//CASE 1 -- no children
	if( ((*refpdel).pLeft==NULL) && ((*refpdel).pRight==NULL) )
	{
		delete refpdel;
		refpdel=NULL;
	}

	//CASE 2 -- right child
	else if( ((*refpdel).pLeft==NULL) && ((*refpdel).pRight!=NULL) )
	{
		BSTnode<Type>* temp;
		temp = refpdel;
		refpdel=(*refpdel).pRight;
		delete temp;
	}

	//CASE 3 -- left child
	else if( ((*refpdel).pLeft!=NULL) && ((*refpdel).pRight==NULL) )
	{
		BSTnode<Type>* temp;
		temp = refpdel;
		refpdel=(*refpdel).pLeft;
		delete temp;
	}

	//CASE 4 -- 2 children
	else if( ((*refpdel).pLeft!=NULL) && ((*refpdel).pRight!=NULL) )
	{
		BSTnode<Type> * temp;
		temp = refpdel;
		BSTnode<Type> *&refpnext = findLMNPtrRef((*refpdel).pRight);

		//swap
		refpdel = refpnext;
		refpnext = temp;
		temp = (*refpnext).pLeft;
		(*refpnext).pLeft = (*refpdel).pLeft;
		(*refpdel).pLeft = temp;
		temp = (*refpnext).pRight;
		(*refpnext).pRight = (*refpdel).pRight;
		(*refpdel).pRight = temp;

		remove(data, refpdel);
	}
}


template <typename Type, typename sType>
BSTnode<Type> * & xBST<Type, sType>::findNodePtrRef(sType data)
{
	return findNodePtrRef(data, pRoot);
}


template <typename Type, typename sType>
BSTnode<Type> * & xBST<Type, sType>::findNodePtrRef(sType data, BSTnode<Type> * & pParent)
{
	if(pParent==NULL)
	{
		return pNull;
	}
	if( data == (*(*pParent).pData) )
	{
		return pParent;
	}
	if( data < (*(*pParent).pData) )
	{
		//go left
		return findNodePtrRef(data, (*pParent).pLeft);
	}
	else
	{
		// go right
		return findNodePtrRef(data, (*pParent).pRight);
	}
}



template <typename Type, typename sType>
BSTnode<Type> * & xBST<Type, sType>::findLMNPtrRef(BSTnode<Type> * & pParent)
{
	if(pParent==NULL)
	{
		return pNull;
	}
	if( (*pParent).pLeft == NULL )
	{
		return pParent;
	}
	return findLMNPtrRef((*pParent).pLeft);
}



template <typename Type, typename sType>
int xBST<Type, sType>::count()
{
	return count(pRoot);
}

template <typename Type, typename sType>
int xBST<Type, sType>::count(BSTnode<Type> * pParent)
{
	if(pParent==NULL)
	{
		return 0;
	}
	int total=0;

	total+=count((*pParent).pLeft);   //left

	total++;      //root

	total+=count((*pParent).pRight);  //right

	return total;
}

template <typename Type, typename sType>
void xBST<Type, sType>::print()
{
	print(pRoot);
}

template <typename Type, typename sType>
void xBST<Type, sType>::print(BSTnode<Type> * pParent)
{
	if(pParent==NULL)
	{
		return;
	}
	print((*pParent).pLeft);
	cout<<(*(*pParent).pData)<<endl;
	print((*pParent).pRight);
}
