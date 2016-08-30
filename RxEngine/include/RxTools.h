//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once
#ifndef _RXTOOL_H_
#define _RXTOOL_H_



#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <sdl2/sdl.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "RxConst.h"
#include "RxVBO.h"
#include "RxLogger.h"
#include "RxMath3D.h"

/************************** graphic tools *******************************/




void Rx_Repere3D();// axe x : rouge, axe y = vert, axe z = bleu;
void Rx_Repere2D();
void Rx_CubeTest();


#ifdef _DEBUG
	GLenum RxCheckGLError(const char* file, int line, const char *fct);
	#define RXCHECKGLERROR(fct) RxCheckGLError(__FILE__,__LINE__, fct)
	#define RXLOGGERGLERROR(msg) RxLogger::Instance().getFileLogger()<<"<GLERREUR> -- FILE <"<<line<<"> -- LINE <"<<file<<"> -- "<<fct<<" -- "<<msg<<std::endl
#else
	GLenum RxCheckGLError();
#endif
	

void RxSetSDLPixel(SDL_Surface *surface, int x, int y, Uint32 color);
Uint32 RxGetSDLPixel(SDL_Surface *surface, int x, int y);


/************************** conversion tools ****************************/
//fonctions de conversion
struct idXML2idGL
{
	 int idxml;
	 int idgl;
	 idXML2idGL(){idxml=idgl=0; }
	bool operator==(unsigned int idxml2)
	{return (idxml == idxml2);}
	
	bool operator==(int idxml2)
	{return (idxml == idxml2);}

};
glm::vec3 str2vec2(const char *svec);
glm::vec2 str2vec2(std::string &svec);
glm::vec3 str2vec3(const char *svec);
glm::vec3 str2vec3(std::string &svec);
glm::ivec2 str2ivec2(std::string &svec);
glm::ivec2 str2ivec2(const char *svec);

glm::vec4 str2vec4(const char *svec);
glm::vec4 str2vec4(std::string &svec);

//glm::quat str2quat(std::string squat);
//glm::quat str2quat(const char* squat);

std::vector<unsigned int> str2vui(const char* svui);
std::vector<unsigned int> str2vui(std::string &svui);
std::vector<unsigned int> xml2gl(std::vector<unsigned int> idxml, std::vector<idXML2idGL> switchtab);
unsigned int xml2gl(unsigned int idxml, std::vector<idXML2idGL> switchtab);
std::vector<glm::vec3> str2vectorvec3(const char * str);
std::vector<glm::vec3> str2vectorvec3(std::string &svvec);

std::vector<float> str2vectorfloat(const char * str);
std::vector<float> str2vectorfloat(std::string &svvec);

std::vector<int> str2vectorint(std::string &sint);

#endif
