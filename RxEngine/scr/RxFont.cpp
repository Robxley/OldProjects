//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

/* -*- c++ -*- */
/////////////////////////////////////////////////////////////////////////////
//
// Font.cpp -- by David Henry
// last modification: june. 20, 2005
//
// Implementation of an OpenGL True Type Font class.
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "RxFont.h"





/////////////////////////////////////////////////////////////////////////////
//
// class RxFont implementation.
//
/////////////////////////////////////////////////////////////////////////////

int RxFont::ttfCount = 0;


// --------------------------------------------------------------------------
// RxFont::RxFont
//
// Constructor.  Build a texture font from a TTF and generate display
// lists for each character of the Latin1 ASCII table.
// --------------------------------------------------------------------------

RxFont::RxFont()
  : textureFontId(0), texWidth(0), texHeight(0),
	maxHeight(0), fontAscent(0), fontDescent(0), lineSkip(0),
	border(0), maxAdvance(0),size(12),myShader(0),pShader(0),
	textColor(1.0f,1.0f,1.0f,1.0f),font(0)
{

	
}
GLuint RxFont::getTexFontId(){ return textureFontId;}

GLuint RxFont::isValide()
{
	return textureFontId && pShader;
}

GLuint RxFont::GenerateGlTexture( const char *filename, int _size, int _border, RxShader *_pShader)
{
	fontPath = filename;
	border = _border;
	size = _size;
	if( !TTF_WasInit() && TTF_Init() == -1 )
	{
		RXLOGGERERROR("[RxFont][GenerateGlTexture] TTF_Init failed: "<< TTF_GetError());
		return 0;
	}
	else
	{
		ttfCount++;
	}
	font = TTF_OpenFont( fontPath.c_str(), _size );
	// TTF_Font *font = TTF_OpenFont( fontPath.c_str(), size );
	if( !font )
	{
		RXLOGGERERROR("[RxFont][GenerateGlTexture] TTF_OpenFont: " << TTF_GetError());
		return 0;
	}
	else 
	{
		getGlyphMetrics( font );

		BuildFontMap( font );

		BuildVBO();


	/** if(TTF_FontFaceIsFixedWidth(font))
		cerr <<"Taille des caractères fixe.\n";
	else
		cerr <<"Taille des caractères différents.\n";
	**/
	RXLOGGER("[RxFont][GenerateGlTexture] Loaded Font :"<<fontPath);
	}

	if(_pShader == NULL)
	{
		pShader = new RxShader(RX_VERT_SHADER_FONT,RX_FRAG_SHADER_FONT);
		myShader = true;
	}
	else
		pShader = _pShader;


  return textureFontId;
}


// --------------------------------------------------------------------------
// RxFont::~RxFont
//
// Destructor.  Delete texture and display lists.
// --------------------------------------------------------------------------


RxFont::~RxFont( void )
{
	TTF_CloseFont( font );
	if( glIsTexture( textureFontId ) 	)
		glDeleteTextures( 1, &textureFontId );

	ttfCount--;

	if( !ttfCount )
		TTF_Quit();

    if(myShader && pShader)
		delete pShader;
	pShader = NULL;
	myShader = false;
	vbo.Destroy();
}


// --------------------------------------------------------------------------
// RxFont::PrintText
//
// Print multiline formated text.
// --------------------------------------------------------------------------

void RxFont::PrintText( const char *format, ... )
{

	int saveoffsety = offsety;

  char buffer[1024];
  std::va_list argument;

  // Format the text
  va_start( argument, format );
    std::vsprintf( buffer, format, argument );
  va_end( argument );

 
  

    //glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBindTexture( GL_TEXTURE_2D, textureFontId );

	char *ptr = buffer;

	// Print each line separately
	do {
	  int len;
	  char *end = strchr( ptr, '\n' );

	  if( !end ) {
		len = strlen( ptr );
	  }
	  else {
		end++;
		len = end - ptr - 1;
	  }

	  
	    // Print the current line
	    PrintTextOneLine( ptr, len );
		offsety -=lineSkip;
	  ptr = end;

	} while( ptr );

  offsety = saveoffsety;
}


void RxFont::PrintText( int x, int y, const char *format, ... )
{
	offsetx = x;
	offsety = y;
  // Format the text
  char buffer[1024];
  std::va_list argument;

  va_start( argument, format );
    std::vsprintf( buffer, format, argument );
  va_end( argument );

  // Print it
  PrintText( buffer );

  	offsetx = 0;
	offsety = 0;
 
}


// --------------------------------------------------------------------------
// RxFont::PrintTextOneLine
//
// Print a single line text.
// --------------------------------------------------------------------------

void RxFont::PrintTextOneLine( const char *text, int length )
{
	if(pShader == NULL) return;
	//ptrshader->Debug();

	glBindTexture(GL_TEXTURE_2D, textureFontId);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glm::mat4 projection = glm::ortho(0.0f, (float) viewport[2]-viewport[0] ,0.0f, (float)viewport[3]-viewport[1]);

	pShader->Enable();		
	pShader->SendMat4f("projection",GL_FALSE,projection);
	//pShader->Send1i("textureSampler", 0);
	pShader->Send4f("uColor",textColor);
	vbo.Enable();
	for(int i = 0, offset = offsetx; i < length; i++)
	{

		pShader->Send2i("offset",offset,offsety);
		unsigned int ptroffset = (unsigned int)6*text[i];
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(unsigned int*)NULL+ptroffset );
		offset+=tAdvance[text[i]]+border;
	}
	vbo.Disable();
	pShader->Disable();

}


