//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once
#ifndef _RXQUADTREE_H_
#define _RXQUADTREE_H_


#include <vector>


typedef struct RxQQuad
{
	void *data;

	unsigned int id;

	struct RxQQuad* children[4];
	struct RxQQuad* dad;
	struct RxQQuad* brothers[3];
	
	RxQQuad()
	{
		dad = NULL;
		children[0] =  children[1] = children[2] = children[3] = NULL;
		brothers[0] = brothers[1] = brothers[2] = NULL;
		data = NULL;
		id = 0;
	}
} RxQQuad;


class RxQuadTree
{
	private:
		
		std::vector<RxQQuad> tree; //arbre complet dont la racine est le premier élément
		std::vector<RxQQuad*> leafs; //ensemble des feuilles de l'arbre
		
		//fonction test de subdivision d'une feuille (1 : subdivision , 0 : non subdivision)
		bool (*SubdivisionFunction)(void *datadad, void **datachild1, void **datachild2, void **datachild3, void **datachild4); 


	public:

		RxQuadTree();
		~RxQuadTree();

		void Init(void* dataroot, bool (*SubdivFct)(void *, void **, void **, void **, void **) );
		void ComputeTree(); //calcul l'arbre
		void Clear(); //suppression de l'arbre.

		RxQQuad *getRoot(); //retourne la racine de l'arbre
		std::vector<RxQQuad*> *getListLeafs(); //retourne la liste des feuilles de l'arbres

};



template<class T> class RxTreeNode
{
	private:
		static unsigned int GenID()
		{
			static unsigned int idcount = 0;
			idcount++;
			return idcount;
		}
	public:
		T *data;

		std::vector<RxTreeNode<T>*> children;
		RxTreeNode<T> *dad;

		unsigned int id;

		RxTreeNode(RxTreeNode<T> & tnode)
		{
			data = tnode.data;
			children = tnode.children;
			dad = tnode.dad;
			id = tnode.id;
		}
		RxTreeNode(const RxTreeNode<T> & tnode)
		{
			data = tnode.data;
			children = tnode.children;
			dad = tnode.dad;
			id = tnode.id;
		}

		RxTreeNode(RxTreeNode<T>* _dad = NULL, T *_data = NULL)
		{
			data = _data;
			dad = _dad;
			id = GenID();

			//on oublie pas de dire au père qu'il a un nouveau fils. 
			//Ce n'est pas facile pour la mère d'élever un enfant seule tout du moins si elle se rapelle du père.
			if(dad)
				dad->children.push_back(this);
		
		}
		~RxTreeNode() //mort subite; il faut avertir la famille
		{
			unsigned int i;
			if(dad) //avertir le père qu'il a perdu un fils, si père il y a
			{
				
				for( i = 0 ; i < dad->children.size() ; i++)
				{
					if(	children[i] == this )
					{
						children[i] == NULL; //mort du fils
						break;
					}
				}
			}

			//on avertie les enfants que leur père est mort, le grand père ce retrouve avec les gamins sur les bras.
			//c'est parfois triste la vie
			if(dad)
			{
				for(i = 0; i < children.size() ; i++)
				{
					if(children[i])  //vérifie que le fils est toujours vivant.
						children[i]->dad = dad->dad; //grand père arrive au secours de ces petits enfants
				}
			}
			else //pas de chance c'est adam qui est mort
			{

				//le premier fils d'adam devient responsable de la famille
				if(children.empty()) return; //si adam est mort avant d'avoir rencontré Eve, et n'a pas eu d'enfant.
				for(i = 0; i < children.size() && !children[i]; i++); //recherche du premier enfant tjrs vivant.
				if(!children[i]) return; //tous les enfants sont déjà morts.

				unsigned int j;
				for(j = i+1; j < children.size(); j++)
				{
					if(children[j])
						children[j]->dad = children[i];  //le grand frère devient chef de famille
				}
				
				children[i]->dad = NULL; //Le grand frère devient le chef de la famille
				
			}
		}

		//si c'est un noeud (donc possède des enfants)
		bool IsNode()
		{
			return children.size();
		}

		//si c'est une feuille donc ne possède pas d'enfant
		bool IsLeaf()
		{
			return children.empty();
		}

		//si c'est un enfant unique, si pas de père on considère comme enfant unique ne connaissant pas à priori ces frères
		bool IsUniqueChild()
		{
			if (!dad) return 1;
			return dad->children.empty();
		}

		std::vector<RxTreeNode<T>*> *Brothers()
		{
			if(!dad) return NULL;
			return &dad->children;
		}
};


template<class T> class RxGenericTree
{
	std::vector<RxTreeNode<T>> tree;
};


#endif