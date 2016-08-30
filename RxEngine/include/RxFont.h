//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


/////////////////////////////////////////////////////////////////////////////
//
// 
// 
//
// 
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _RXFONT_H_
#define _RXFONT_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <cstdarg>
#include <RxVBO.h>
#include <RxShader.h>
#include "RxLogger.h"
#include "RxMath3D.h"


#define RX_VERT_SHADER_FONT "shaders/font.vert"
#define RX_FRAG_SHADER_FONT "shaders/font.frag"
//#define RX_DEFAULT_TEXTURE NULL
#define RX_DEFAULT_TEXTURE "defaut.ttf"

/////////////////////////////////////////////////////////////////////////////
//
// class RxFont - An OpenGL True Type Font object.
//
/////////////////////////////////////////////////////////////////////////////

class RxFont
{
 public:
	// Constructor/destructor
	RxFont();
	~RxFont( void );

	GLuint GenerateGlTexture(const char *filename = "defaut.ttf", int _size = 12, int _border = 2, RxShader *_pShader = NULL );

	GLuint isValide();

	// Public interface
	void PrintText( const char *format, ... );
	void PrintText( int x, int y, const char *format, ... );
	int getLineSkip( void ) const {	return lineSkip;  }

	void setTextColor(glm::vec4 color);

	// Fonction qui renvoit la longueur/hauteur du texte affiché avec la font
	int LengthText(const char* format,...);

	GLuint getTexFontId();


  

 private:
	// Internal functions
	void PrintTextOneLine( const char *text, int length);

	void getGlyphMetrics( TTF_Font *font );
	void BuildFontMap( TTF_Font *font );
	void BuildVBO( void );

 private:

	RxVBO vbo;
	RxShader *pShader;

	glm::vec4 textColor;


	bool myShader;

	std::string fontPath;
	int size;
	// Member variables
	GLuint textureFontId;

	TTF_Font *font;

	int texWidth;
	int texHeight;

	int maxHeight;
	int fontAscent;
	int fontDescent;
	int lineSkip;
	int border;

	// Glyph metrics advance parameter
	std::vector<int> tAdvance; // for each glyph
	int maxAdvance; // store the max

	// TrueTypeFont counter
	static int ttfCount;


	std::vector<glm::vec2> tVertex2d;
	std::vector<glm::vec2> tTexCoord2;

	std::vector<unsigned int> tIndices;

	int offsetx, offsety;
};

#endif // __FONT_H__
