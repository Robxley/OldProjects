
//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxLevelGen.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


//vector<door*> RxLevelGen::door::all_doors;

RxLevelGen::RxLevelGen(): 
width(128),height(128),
min_doors(1), max_doors(2),
min_dungeons(1), max_dungeons(1),
min_decors(200), max_decors(200)
{
	  /* initialize random seed: */
	  srand ( time(NULL) );
}

RxLevelGen::~RxLevelGen() 
{

}

void RxLevelGen::Init()
{

}

void RxLevelGen::NewLevel(int _levelWidth, int _levelHeight, int _genroot)
{

}

void RxLevelGen::ClearLevel()
{

}

void RxLevelGen::DrawLevel()
{

}

void RxLevelGen::ComputeBorder()
{
	borderLeft.line.clear();
	borderRight.line.clear();
	borderTop.line.clear();
	borderBottom.line.clear();

	borderLeft.line.resize(height,0);			//			top
	borderRight.line.resize(height,0);			//		-------------
	borderTop.line.resize(width,0);				//		|			|
	borderBottom.line.resize(width,0);			//left	|		   h| right   
												//		|	  w		|
												//		-------------
												//			bottom



	  /* generate secret number: */
	  int decal;
	  int k;
	  borderTop.line[0] = init_decal_border;
	  borderBottom.line[0] = init_decal_border;
	  for(k = 1; k < width; k++)
	  {
		  //bord haut
		  decal = rand() % 3 - 1;  //entier entre -1 et 1
		  borderTop.line[k] =   borderTop.line[k-1]+decal;
		  if( borderTop.line[k] < 0)  borderTop.line[k] = 1;

		  //bord bas
		  decal = rand() % 3 - 1;
		  borderBottom.line[k] =   borderBottom.line[k-1]+decal;
		  if( borderBottom.line[k] < 0)  borderBottom.line[k] = 1;
	  }

	  borderLeft.line[0] = init_decal_border;
	  borderRight.line[0] = init_decal_border;
	  for(k = 1; k < height; k++)
	  {
		  //bord gauche
		  decal = rand() % 3 - 1;
		  borderLeft.line[k] =   borderLeft.line[k-1]+decal;
		  if( borderLeft.line[k] < 0)  borderLeft.line[k] = 1;

		  //bord droit
		  decal = rand() % 3 - 1;
		  borderRight.line[k] =   borderRight.line[k-1]+decal;
		  if( borderRight.line[k] < 0)  borderRight.line[k] = 1;
	  }



}


void RxLevelGen::ComputeObject()
{
	//génération des portes
	int nb_doors;
	if(max_doors > 4) max_doors = 4;
	if(max_doors != min_doors)
		nb_doors = (rand() % (max_doors-min_doors+1))+min_doors; 
	else 
		nb_doors = max_doors;

	tab_doors.resize(nb_doors);

	int k;
	vector<unsigned int> unpicked_border;
	unpicked_border.resize(4);
	unpicked_border[0] = 0; //left
	unpicked_border[1] = 1; //right
	unpicked_border[2] = 2; //top
	unpicked_border[3] = 3; //bottom


	unsigned int border_range[4];

	border_range[0] = cornerLeftTop.y-cornerLeftBottom.y;		//left
	border_range[1] = cornerRightTop.y-cornerRightBottom.y;		//right
	border_range[2] = cornerRightTop.x-cornerLeftTop.x;			//top
	border_range[3] = cornerRightBottom.x-cornerLeftBottom.x;	//bottom

	for(k = 0; k < nb_doors; k++)
	{
		tab_doors[k].size = (rand() % (max_size_doors-min_size_doors+1))+min_size_doors;
		int nborder = rand()%unpicked_border.size(); //selection du border
		tab_doors[k].from_border = unpicked_border[nborder];
		unpicked_border.erase(unpicked_border.begin()+nborder); //suppression du border utilisé de la liste des borders
		
		//position entre les coins droite-gauche / haut-bas et prise en compte de la taille de la porte
		tab_doors[k].position = rand()% (border_range[tab_doors[k].from_border] - tab_doors[k].size);
	}
}