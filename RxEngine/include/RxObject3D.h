//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once
#ifndef _RXOBJECT3D_H_
#define _RXOBJECT3D_H_

#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <iostream>
#include <vector>
#include "RxConst.h"
#include "RxVBO.h"
#include "RxTexManager.h"
#include "RxLogger.h"
#include "RxShader.h"
#include "RxMath3D.h"



/************************************************************/
/*********** Listes des classes******************************/ 
/************************************************************/
/*															*/
/************************************************************/
/************************************************************/
/*															*/
/*		struct RxFace;										*/
/*		class RxLight;										*/
/*		class RxMaterial;									*/
/*		class RxItem3D;										*/
/*															*/
/************************************************************/




/************************************************************/
/*********** RxFace			*********************************/ 
/************************************************************/
typedef struct _Face
{
	unsigned int id[3];
	_Face(unsigned int id0 = 0, unsigned int id1 = 0, unsigned int id2 = 0)
	{id[0] = id0; id[1] = id1; id[2] = id2;}

	_Face(const unsigned int* _id)
	{id[0] = _id[0]; id[1] = _id[1]; id[2] = _id[2];}

	void set(unsigned int id0 = 0, unsigned int id1 = 0, unsigned int id2 = 0)
	{id[0] = id0; id[1] = id1; id[2] = id2;}

	void set(const unsigned int* _id)
	{id[0] = _id[0]; id[1] = _id[1]; id[2] = _id[2];}
} RxFace;

/************************************************************/
/*********** RxLight		*********************************/ 
/************************************************************/
enum RX_LIGHT_KIND
{
	RX_DIRECTIONAL_LIGHT,
	RX_PUNCTUAL_LIGHT
};

class RxLight
{
	public:
		glm::vec4 pos; //position (pos.w = RX_PUNCTUAL_LIGHT or pos.w = RX_DIRECTIONAL_LIGHT)
		glm::vec3 dir; //direction
		bool enable; //active
		unsigned int ID;

		glm::vec4 color;
		glm::vec4 diffuse;
		glm::vec4 ambient;
		glm::vec4 specular;
		glm::vec3 attenuation;
		
		bool IsPunctual();
		bool IsDirectional();

		float getTypeLight();


		RxLight();
		RxLight(glm::vec4 position, glm::vec4 direction = glm::vec4(), bool _enable = RX_ON);

};


/************************************************************/
/*********** RxMaterial			*****************************/ 
/************************************************************/

enum RX_FRAGMENT_ID
{
	RX_ID_FRAG_COLORMAP,
	RX_ID_FRAG_NORMALMAP,
	RX_ID_FRAG_HEIGHTMAP
};


class RxMaterial
{


public:

	//textures
	RxTexture colorMap;  //texture 
	RxTexture normalMap; //normal map pr le bump mapping ....
	RxTexture heightMap;

	GLuint idFragmentColorMap;	// par défaut GL_TEXTURE0 -> glActiveTexture(idFragmentColorMap);
	GLuint idFragmentNormalMap; // par défaut GL_TEXTURE1 -> glActiveTexture(idFragmentNormalMap);
	GLuint idFragmentHeightMap; // par défaut GL_TEXTURE2 -> glActiveTexture(idFragmentNormalMap);

	//surfaces
	glm::vec4 color; 
	glm::vec4 diffuse;
	glm::vec4 ambiant;
	glm::vec4 specular;
	float shininess;

	RxMaterial();

	void setColorMap(const char *filename);
	void setNormalMap(const char *filename);
	void setHeighMap(const char *filename);

	void setColorMap(GLuint id);
	void setNormalMap(GLuint id);
	void setHeighMap(GLuint id);

	void setColorMap(RxTexture &texture);
	void setNormalMap(RxTexture &texture);
	void setHeighMap(RxTexture &texture);


	void Bind();

};

class RxBoundingBox
{
	public:
		glm::vec3 size;
		glm::vec3 position;
		RxBoundingBox();
		bool IsValide();
		//tab : taille egale à 8
		void Compute8Points(glm::vec3 *tab);
};

/************************************************************/
/*********** les classes d'objet 3D simple *************************/ 
/************************************************************/

class RxItem3D //classe de base
{
protected:
	unsigned int type; //definie le type de l'objet

	static RxMaterial defautMaterial;  //texture par defaut (index dans tabIdtex) (initialiser à 0)	
	static RxShader *defautShader;  //texture par defaut (index dans tabIdtex) (initialiser à 0)


	RxShader *shader;		   //shader de l'objet
	RxMaterial material;      //materiel de l'objet

	std::string transform_uniform_name;

	//boite englobante de l'objet.
	RxBoundingBox boundingBox;
	
	glm::mat4 *projection, *modelview;
	glm::mat4 transform;

public:
	
	RxItem3D();
	virtual ~RxItem3D();

	virtual	void Draw() = 0;  //dessine l'Item 3D dans sa globalité
	virtual void Draw(glm::vec3 pos, glm::vec3 dir); //dessine l'Item 3d en fonction de la position et direction de la camera
	virtual unsigned int getType();
	virtual bool IsValide();

	virtual void setShader(RxShader *s);
	virtual RxShader* getShader();
	virtual void setMaterial(RxMaterial &m);
	virtual void setTexture(RxTexture &t);
	virtual void setColor(glm::vec4 &color);
	virtual RxMaterial &getMaterial();

	static void setDefaultMaterial(RxMaterial& defaultm);

	void setMatrix(glm::mat4* projection, glm::mat4* modelview);
	void setProjection(glm::mat4* projection);
	void setModelView(glm::mat4* modelview);
	void setTransform(glm::mat4 &transform);
	glm::mat4 *getProjection();
	glm::mat4 *getModelView();
	glm::mat4& getTransform();

	void setUniformNameTransform(const char*name);
	std::string& RxItem3D::getUniformNameTransform();
	//fonction bouding box
	virtual void setBoundingBox(RxBoundingBox  box);
	virtual void setBoundingBox(glm::vec3 size,glm::vec3 position);
	virtual void ComputeBoundingBox();
	virtual RxBoundingBox &getBoundingBox();


};


#endif