//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXTREESTRUCT_H_
#define _RXTREESTRUCT_H_
#include <vector>

#include "RxObject3D.h"
#include "RxLogger.h"
#include "RxMath3D.h"





template <typename T>
struct RxNode;

template <typename T>
class RxTree;



typedef struct _NodeGraphScene
{
	glm::mat4 local, global;

	std::vector<RxItem3D*> roomObjects;
	RxLight *ptrLight;

} RxRoomScene;


typedef RxTree<RxRoomScene> RxGraphScene;

template <typename T>
struct RxNode
	{
	//private :
		RxNode<T> *parent;
		std::vector<RxNode<T>*> children;
		int currentChild;

		T data;
	//public :



		RxNode(const typename T& _data)
		{
			data = _data;
			currentChild = -1;
			parent = NULL;
		}
		void ResetCurrentChild()
		{
			currentChild = -1;
		}
		int IsEmpty()
		{
			return children.empty();
		}

		unsigned int nbChildren()
		{
			return children.size();
		}

		RxNode<T>* getCurrentChild()
		{
			if(currentChild >=  (int)nbChildren() || IsEmpty())
			{
				currentChild = nbChildren();
				return NULL;
			}
			else if (currentChild == -1)
			{
				return NULL;
			}
			else
				return children[currentChild];
		}
		int IsReading()
		{
			return (currentChild < (int)children.size());
		}

		//renvoie Null ou le prochain children
		RxNode<T>* getNextChild()
		{
			currentChild++;
			return getCurrentChild();
		}
		//renvoie Null ou le children


		RxNode<T>* getParent()
		{
			return parent;
		}

};

template <typename T>
class RxTree
{
	RxNode<T> *current;
	public:
		RxTree();
		~RxTree();

		void Clear();


		RxNode<T> * CreateRoot(const typename T &child);
		RxNode<T> * AddChild( const typename T &child, RxNode<T> *parentNode = NULL);

		RxNode<T> * ResetRun(); //replace le curseur au début de l'arbre
		RxNode<T> * NextNodeDepthRun(); //passe au noeud suivant
		RxNode<T> * NextNodeWidthRun(); //passe au noeud suivant


		RxNode<T>* getRoot();  //debut de l'arbre
		RxNode<T>* getCurrentNode(); //renvoie le noeud actif
		void setCurrentNode(RxNode<T>* _current);

	private:

		std::vector<RxNode<T>*> tree; //contient l'ensemble des noeuds de l'arbres.
		RxNode<T> * root;

};


template <typename T>
RxTree<T>::RxTree()
{
	current = NULL;
	root = NULL;
}

template <typename T>
RxNode<T> * RxTree<T>::NextNodeDepthRun()
{
	if(tree.empty()) return 0;

	RxNode<T> *nextCurrent = current;


	while( root->IsReading() ) //si on a parcouru tous les oeufs de la mère poule on tombe sur le commencement de l'oeuf ou de la poule == NULL
	{
		if( nextCurrent->getNextChild() ) //si elle a des oeufs et suffisamment assez
		{
				return current = nextCurrent->getCurrentChild();
		}
		else if( nextCurrent != root) //bon, elle n'a pas eu d'oeuf ou n'en a plus, on retourne voir la mère pour passer à la soeur
		{
			nextCurrent = nextCurrent->getParent();
		}
	}

	current = NULL;
	return NULL;

}




template <typename T>
RxNode<T> * RxTree<T>::NextNodeWidthRun()
{
	if(tree.empty()) return 0;

	RxNode<T> *nextCurrent = current;


	while( root->IsReading() ) //si on a parcouru tous les oeufs de la mère poule on tombe sur le commencement de l'oeuf ou de la poule == NULL
	{
		if( nextCurrent->getNextChild() ) //si elle a des oeufs et suffisamment assez
		{
				return current = nextCurrent->getCurrentChild();
		}
		else if( nextCurrent != root) //bon, elle n'a pas eu d'oeuf ou n'en a plus, on retourne voir la mère pour passer à la soeur
		{
			nextCurrent = nextCurrent->getParent();
		}
	}

	current = NULL;
	return NULL;

}


template <typename T>
RxTree<T>::~RxTree()
{
	Clear();
}

template <typename T>
void RxTree<T>::Clear()
{
	for(unsigned int i = 0; i < tree.size(); i++)
		delete tree[i];

	current = NULL;
	root = NULL;
	tree.clear();
}

template <typename T>
RxNode<T> * RxTree<T>::ResetRun()
{
	for(unsigned int i=0; i < tree.size(); i++)
		tree[i]->ResetCurrentChild();

	if(tree.size())
		current = root;
	else
		current = NULL;

	return current;
}


template <typename T>
RxNode<T> * RxTree<T>::AddChild( const typename T &child, RxNode<T> *parentNode = NULL)
{
	if(!parentNode) parentNode = current;
	RxNode<T> * c = new RxNode<T>(child);
	tree.push_back(c);

	if(parentNode) //non null
	{
		RxNode<T> *child = tree.back();
		child->parent = parentNode;
		parentNode->children.push_back(child);
	}
	else
	{//null, c'est le premier, c'est la racine de l'abre
		RxNode<T> *child = tree.back();
		child->parent = NULL;
		current = tree.front();
		root = tree.front();
	}

	return tree.back();
}

template <typename T>
RxNode<T> * RxTree<T>::CreateRoot(const typename T &child)
{
	tree.push_back(new RxNode<T>(child));
	root = tree.front();
	return root;
}


template <typename T>
RxNode<T>* RxTree<T>::getRoot()
{
	return root;
}

template <typename T>
RxNode<T>* RxTree<T>::getCurrentNode()
{
	return current;
}

template <typename T>
void RxTree<T>::setCurrentNode(RxNode<T>* _current)
{
	current = _current;
}


#endif
