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

#include "Font.hpp"
#include "Logger.hpp"
#include "Assertion.hpp"
#include "Common.hpp"
#include <glm/gtx/transform.hpp>

namespace BH3D
{

	Font::Font() :
		border(BH3D_FONT_BORDER), textSize(BH3D_FONT_SIZE), pShader(nullptr),
		glFontTexture(0), texWidth(0), texHeight(0),
		maxHeight(0), fontAscent(0), fontDescent(0), lineSkip(0),
		maxAdvance(0), myShader(0),
		textColor(1.0f, 1.0f, 1.0f, 1.0f), myTexture(1)
	{

		
	}
	void Font::Destroy()
	{
		if (glFontTexture && myTexture)
			glDeleteTextures(1, &glFontTexture);
		glFontTexture = 0;
		myTexture = 1;

		if (myShader && pShader)
			delete pShader;
		pShader = nullptr;
		myShader = false;
		dynamicTextVbo.Destroy();
		staticTextVbo.Destroy();

		unsigned int elements = 0;

		texWidth = 0;
		texHeight = 0;

		maxHeight = 0;
		fontAscent = 0;
		fontDescent = 0;
		lineSkip = 0;
		border = 0;

		tAdvance.clear(); 
		maxAdvance = 0;

	}
	Font::~Font()
	{
		Destroy();
	}
	GLuint Font::UseFont(const char * filename, int _size, int _border, Shader *_pShader)
	{
		if (IsValid())
			Destroy();

		border = _border;
		textSize = _size;

		if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
		{
			BH3D_LOGGERERROR(" TTF_Init error: SDL have to be Init before use TTF");
			return 0;
		}

		if (TTF_Init() == -1)
		{
			BH3D_LOGGERERROR(" TTF_Init error: " << TTF_GetError());
			return 0;
		}

		TTF_Font * font = TTF_OpenFont(filename, _size);


		if (!font)
		{
			BH3D_LOGGERERROR(" TTF_OpenFont error : " << TTF_GetError());
			TTF_Quit();
			return 0;
		}


		GetGlyphMetrics(font);

		if (myTexture)
		{
			BuildFontMap(font);
		}

		BuildVBO();

		LoadShader(_pShader);

		TTF_CloseFont(font);
		TTF_Quit();

		

		return glFontTexture;
	}

	void Font::UseTexture(GLuint glid,  int size, Shader * _pShader, float lineSkipFactor)
	{

		if (glid == 0) return;
		if (IsValid()) Destroy();

		glFontTexture = glid;
		myTexture = 0;

		texWidth = size * 16;
		texHeight = size * 16;

		tAdvance.resize(256, size);
		maxAdvance = size;
		border = 0;
		maxHeight = size;

		fontDescent = 0;
		fontAscent = size;
		lineSkip = (int)(size*lineSkipFactor);

		BuildVBO();

		LoadShader(_pShader);


	}

	void Font::LoadShader(Shader *_pShader)
	{
		if (_pShader == nullptr)
		{
			if (pShader != nullptr && myShader) delete pShader;
			pShader = new Shader(BH3D_FONT_SHADER_VERT, BH3D_FONT_SHADER_FRAG);
			myShader = true;
		}
		else
		{
			pShader = _pShader;
			myShader = false;
		}
	}

	void Font::PrintText(const std::string & text)
	{
		BH3D_ASSERT(pShader != nullptr)
		if (pShader == nullptr || text.empty()) 
			return;

		int saveoffsety = offsety;

		glBindTexture(GL_TEXTURE_2D, glFontTexture);
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glm::mat4 projection = glm::ortho(0.0f, (float)viewport[2] - viewport[0], 0.0f, (float)viewport[3] - viewport[1]);

		pShader->Enable();
		pShader->SendMat4f(BH3D_FONT_PROJ_UNIFORM, GL_FALSE, projection);
		pShader->Send4f(BH3D_FONT_COLOR_UNIFORM, textColor);
		dynamicTextVbo.Enable();

		std::size_t start = 0, end;
		do
		{
			//find the first '\n'
			end = text.find('\n', start);

			if (end == std::string::npos)
				end = text.size();

			PrintTextOneLine(text, start, end);
			offsety -= lineSkip;
			start = end + 1;

		} while (end < text.size());

		//dynamicTextVbo.Disable();
		//pShader->Disable();
		offsety = saveoffsety;
	}

