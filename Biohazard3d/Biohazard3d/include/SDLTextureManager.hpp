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




 
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include <SDL_image.h>
 
#ifndef _BH3D_SDLTEXTUREMANAGER_H_
#define _BH3D_SDLTEXTUREMANAGER_H_

#define BH3D_TextureManager() BH3D::SDLTextureManager::Instance()
#define BH3D_LoadTexture(msg) BH3D::SDLTextureManager::Instance().Load(msg)


namespace BH3D
{

	class SDLTextureManager : public ResourceManager<SDLTextureManager, Texture>
	{
		friend ResourceManager;

		public:
			void UseMipmap(bool mipmap);

			//GL_TEXTURE_2D, GL_TEXTURE_ARRAY_2D, ...
			inline void SetTextureTarget(GLenum target) {gltarget = target;	}; 

		protected:

			//singleton
			SDLTextureManager();
			SDLTextureManager(const SDLTextureManager & r){};
			~SDLTextureManager();

			Texture * LoadResourceFromFile(const std::string & filename);
			Texture * LoadResourceFromRaw(const void * data) { return defaultResource; }//not implemented

			void FreeResource(Texture* ressource);
			bool useMipmap = 1;
			GLenum gltarget = GL_TEXTURE_2D;
	};

}
#endif //
