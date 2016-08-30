/*
 * The MIT License
 *
 * Copyright 2014 Robxley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _BH3D_FONT_H_
#define _BH3D_FONT_H_

#include "Shader.hpp"
#include <sstream>
#include "VBO.hpp"
#include "SDL_ttf.h"

//default font parameters
//---------------------------------------------
#define BH3D_FONT_FILE "data/default.ttf"
#define BH3D_FONT_SIZE 15
#define BH3D_FONT_BORDER 2

//default shader parameters to draw the font
//--------------------------------------------
#ifdef BH3D_USE_OPENGLES2
#define BH3D_FONT_SHADER_VERT "data/shaders/font_es.vert"
#define BH3D_FONT_SHADER_FRAG "data/shaders/font_es.frag"
#else
#define BH3D_FONT_SHADER_VERT "data/shaders/font.vert"
#define BH3D_FONT_SHADER_FRAG "data/shaders/font.frag"
#endif

//uniform names send to the font shader
#define BH3D_FONT_PROJ_UNIFORM "projection"	//projection matrice
#define BH3D_FONT_COLOR_UNIFORM "uColor"	//font color
#define BH3D_FONT_POS_UNIFORM "offset"		//screen offset position

//méthode pour construire la texture font
//----------------------------------------------------
//#define BH3D_BUILD_MAP_USING_GLTEXSUBIMAGE2D	//Généralement plus lents (très lent avec OPENGLES 2.0) 
//#define BH3D_BUILD_MAP_USING_SDLBLITSURFACE		//Par defaut Généralement plus rapide


namespace BH3D
{

	//Classe permettant l'affichage de texte à partir d'une texture ou du font (utilisée pour créer une texture)
	//Le texte est caractérisé comme étant statique (constant) ou dynamique
	//Le texte statique est envoyé en mémoire vidéo via VBA/VBO et ne peut être modifié qu'a un moment précis du code.
	//Le texte dynamique est affiché à la volé et peut être modifié n'importe quand (plus lent que le texte statique).
	//Les textes sont basées sur l'utilisation d'un objet std::ostringstream distinct
	//Le ostringstream du texte statique n'est pas directement accessible.
	class Font
	{
		public:

			Font();
			~Font();

			//Initialisation
			//----------------------------------------------------------------------------------------

			// Génére une texture à partir d'une font (Chargement plus lent surtout sur platform mobile)
			//Create a texture from a font
			GLuint UseFont(const char * filename = BH3D_FONT_FILE, int _size = BH3D_FONT_SIZE, int _border = BH3D_FONT_BORDER, Shader *_pShader = nullptr);
			
			//Utilise une texture comme font
			//Use a OpenGL texture as a font
			void UseTexture(GLuint glid, int size = BH3D_FONT_SIZE, Shader *_pShader = nullptr, float lineSkipFactor = 1.10f);

			//Détruit l'objet (un appelle a UseFont , ou UseTexture est à nouveau nécessaire)
			//Destroy the object
			void Destroy();

			//Gestion du texte "statique"
			//-------------------------------------------------------------------------------------------

			//Ajout du texte statique et retourne son identifiant par un entier
			//Le texte ajouté n'est valide qu'après l'appel de la fonction ComputeStaticText()
			//La fonction retourne un identifiant sous forme d'entier permetant d'afficher par la suite uniquement le texte concerné
			//Add a static text. This text can be only use if ComputeStaticText function was called.
			//Return the id of added text
			unsigned int AddStaticText(const std::string staticText);

			//Création du VBA/VBO correspondant aux textes statics ajoutés via AddStaticText
			//Create the VBA/VBO
			bool ComputeStaticText();

			//Détruit le texte statique
			//Clear all static text
			void ClearStaticText();

			//afficher une partie (ou l'ensemble) des textes statiques à la position x , y
			//si end = 0 affiche l'ensemble du texte statique contenu dans std::ostringstream
			//Display the static text at the position (x,y) contained in std::ostringstream in range [start, end]
			void PrintStaticTexts(int x, int y, unsigned int start , unsigned int end );

			//affiche à la position x, y, le texte avec l'identifié id. L'identifiant correspond à la valeur retournée par AddStaticText
			//display the static text at the position (x,y) by id. id is given by AddStaticText
			void PrintStaticTexts(int x, int y, unsigned int id);

			//Gestion du texte "dynamique"
			//-------------------------------------------------------------------------------------------

			//Affichage d'un texte quelconque à la position 0,0
			//Print a text
			void PrintText(const std::string & text);

			//Affichage du texte quelconque à la position x,y
			//Print a text at the position (x,y)
			void PrintText(int x, int y, const std::string & text);

			//Affichage du texte contenu dans dynamicTextStream (accessible via GetDynamicTextStream)
			//Print all texts contain std::ostringstream dynamicTextStream
			inline void PrintTextStream();

			//Affichage à la position (x, y) du texte contenu dans dynamicTextStream (accessible via GetDynamicTextStream)
			// Print all texts contain std::ostringstream dynamicTextStream at the position (x,y)
			inline void PrintTextStream(int x, int y);


			//getter / setter
			inline int GetLineSkip(void) const;
			inline void SetTextColor(const glm::vec4  &color);
			int LengthText(const std::string & text) const;
			int LengthStaticText(unsigned int id) const;
			inline GLuint IsValid() const;
			inline GLuint GetGLTexture() const;
			inline void SetGLtexture(GLuint id);

			inline const std::ostringstream & GetStaticTextStream() const;
			inline std::ostringstream & GetDynamicTextStream();

		private :

			// Internal functions
			void PrintTextOneLine(const std::string & text, std::size_t start, std::size_t end);

			void GetGlyphMetrics(TTF_Font *font);

			void BuildFontMap(TTF_Font *font);

			void BuildVBO();

			void LoadShader(Shader *pShader);

		private:

			//Text streams variables
			std::ostringstream dynamicTextStream;
			std::ostringstream staticTextStream;

			//VBO
			VBO dynamicTextVbo;
			VBO staticTextVbo;

			//Info sur AddStaticText
			unsigned int staticTextElementNumber = 0; //Number of texts added with AddStaticText
			
			//Simple struct for the begin/end of each part of text save in staticTextStream
			struct startend{
				std::size_t start, end;
			};

			//Position start/end of each part of text in std::ostringstream staticTextStream added with AddStaticText
			//the index in vector matches with the id returned by AddStaticText
			std::vector<startend> tStaticTextOffsets; 

			//text color
			glm::vec4 textColor{ 1.0f, 1.0f, 1.0, 1.0 };

			//Shader use to display the text
			Shader *pShader = nullptr;
			bool myShader = 0;

			int textSize = 12;			//Text size
			GLuint glFontTexture = 0;	//OpenGL texture id
			bool myTexture = 1;			// class texture or not ?
			
			unsigned int texWidth = 0;
			unsigned int texHeight = 0;

			// Glyph metrics advance parameter
			// see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_38.html#SEC38
			int maxHeight = 0;
			int fontAscent = 0;
			int fontDescent = 0;
			int lineSkip = 0;
			int border = 0;

			std::vector<int> tAdvance; // for each glyph
			int maxAdvance = 0; // store the max

			//offset on the screen (position of the text)
			int offsetx, offsety;

	};

	
	inline void Font::PrintTextStream()
	{ 
		PrintText(dynamicTextStream.str());
	}

	inline void Font::PrintTextStream(int x, int y)
	{ 
		PrintText(x, y, dynamicTextStream.str());
	}

	inline const std::ostringstream & Font::GetStaticTextStream() const
	{ 
		return staticTextStream;
	};

	inline std::ostringstream & Font::GetDynamicTextStream()
	{ 
		return dynamicTextStream;
	};

	inline GLuint Font::IsValid() const
	{
		return glFontTexture && pShader;
	}

	inline void Font::SetTextColor(const glm::vec4 & color)
	{
		textColor = color;
	}

	inline int Font::GetLineSkip(void) const
	{
		return lineSkip;
	}

	inline GLuint Font::GetGLTexture() const
	{
		return glFontTexture;
	}
	inline void Font::SetGLtexture(GLuint id)
	{
		if (glFontTexture)
			glDeleteTextures(1, &glFontTexture);

		glFontTexture = id;
	}
	inline void Font::PrintStaticTexts(int x, int y, unsigned int id)
	{
		BH3D_ASSERT_MSG(id < tStaticTextOffsets.size(), "id out of range", );
		PrintStaticTexts(x, y, (unsigned int) tStaticTextOffsets[id].start, (unsigned int) tStaticTextOffsets[id].end);
	}

	inline void Font::ClearStaticText()
	{
		tStaticTextOffsets.clear();
		staticTextVbo.Destroy();
	}

	inline void Font::PrintText(int x, int y, const std::string & text)
	{
		offsetx = x;
		offsety = y;

		// Print it
		PrintText(text);

		offsetx = 0;
		offsety = 0;

	}
}
#endif //