	void Font::PrintTextOneLine(const std::string & text, std::size_t start, std::size_t end)
	{
		int offset = offsetx;
		for (std::size_t i = start; i < end; i++)
		{

			pShader->Send2i(BH3D_FONT_POS_UNIFORM, offset, offsety);
			unsigned int ptroffset = (unsigned int)6 * text[i];
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (unsigned int*)nullptr + ptroffset);
			offset += tAdvance[text[i]] + border;
		}

	}


	void Font::GetGlyphMetrics(TTF_Font *font)
	{

		// see https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_38.html#SEC38
		tAdvance.reserve(256);
		maxAdvance = 0;

		for (int i = 0; i < 256; ++i) {
			int dummy, advance;
			TTF_GlyphMetrics(font, static_cast<Uint16>(i), &dummy, &dummy,
				&dummy, &dummy, &advance);
			tAdvance.push_back(advance);

			if (advance > maxAdvance)
				maxAdvance = advance;
		}


		maxHeight = TTF_FontHeight(font);
		fontDescent = TTF_FontDescent(font) - 1;
		fontAscent = TTF_FontAscent(font);
		lineSkip = TTF_FontLineSkip(font);
	}

#ifdef BH3D_BUILD_MAP_USING_GLTEXSUBIMAGE2D

	//Methode glTexSubImage2D
	void Font::BuildFontMap(TTF_Font *font)
	{
		texWidth = (maxAdvance + border) * 16 + border;
		texHeight = (maxHeight + border) * 16 + border;

		// Create a black texture with transparent alpha
		std::vector<GLubyte> texels(texWidth * texHeight * 4, 0);

		glGenTextures(1, &glFontTexture);
		glBindTexture(GL_TEXTURE_2D, glFontTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// clamp to edge (Necessaire pour opengles 2.0 pour des images non 2^n)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, &texels[0]);

		int xoffset = border;
		int yoffset = border;

		// Now "insert" each character in the texture
		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < 16; ++j)
			{
				char letter[2];
				letter[0] = static_cast<char>(i * 16 + j);
				letter[1] = '\0';

				if (letter[0] != 0)
				{
					// White with full alpha
					SDL_Color foreground = { 255, 255, 255, 255 };
					SDL_Surface *sLetter = TTF_RenderText_Blended(font, letter, foreground);
					//SDL_Surface *sLetter = TTF_RenderText_Solid(font, letter, foreground);
					if (sLetter)
					{
						glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, sLetter->w,
							sLetter->h, GL_RGBA, GL_UNSIGNED_BYTE, sLetter->pixels);

						SDL_FreeSurface(sLetter);
					}
				}

				xoffset += maxAdvance + border;
			}

			xoffset = border;
			yoffset += maxHeight + border;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

	}


#else
	//Methode SDL_BlitSurfce
	void Font::BuildFontMap(TTF_Font *font)
	{
		texWidth = (maxAdvance + border) * 16 + border;
		texHeight = (maxHeight + border) * 16 + border;


		SDL_Surface *surface;
		Uint32 rmask, gmask, bmask, amask;

		/* SDL interprets each pixel as a 32-bit number, so our masks must depend
		on the endianness (byte order) of the machine */
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
		#else
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
		#endif

		surface = SDL_CreateRGBSurface(0, texWidth, texHeight, 32, rmask, gmask, bmask, amask);
		if (surface == nullptr) {
			BH3D_ASSERT_MSG(surface, "RGB surface fail", );
			return;
		}

		int xoffset = border;
		int yoffset = border;

		SDL_Rect rect;

		// Now "insert" each character in the texture
		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < 16; ++j)
			{
				char letter[2];
				letter[0] = static_cast<char>(i * 16 + j);
				letter[1] = '\0';

				if (letter[0] != 0)
				{
					// White with full alpha
					SDL_Color foreground = { 255, 255, 255, 255 };
					SDL_Surface *sLetter = TTF_RenderText_Blended(font, letter, foreground);
					//SDL_Surface *sLetter = TTF_RenderText_Solid(font, letter, foreground);
					BH3D_ASSERT_MSG(sLetter, "TTF_RenderText_Blended Fail", );
					if (sLetter)
					{
						rect.x = xoffset;
						rect.y = yoffset;
						rect.w = sLetter->w;
						rect.h = sLetter->h;
						SDL_BlitSurface(sLetter,nullptr,surface,&rect);
						SDL_FreeSurface(sLetter);
					}
				}

				xoffset += maxAdvance + border;
			}

			xoffset = border;
			yoffset += maxHeight + border;
		}

		//SDL_Surface *textureRGBA = SDL_ConvertSurfaceFormat(texture, SDL_PIXELFORMAT_ABGR8888, 0);

		glGenTextures(1, &glFontTexture);
		glBindTexture(GL_TEXTURE_2D, glFontTexture);

		// use linear filetring
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// clamp to edge (Necessaire pour opengles 2.0 pour des images non 2^n)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_FreeSurface(surface);
	}

	
