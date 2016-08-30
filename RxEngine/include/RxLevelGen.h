

//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once
#ifndef _RXLEVELGEN_H_
#define _RXLEVELGEN_H_

#include "RxScene.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

class RxLevelGen
{

private:

	//frontière des niveaux
	typedef struct border
	{
		vector<int> line;
	} border;

	//porte liant deux niveaux
	typedef struct door
	{
		static vector<door*> all_doors;
		int position, size;
		int from_border;
		void *data;
	}door;

	//donjon
	typedef struct 	dungeon
	{
		static vector<dungeon*> all_dungeons;
		ivec2 position;
		void *data;
	}dungeon;

	//décor
	typedef struct decor
	{
		static vector<decor*> all_decors;
		unsigned int type;
		vec2 position;
		void *data;
	}decor;

	private:
		RxScene scene;
		int width, height;

		//bordure de map
		border borderLeft, borderRight, borderTop, borderBottom;
		ivec2 cornerLeftTop, cornerLeftBottom, cornerRightTop, cornerRightBottom;

		//overture
		vector<door> tab_doors;
		unsigned int max_doors, min_doors;
		unsigned int max_size_doors, min_size_doors;
	
		//souterrain/donjon
		vector<dungeon> tab_dungeons;
		unsigned int max_dungeons, min_dungeons;
		
		//objet decor
		vector<decor> tab_decors;
		unsigned int max_decors, min_decors;
		

		unsigned int init_decal_border;
		void ComputeBorder();
		void ComputeObject();

		
	public:
		RxLevelGen();
		~RxLevelGen();
		void Init();
		void NewLevel(int _levelWidth, int _levelHeight, int _genroot);
		void ClearLevel();
		void DrawLevel();


	private:

};
#endif