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


#include "SDLTextureManager.hpp"
#include "Logger.hpp"

#include <memory>

namespace BH3D
{

	SDLTextureManager::~SDLTextureManager()
	{
		BH3D_LOGGER("TextureManager End");
	};
	SDLTextureManager::SDLTextureManager()
	{
		BH3D_LOGGER("TextureManager Start");
	}
	Texture * SDLTextureManager::LoadResourceFromFile(const std::string & filename)
	{
		
		SDL_Surface * texture;

		texture = IMG_Load(filename.c_str()); //charge l'image avec la SDL

		// Vérification du chargement

		if (texture == 0)
		{
			BH3D_LOGGER("Can't Load the texture : " << IMG_GetError() << filename);
			return 0;
		}

		//Force le format RGBA
		SDL_Surface *textureRGBA = SDL_ConvertSurfaceFormat(texture, SDL_PIXELFORMAT_ABGR8888, 0);

		//Suppression de la surface sdl non RGBA
		SDL_FreeSurface(texture);

		GLuint id;
		//création de la texture opengl et remplissage
		glGenTextures(1, &id);
		glBindTexture(gltarget, id);

#ifdef BH3D_USE_OPENGLES2
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
		glTexParameteri(gltarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(gltarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(gltarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif

		if (useMipmap) 
			glTexParameteri(gltarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			
		glTexImage2D(gltarget,
			0,
			GL_RGBA,
			textureRGBA->w, textureRGBA->h,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			textureRGBA->pixels);

		if (useMipmap) glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(textureRGBA); /* No longer needed */

		
		return (new Texture(id, gltarget));

	}
	void SDLTextureManager::FreeResource(Texture* ressource)
	{
		//suppression de la texture opengl
		GLuint id = ressource->GetGLTexture();
		glDeleteTextures(1, &id);

		//suppression de la ressource
		delete ressource;
	}
	void SDLTextureManager::UseMipmap(bool mipmap)
	{
		useMipmap = mipmap;
	}
}
 

 
 
 
 