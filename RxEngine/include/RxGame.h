//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once

#ifndef _RXGAME_H_
#define _RXGAME_H_

#include "RxMath3D.h"
#include <string>
#include <vector>



class RxGameMenu
{
	struct OptionGame 
	{
		string name;
		int value;
	};
	string name; //nom du menu
	RxGameMenu* subMenu;
	
public:
	RxGameMenu();
	
};

class RxGame
{
	
	string lvldir;  //dossier contenant les différents niveaux
	string lvlbasename; //nom de base des niveaux
	unsigned int nb_lvl;
	public:

		RxGame();
		void NextLevel(); 
		void RestartGame();
		void RestartLevel();
		void EndGame();
		void LoadLevel();
		void LoadGameOptions();

		void Draw();

};

#endif