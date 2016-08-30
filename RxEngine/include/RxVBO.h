//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :	RxVBO.h										   #
//#	  Description : 									       #
//#			Gestion des VBO (et VAO)					       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXVBO_H_
#define _RXVBO_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif


#include <vector>

#include "RxLogger.h"
#include "RxTools.h"
#include "RxMath3D.h"


#define RX_ATTRIBUT_VEC 0	// glVertexAttribPointer
#define RX_ATTRIBUT_IVEC 1	// glVertexAttribIPointer
//#define RX_ATTRIBUT_LVEC 2  // glVertexAttribLPointer -- Vraiment utile ? (non supporté)

class RxVBO
{
public:

	RxVBO();
	~RxVBO();

	void Destroy();
	bool Create(GLenum = GL_STATIC_DRAW);

	// Activation et désactivation du VBA
	void Enable();
	void Disable();
		
	bool IsValide(); //Retourne : ArrayBufferID || ElementBufferID

	GLuint getElementSize();
	GLuint getArrayBufferID();
	GLuint getElementBufferID();
	GLuint getVertexArraysID();

	//routines bufferData.
	//il est important que les accès mémoires à *data restent disponible jusqu'a l'appel de VBO::Create(...)
	void AddArrayBufferData(GLuint indexAttrib, const float* data, unsigned int dataSize, GLint vertexSize = 3); 
	void AddArrayBufferData(GLuint indexAttrib, const glm::vec2* data, unsigned int dataSize);
	void AddArrayBufferData(GLuint indexAttrib, const glm::vec3* data, unsigned int dataSize);
	void AddArrayBufferData(GLuint indexAttrib, const glm::vec4* data, unsigned int dataSize);

	//si size = 0 => size = data.size()
	void AddArrayBufferData(GLuint indexAttrib, std::vector<float> &data, unsigned int dataSize = 0, GLuint vertexSize = 3); 
	void AddArrayBufferData(GLuint indexAttrib, std::vector<glm::vec2> &data, unsigned int dataSize = 0);
	void AddArrayBufferData(GLuint indexAttrib, std::vector<glm::vec3> &data, unsigned int dataSize = 0);
	void AddArrayBufferData(GLuint indexAttrib, std::vector<glm::vec4> &data, unsigned int dataSize = 0);


	//routines bufferData.
	//il est important que les accès mémoires à *data restent disponible jusqu'a l'appel de VBO::Create(...)
	void AddArrayBufferData(GLuint indexAttrib, const int* data, unsigned int dataSize, GLint vertexSize = 3); 
	void AddArrayBufferData(GLuint indexAttrib, const glm::ivec2* data, unsigned int dataSize);
	void AddArrayBufferData(GLuint indexAttrib, const glm::ivec3* data, unsigned int dataSize);
	void AddArrayBufferData(GLuint indexAttrib, const glm::ivec4* data, unsigned int dataSize);

	//si size = 0 => size = data.size()
	void AddArrayBufferData(GLuint indexAttrib, std::vector<int> &data, unsigned int dataSize = 0, GLuint vertexSize = 3); 
	void AddArrayBufferData(GLuint indexAttrib, std::vector<glm::ivec2> &data, unsigned int dataSize = 0);
	void AddArrayBufferData(GLuint indexAttrib, std::vector<glm::ivec3> &data, unsigned int dataSize = 0);
	void AddArrayBufferData(GLuint indexAttrib, std::vector<glm::ivec4> &data, unsigned int dataSize = 0);

	void AddStructArrayBufferData(GLuint nbAttrib, GLuint *ptAttribIndex, GLenum *ptAttribType, GLuint *ptAttribSize, GLuint *ptAttribOffsetSize, GLuint stride, GLuint size, const void *data, unsigned int * ptAttribInType = NULL);
	void AddStructArrayBufferData(GLuint nbAttrib, std::vector<GLuint> &tAttribIndex, std::vector<GLenum> &tAttribType, std::vector<GLuint> &tAttribSize, std::vector<GLuint> &tAttribOffsetSize, GLuint stride, GLuint size, const void *data,  std::vector<GLuint> &tAttribInType = std::vector<GLuint>());

	void AddElementBufferData(const unsigned int* data, unsigned int dataSize);

private:

	GLuint arrayBufferID;   //VBO
	GLuint vertexArraysID;  //VAO fesant appelle au vbo
	GLuint elementBufferID; //

	

	typedef struct _ArrayBuffer
	{
		public:
		GLuint nbAttrib;		//nombre d'attribut
		std::vector<GLuint> tAttribIndex;	//index des Attributs  (glVertexAttribPointer)- tableau de la taille tAttribIndex[nb_Attrib]
		std::vector<GLenum> tAttribType;	//le type des attributs - tableau de la taille tAttribType[nb_Attrib] GL_INT, GL_FLOAT etc...
		std::vector<GLuint> tAttribSize;    //1,2,3,4 (vecteur a 1,2,3 ou 4 coordonnées)- tableau de la taille tAttribType[tAttribSize] 
		std::vector<GLuint> tAttribOffsetStart; //offset en octet entre chaque element de la structure - tableau de la taille tAttribOffsetSize[tAttribSize] 
		std::vector<unsigned int> tAttribInType; // RX_ATTRIBUT_VEC, RX_ATTRIBUT_IVEC, RX_ATTRIBUT_LVEC

		GLuint stride;
		GLuint size;			//taille en  bytes (octets)
		const void *data;		//pointeur sur les données  
		_ArrayBuffer():nbAttrib(0),size(0),data(0),stride(0)
		{
		}
	}ArrayBuffer;


	typedef struct _ElementBuffer
	{
		GLuint dataUIntSize;
		const unsigned int *data;
	} ElementBuffer;

	std::vector<ElementBuffer> tElementBuffer;
	std::vector<ArrayBuffer> tArrayBuffer;

};

#endif
