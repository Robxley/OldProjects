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

 
#ifndef _BH3D_SDLASSIMPIO_H_
#define _BH3D_SDLASSIMPIO_H_

#include <SDL.h>

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include "Logger.hpp"

namespace BH3D
{

	class SDLIOStream : public Assimp::IOStream
	{
		friend class SDLIOSystem;
	private:
		SDL_RWops *file = nullptr;
	protected:
		SDLIOStream();
	public:
		void 	Flush(){};
		size_t FileSize() const;
		size_t Read(void *pvBuffer, size_t pSize, size_t pCount);
		aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
		size_t Tell()const;
		size_t 	Write(const void *pvBuffer, size_t pSize, size_t pCount);
		~SDLIOStream();
	};


	class SDLIOSystem : public Assimp::IOSystem
	{
	private:
		
		void Close(Assimp::IOStream *pFile);
		bool Exists(const char *pFile) const;
		char getOsSeparator() const;
		Assimp::IOStream * 	Open(const char *pFile, const char *pMode = "rb");
		Assimp::IOStream* Open(const std::string& pFile, const std::string& pMode);
		~SDLIOSystem();

	};



}
#endif //