#endif
	unsigned int Font::AddStaticText(const std::string staticText)
	{
		std::size_t start = staticTextStream.str().size();
		staticTextStream << staticText;
		std::size_t end = staticTextStream.str().size()-1;
		tStaticTextOffsets.push_back({start,end});
		return (unsigned int) (tStaticTextOffsets.size() - 1);
	}

	void Font::PrintStaticTexts(int x, int y, unsigned int start, unsigned int end)
	{

		if (end == 0)
			end = staticTextElementNumber/6; //il y a 2 triangles par charactère soit /6

		BH3D_ASSERT_MSG(pShader, "Shader == nullptr",);
		BH3D_ASSERT_MSG(staticTextVbo.IsValid(), "StaticTextVBO invalid- Have to call ComputeStaticText",);
	
		glBindTexture(GL_TEXTURE_2D, glFontTexture);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glm::mat4 projection = glm::ortho(0.0f, (float)viewport[2] - viewport[0], 0.0f, (float)viewport[3] - viewport[1]);

		pShader->Enable();
		pShader->SendMat4f(BH3D_FONT_PROJ_UNIFORM, GL_FALSE, projection);
		pShader->Send4f(BH3D_FONT_COLOR_UNIFORM, textColor);
		
		staticTextVbo.Enable();
		pShader->Send2i(BH3D_FONT_POS_UNIFORM, x, y);
		glDrawElements(GL_TRIANGLES, (end - start + 1)*6, GL_UNSIGNED_INT, ((unsigned int*)nullptr) + (start*6));
	
	}

	bool Font::ComputeStaticText()
	{
		if (staticTextVbo.IsValid())
			staticTextVbo.Destroy();

		GLfloat oneOverTexWidth = 1.0f / static_cast<GLfloat>(texWidth);
		GLfloat oneOverTexHeight = 1.0f / static_cast<GLfloat>(texHeight);

		GLfloat s = 0;
		GLfloat t = 0;

		GLfloat toffset = fontAscent * oneOverTexHeight;
		GLfloat soffset;

		std::vector<glm::vec2> tVertex2d;
		std::vector<glm::vec2> tTexCoord2;

		std::vector<unsigned int> tIndices;


		float x = 0, y = 0;

		const std::string & staticText = staticTextStream.str();

		std::size_t i;
		unsigned int id = 0;
		unsigned int ii = 0; // compteur sur tStaticTextOffsets[ii].end afin d'identifier le changement de texte
		for (i = 0; i < staticText.size(); i++)
		{

			//first triangle of char square
			tIndices.push_back(id);
			tIndices.push_back(id + 1);
			tIndices.push_back(id + 2);

			//second triangle
			tIndices.push_back(id);
			tIndices.push_back(id + 2);
			tIndices.push_back(id + 3);



			unsigned char c = staticText[i];


			//coordonnée du caractere c sur la texture
			s = (c % 16)*(maxAdvance + border) * oneOverTexWidth + (border * oneOverTexWidth);
			t = char(c / 16)*(maxHeight + border) * oneOverTexHeight + (border * oneOverTexHeight);

			soffset = tAdvance[c] * oneOverTexWidth;


			//vertex of char square
			tTexCoord2.push_back(glm::vec2(s, t + toffset - (fontDescent * oneOverTexHeight)));
			tVertex2d.push_back(glm::vec2(x, y + fontDescent));

			tTexCoord2.push_back(glm::vec2(s + soffset, t + toffset - (fontDescent * oneOverTexHeight)));
			tVertex2d.push_back(glm::vec2(x + tAdvance[c], y + fontDescent));

			tTexCoord2.push_back(glm::vec2(s + soffset, t));
			tVertex2d.push_back(glm::vec2(x + tAdvance[c], y + fontAscent));

			tTexCoord2.push_back(glm::vec2(s, t));
			tVertex2d.push_back(glm::vec2(x, y + fontAscent));

			id += 4;//+4 nombres de vertex par char

			if (i == tStaticTextOffsets[ii].end) //nouveau texte
			{
				y = 0;
				x = 0;
				ii++;
				continue;
			}
			else if (staticText[i] == '\n') //new line, add offset on y, reset x and skip the char
			{
				y -= lineSkip;
				x = 0;
				continue;
			}else
				x += tAdvance[c] + border; //x offset for the next char


		}

		staticTextElementNumber = (unsigned int) tIndices.size();
		//vbo creation
		staticTextVbo.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, tVertex2d);
		staticTextVbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tTexCoord2);
		staticTextVbo.AddElementBufferData(&tIndices[0], tIndices.size());
		if (!staticTextVbo.Create())
		{
			BH3D_LOGGERERROR("Static text vbo error");
			return BH3D_ERROR;
		}
		return BH3D_OK;
	}

	void Font::BuildVBO(void)
	{
		dynamicTextVbo.Destroy();

		GLfloat oneOverTexWidth = 1.0f / static_cast<GLfloat>(texWidth);
		GLfloat oneOverTexHeight = 1.0f / static_cast<GLfloat>(texHeight);

		GLfloat s = border * oneOverTexWidth;
		GLfloat t = border * oneOverTexHeight;

		GLfloat toffset = fontAscent * oneOverTexHeight;

		std::vector<glm::vec2> tVertex2d;
		std::vector<glm::vec2> tTexCoord2;

		std::vector<unsigned int> tIndices;

		unsigned int id = 0;		
		for (int i = 0, c = 0; i < 16; ++i)
		{
			for (int j = 0; j < 16; ++j, c++, id += 4)	//id += 4 nombres de vertex par charactere
			{											//c++ pour les 256 charactères

				//first triangle of char square
				tIndices.push_back(id);
				tIndices.push_back(id + 1);
				tIndices.push_back(id + 2);

				//second triangle
				tIndices.push_back(id);
				tIndices.push_back(id + 2);
				tIndices.push_back(id + 3);

				GLfloat soffset = tAdvance[c] * oneOverTexWidth;

				//vertex of char square
				tTexCoord2.push_back(glm::vec2(s, t + toffset - (fontDescent * oneOverTexHeight)));
				tVertex2d.push_back(glm::vec2(0, fontDescent));

				tTexCoord2.push_back(glm::vec2(s + soffset, t + toffset - (fontDescent * oneOverTexHeight)));
				tVertex2d.push_back(glm::vec2(tAdvance[c], fontDescent));

				tTexCoord2.push_back(glm::vec2(s + soffset, t));
				tVertex2d.push_back(glm::vec2(tAdvance[c], fontAscent));

				tTexCoord2.push_back(glm::vec2(s, t));
				tVertex2d.push_back(glm::vec2(0, fontAscent));
				s += (maxAdvance + border) * oneOverTexWidth;

			}
			
			//offset for the next square
			s = border * oneOverTexWidth;
			t += (maxHeight + border) * oneOverTexHeight;
		}

		//vbo creation
		dynamicTextVbo.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, tVertex2d);
		dynamicTextVbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tTexCoord2);
		dynamicTextVbo.AddElementBufferData(&tIndices[0], tIndices.size());
		if (!dynamicTextVbo.Create())
		{
			BH3D_LOGGERERROR("Font VBO error");
		}
	}

	int Font::LengthStaticText(unsigned int id) const
	{
		BH3D_ASSERT_MSG(id < tStaticTextOffsets.size(), "Wrong id", 0);
		const std::string & staticText = staticTextStream.str();
		std::string text = staticText.substr(tStaticTextOffsets[id].start, tStaticTextOffsets[id].end - tStaticTextOffsets[id].start + 1);
		return LengthText(text);
	}

	int Font::LengthText(const std::string & text) const
	{

		/*int w, h;
		if (TTF_SizeText(font, text.c_str(), &w, &h))
			w = -1;

		return w;*/
		unsigned int len = 0;
		for (auto c : text)
		{
			len += tAdvance[c];
		}

		return len;
	}
}

 