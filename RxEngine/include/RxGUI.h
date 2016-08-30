//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once

#ifndef _RXGUI_H_
#define _RXGUI_H_


#include <tinyxml.h>
#include <vector>
#include <string>


#include "RxTexManager.h"
#include "RxWindow.h"
#include "RxTools.h"




class RxGUI
{
	public:
		RxGUI();
		~RxGUI();

		//rajout d'un item à l'interface GUI
		RxItem2D* AddItem2D(RxItem2D &item); 
		
		//fonction de chagement du GUI à partir d'un fichier de description XML
		int LoadXMLGUI(const char *filename);
		
		//affichage du GUI
		void Draw();

		//appelle des fct callback des différents objets.
		void CallBackAllFct();

		//permet d'identifier les fonctions callback à un objet 2d
		void setCallBactFct(std::string itemname, void (*callBackFct)(int));
		void setCallBactFct(RxItem2D *item, void (*callBackFct)(int));
		void setCallBactFct(int itemid, void (*callBackFct)(int));


		void setFont( RxFont *_font);
		void setFather(RxItem2D* father = NULL);
	private:

		struct RxControler
		{
			int id;
			std::string name;
			RxItem2D *item;
			void (*callBackFct)(int);


			RxControler()
			{
				item = NULL;
				callBackFct = NULL;
			}

			void release()
			{
				delete item;
				item = NULL;
			}
		};


		std::vector<RxControler> tabItem2D;
		

		//lecture des valeurs communes à chaque objet
		bool GlobalItemValues( RxItem2D *item, TiXmlElement * pElem2 ); //return 1 si une valeur a été lu, 0 sinon
		
		void ItemReader(RxItem2D *item,TiXmlElement *pElem);
		void ButtonReader(RxButton *item,TiXmlElement *pElem);
		void TextBoxReader(RxTextBox *item,TiXmlElement *pElem);
		void setDefaultValues(RxItem2D *item);

		void Release();

	private:
		std::string name;

		RxFont *font;

		std::vector<idXML2idGL> idtextab; //liaison entre l'id des textures du fichier XML et de RxTexManager
		//defaut item
		RxItem2D defaultitem;

};

#endif