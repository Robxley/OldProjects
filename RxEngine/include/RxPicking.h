//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXPICKING_H_
#define _RXPICKING_H_

#include <GL/glew.h>

#include <vector>
#include "RxMath3D.h"
#include "RxFBO.h"
#include "RxShader.h"
#include "RxActor.h"
#include "RxObject3D.h"

#define RX_PICKING_VERT "shaders/picking.vert"
#define RX_PICKING_FRAG "shaders/picking.frag"

class RxPicking
{
	public:

		//initialisation de la méthode (VBO, shader)
		//precision_ratio : paramétre de précision sur le FBO utilisé pour le picking
		//pour un ratio de 1 le viewport du fbo sera le meme que le viewport courant
		//pour un ratio de 2 le viewport du fbo sera moitié moins grand (largeur/ratio, hauteur/ratio)
		//pour un ratio x : viewport -> (largeur/x, hauteur/x)...
		RxPicking(unsigned int precision_ratio = 1);

		//On démarre les dessins dans un contexte de color picking
		virtual void BeginPickingMod();

		//on arrete le picking
		virtual void EndPigkingMod();

		//convertion de la clé d'identification en un couleur
		virtual ivec3 Key2Color(unsigned int _key);

		//convertion de la couleur en clé
		virtual unsigned int Color2key(ivec3 _color);

		//lecture de la clé en au pixel (x,y)
		virtual unsigned int ReadKeyInBuffer(ivec2 position);
		virtual unsigned int ReadKeyInBuffer(unsigned int x, unsigned int y);

		virtual void setPrecisionRatio(unsigned int ratio);
		virtual RxFBO*getFBO();

		virtual ivec2 getCurrentViewPortSize(){return realSize;}
		virtual unsigned int getPrecisionRatio(){return fbo_ratio;}
		

		//objet opengl
		RxFBO fbo;
		RxShader shader;

		//sauvegarde de variable opengl
		vec4 save_clear_color;
		vec4 clear_color_picking;

		//options du picking
		unsigned int fbo_ratio;
		ivec2 realSize;
		
};


class RxPickedItem
{
	public:
		RxPickedItem();
		virtual ~RxPickedItem();
		bool pickable;
		bool picked;

		//fonction callback qui sera automatiquement appeler si 
		// l'objet est selection avec data
		virtual void CallBackFct() = 0;

		virtual void Draw() = 0;

		void *data;

		//information sur le clique
		unsigned int x,y;
		unsigned int boutonState;

	private:
		unsigned int key;

};


class RxPickedItem3D : public RxPickedItem
{
	public:
		RxPickedItem3D();
		RxItem3D *pItem3D;
		mat4 transform;
		virtual void Draw();
		virtual void CallBackFct();
};
	
class RxPickedActor : public RxPickedItem
{
	public:
		RxPickedActor();
		RxActor *pActor;
		virtual void Draw();
		virtual void CallBackFct();
};


class RxPickingManager
{
	public:
		RxPickingManager(unsigned int precision_ratio = 1);
		vector<RxPickedItem*> tabPickedItem;

		void AddPikedItem(RxPickedItem *pItem);


		void DrawPickingMod(int mx, int my);

		RxPickedItem *getWhoIsPicked();

		unsigned int getKey();

		mat4 modelViewMat;
		//mat4 projectionMat;

		RxPicking picker;
	private:

		



		int pickedKey;
};


#endif //_RXPICKING_H_