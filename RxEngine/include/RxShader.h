//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once
#ifndef _RXSHADER_H_
#define _RXSHADER_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif


#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "RxLogger.h"
#include "RxMath3D.h"
#include "RxConst.h"
#include "RxTools.h"
#include <glm/gtc/type_ptr.hpp>

//Attribution par defaut
#define RX_VERTEX_ATTRIB_INDEX 0
#define RX_COLOR_ATTRIB_INDEX  1
#define RX_COORD0_ATTRIB_INDEX 2
#define RX_NORMAL_ATTRIB_INDEX 3
#define RX_COORD1_ATTRIB_INDEX 4
#define RX_COORD2_ATTRIB_INDEX 5
#define RX_DATA0_ATTRIB_INDEX  6
#define RX_DATA1_ATTRIB_INDEX  7
#define RX_DATA2_ATTRIB_INDEX  8

#define RX_VERTEX_ATTRIB_NAME  "in_Vertex"
#define RX_COLOR_ATTRIB_NAME   "in_Color"
#define RX_COORD0_ATTRIB_NAME  "in_Coord0"
#define RX_NORMAL_ATTRIB_NAME  "in_Normal"
#define RX_COORD1_ATTRIB_NAME  "in_Coord1"
#define RX_COORD2_ATTRIB_NAME  "in_Coord2"
#define RX_DATA0_ATTRIB_NAME   "in_Data0"
#define RX_DATA1_ATTRIB_NAME   "in_Data1"
#define RX_DATA2_ATTRIB_NAME   "in_Data2"



#define RX_NUMDEFAULT_UNIFORM 3

#define RX_PROJECTION_MATRIX "projection"
#define RX_MODELVIEW_MATRIX "modelview"
#define RX_TRANSFORM_MATRIX "transform"

#define RX_PROJECTION_MATRIX_ID 0
#define RX_MODELVIEW_MATRIX_ID 1
#define RX_TRANSFORM_MATRIX_ID 2



class RxShader
{
	public:
	//attribut
	typedef struct attrib_struct
	{
		GLuint index;
		std::string name;
	} RxAttrib;

	static RxShader *bindShader;

	unsigned int defaultUniform[RX_NUMDEFAULT_UNIFORM];
	//std::map<std::string, unsigned int> mapUniforms;

	public:

		RxShader();

		//chargement du vertex shader et fragment
		RxShader(const char *vertexSource, const char *fragmentSource);
		~RxShader();
	
		//chargement du vertex shader et fragment
		int Load(const char *vertexSource, const char *fragmentSource);
		
		//Activation shader
		void Enable();
		
		//Desactivation shader
		void Disable();


		//attribut name			-> index 
		void BindAttribLocation(GLuint index, const GLchar* name);

		//attribution -> voir #define
		virtual void BindAttribLocationDefault();
		

		virtual void BindUniformDefault();

		//retour l'info sur programID
		bool IsValid();

		//retour la référence sur error
		std::string& getError();

		GLuint getGlProgramID();

		


		//Fonction d'envoie uniform
		//-----------------------------------------------------------------------------
		// Matrice spécifique
		//----------------------------------------------------------------------------
		void SendProjection(const glm::mat4 & m);
		void SendModelView(const glm::mat4 & m);
		void SendTransform(const glm::mat4 & m);

		//-----------------------------------------------------------------------------
		// Send1i/Send1f
		//-----------------------------------------------------------------------------
		void Send1i(const GLchar* name,int a);
		void Send1f(const GLchar* name,float a);

		//-----------------------------------------------------------------------------
		// Send2i/Send2f
		//-----------------------------------------------------------------------------
		void Send2i(const GLchar* name,int a,int b);
		void Send2f(const GLchar* name,float a,float b);

		//-----------------------------------------------------------------------------
		// Send3i/Send3f
		//-----------------------------------------------------------------------------
		void Send3i(const GLchar* name,int a,int b,int c);
		void Send3f(const GLchar* name,float a,float b,float c);

		//-----------------------------------------------------------------------------
		// Send4i/Send4f
		//-----------------------------------------------------------------------------
		void Send4i(const GLchar* name,int a,int b,int c, int d);
		void Send4f(const GLchar* name,float a,float b,float c,float d);


		//-----------------------------------------------------------------------------
		// Sendmat4i/Sendmat4i
		//-----------------------------------------------------------------------------
		void SendMat4f(const GLchar* name, GLboolean transpose, const GLfloat *value);

	#ifdef _RXMATH3D_H_
		//-----------------------------------------------------------------------------
		// Send2i/Send2f
		//-----------------------------------------------------------------------------
		void Send2i(const GLchar* name,const glm::ivec2 &v);
		void Send2f(const GLchar* name,const glm::vec2 &v);

		//-----------------------------------------------------------------------------
		// Send3i/Send3f
		//-----------------------------------------------------------------------------
		void Send3i(const GLchar* name,const glm::ivec3 &v);
		void Send3f(const GLchar* name,const glm::vec3 &v);

		//-----------------------------------------------------------------------------
		// Send4i/Send4f
		//-----------------------------------------------------------------------------
		void Send4i(const GLchar* name,const glm::ivec4 &v);
		void Send4f(const GLchar* name,const glm::vec4 &v);

		//-----------------------------------------------------------------------------
		// SendMat4f
		//-----------------------------------------------------------------------------
		void SendMat4f(const GLchar* name, GLboolean transpose, const glm::mat4& m);
		void SendMat4f(const GLchar* name, GLboolean transpose, const std::vector<glm::mat4> & m);

	#endif


	private:

		//chargement du vertex shader ou du fragment shader
		int LoadTypeShader(GLuint &shader, GLenum type, const char *source);
		void Destroy();
		
		void BindAllAttribLocation();


		//erreur
		std::string error;


		//INFO GL
		GLuint programID;
        GLuint vertexID;
        GLuint fragmentID;

		std::vector<RxAttrib> tab_attribs;
};


#endif 