// --------------------------------------------------------------------------
// RxFont::getGlyphMetrics
//
// --------------------------------------------------------------------------

void RxFont::getGlyphMetrics( TTF_Font *font )
{
  tAdvance.reserve( 256 );
  maxAdvance = 0;

  for( int i = 0; i < 256; ++i ) {
	int dummy, advance;
	TTF_GlyphMetrics( font, static_cast<Uint16>(i), &dummy, &dummy,
					  &dummy, &dummy, &advance );
	tAdvance.push_back( advance );

	if( advance > maxAdvance )
	  maxAdvance = advance;
  }

  // fontHeight == fontAscent - fontDescent + 1
  maxHeight = TTF_FontHeight( font );
  fontDescent = TTF_FontDescent( font ) - 1;
  fontAscent = TTF_FontAscent( font );
  lineSkip = TTF_FontLineSkip( font );
}


// --------------------------------------------------------------------------
// RxFont::BuildFontMap
//
// --------------------------------------------------------------------------

void RxFont::BuildFontMap( TTF_Font *font )
{
	texWidth = (maxAdvance + border) * 16 + border;
	texHeight = (maxHeight + border) * 16 + border;

	// Create a black texture with transparent alpha
	std::vector<GLubyte> texels(texWidth * texHeight * 4 ,0);

	glGenTextures( 1, &textureFontId );
	glBindTexture( GL_TEXTURE_2D, textureFontId );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, &texels[0] );

	int xoffset = border;
	int yoffset = border;

	// Now "insert" each character in the texture
	for( int i = 0; i < 16; ++i )
	{
		for( int j = 0; j < 16; ++j ) 
		{
			char letter[2];
			letter[0] = static_cast<char>(i * 16 + j);
			letter[1] = '\0';

			if( letter[0] != 0 )
			{
				// White with full alpha
				SDL_Color foreground = { 255, 255, 255, 255 };
				SDL_Surface *sLetter = TTF_RenderText_Blended( font, letter, foreground );

				if( sLetter ) 
				{
					glTexSubImage2D( GL_TEXTURE_2D, 0, xoffset, yoffset, sLetter->w,
								sLetter->h, GL_RGBA, GL_UNSIGNED_BYTE, sLetter->pixels );

					SDL_FreeSurface( sLetter );
				}
			}

			xoffset += maxAdvance + border;
		}

		xoffset = border;
		yoffset += maxHeight + border;
	}


	glBindTexture(GL_TEXTURE_2D,0);



}


// --------------------------------------------------------------------------
// RxFont::buildDisplayLists
//
// Build a display list for each character.  A character is represented
// by a textured quad + a translation of its "width" about the X-axis.
// --------------------------------------------------------------------------




void RxFont::BuildVBO( void )
{
	

	vbo.Destroy();

	GLfloat oneOverTexWidth = 1.0f / static_cast<GLfloat>(texWidth);
	GLfloat oneOverTexHeight = 1.0f / static_cast<GLfloat>(texHeight);

	GLfloat s = border * oneOverTexWidth;
	GLfloat t = border * oneOverTexHeight;

	GLfloat toffset = fontAscent * oneOverTexHeight;

	tVertex2d.clear();
    tTexCoord2.clear();

	tIndices.clear();




	//float debug_offset = 0;

	for( int i = 0, c = 0; i < 16; ++i )
	{
		for( int j = 0; j < 16; ++j, c++ )
		{
			
	 
			unsigned int id = tVertex2d.size();
			
			tIndices.push_back(id);
			tIndices.push_back(id+1);
			tIndices.push_back(id+2);
			
			

			tIndices.push_back(id);
			tIndices.push_back(id+2);
			tIndices.push_back(id+3);
			
		

			GLfloat soffset = tAdvance[c] * oneOverTexWidth;

			tTexCoord2.push_back(glm::vec2( s, t + toffset - (fontDescent * oneOverTexHeight) ) );
			tVertex2d.push_back( glm::vec2( 0, fontDescent ));

			tTexCoord2.push_back(glm::vec2( s + soffset, t + toffset - (fontDescent * oneOverTexHeight) ));
			tVertex2d.push_back( glm::vec2( tAdvance[c], fontDescent));

			tTexCoord2.push_back(glm::vec2( s + soffset, t ));
			tVertex2d.push_back( glm::vec2( tAdvance[c], fontAscent));

			tTexCoord2.push_back(glm::vec2( s, t ));
			tVertex2d.push_back( glm::vec2( 0, fontAscent));
			s += (maxAdvance + border) * oneOverTexWidth;


			//debug_offset+=static_cast<GLfloat>(_advance[c]);

		}
		s = border * oneOverTexWidth;
		t += (maxHeight + border) * oneOverTexHeight;
	}

	vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,tVertex2d);
	vbo.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX, tTexCoord2);
	vbo.AddElementBufferData(&tIndices[0],tIndices.size());
	if(!vbo.Create())
	{
		RXLOGGERERROR("[RxFont][BuildVBO] VBO Fail");
	}
}

int RxFont::LengthText(const char* format,...)
{
    va_list liste;
    va_start(liste,format);

    int w,h;
	//int len;
	//len = strlen(format);
   
	char texte[512];
    vsprintf(texte,format,liste);

	if (TTF_SizeText(font,texte,&w,&h))
		w = -1;

	return w;


  /*  if (TTF_SizeText(font,texte,&w,&h) == 0)
	{
		delete[] texte;
        return w;
	}
	else
	{
		delete[] texte;
        return -1;
	}*/
}

void  RxFont::setTextColor(glm::vec4 color)
{
	textColor = color;
}