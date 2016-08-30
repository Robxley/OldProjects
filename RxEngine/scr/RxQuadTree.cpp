#include "RxQuadTree.h"

RxQuadTree::RxQuadTree()
{
	SubdivisionFunction = NULL;
}

RxQuadTree::~RxQuadTree()
{
}

void RxQuadTree::Init(void* dataroot, bool (*SubdivFct)(void *, void **, void **, void **, void **) )
{
	Clear(); //on efface le contenu de l'arbre
	
	//creation de la racine de l'arbre
	RxQQuad root;
	root.data = dataroot;
	root.id = 0;

	//ajout de la racine à l'arbre
	tree.push_back(root);

	SubdivisionFunction = SubdivFct;

}

void RxQuadTree::ComputeTree() 
{
	unsigned int i = 0;

	void *dataChild0, *dataChild1, *dataChild2, *dataChild3;
		
	while( i < tree.size() )  //on parcourt l'abre
	{
		dataChild1 = dataChild2 = dataChild3 = dataChild0 = NULL;
		RxQQuad &dad = tree[i];
		if( (*SubdivisionFunction)( dad.data, &dataChild0, &dataChild1, &dataChild2, &dataChild3 ) ) //subdivision
		{
			//creation des 4 enfants
			RxQQuad _children[4];
			
			//ils ont tous le meme père
			_children[0].dad = _children[1].dad = _children[2].dad  =_children[3].dad = &dad;
			
			//enregistrement des pointeurs sur les données fournie par SubdivisionFunction
			_children[0].data = dataChild0;
			_children[1].data = dataChild1;
			_children[2].data = dataChild2;
			_children[3].data = dataChild3;

			unsigned m,j,k;	
			for(m = 0;  m < 4; m++)
			{
				unsigned int s = tree.size();
				_children[m].id = s ;
				tree.push_back(_children[m]);	//rajoute le fils à l'arbre
				dad.children[m] = &tree[s]; //rajoute le fils au père
			}

			//enregistrement des frères entre eux
			unsigned int s = tree.size()-4;  //on se place sur l'indice du premier enfant
			for(m = 0; m < 4; m++)
			{
				for(j = 0; j < 4; j++)
				{
					k = 0;
					if( m != j)
					{
						tree[s+m].brothers[k]= &tree[s+j];
						k++;
					}
				}
			}
			

		}
		else //si on subdivise pas c'est une feuille de l'arbre
		{
			leafs.push_back(&dad);
		}
		i++;
	}
}

void RxQuadTree::Clear()
{
	tree.clear();
	leafs.clear();
}

RxQQuad* RxQuadTree::getRoot()
{
	return &tree[0];
}

std::vector<RxQQuad*> *RxQuadTree::getListLeafs()
{
	return &leafs;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
