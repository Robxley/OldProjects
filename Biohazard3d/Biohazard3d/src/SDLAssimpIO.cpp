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

#include "SDLAssimpIO.hpp"

namespace BH3D
{
	SDLIOStream::SDLIOStream()
	{
		file = nullptr;
	}
	size_t SDLIOStream::FileSize() const
	{
		if (file == nullptr)
			return 0;
		return SDL_RWsize(file);
	}

	size_t SDLIOStream::Read(void *pvBuffer, size_t pSize, size_t pCount)
	{
		return SDL_RWread(file,pvBuffer, pSize, pCount);
	}
	aiReturn SDLIOStream::Seek(size_t pOffset, aiOrigin pOrigin)
	{
		/* inutile les macros correspondent entre assimp et sdl
		if (pOrigin == aiOrigin_SET)
			SDL_RWseek(file, pOffset, RW_SEEK_SET);
		if (pOrigin == aiOrigin_CUR)
			SDL_RWseek(file, pOffset, RW_SEEK_CUR);
		if (pOrigin == aiOrigin_END)
			SDL_RWseek(file, pOffset, RW_SEEK_END);
		*/
		if (SDL_RWseek(file, pOffset, pOrigin) != -1)
			return aiReturn_SUCCESS;
		else
			return aiReturn_FAILURE;
	}
	size_t SDLIOStream::Tell() const
	{
		return SDL_RWtell(file);
	}
	size_t 	SDLIOStream::Write(const void *pvBuffer, size_t pSize, size_t pCount)
	{
		return SDL_RWwrite(file, pvBuffer, pSize, pCount);
	}
	SDLIOStream::~SDLIOStream()
	{
		if (file != nullptr)
			SDL_RWclose(file);
		file = nullptr;
	}
	


	void SDLIOSystem::Close(Assimp::IOStream *pFile)
	{
		if (pFile != nullptr)
			delete pFile;
	}
	bool SDLIOSystem::Exists(const char *pFile) const
	{
		SDL_RWops *file = SDL_RWFromFile(pFile, "rb");
		if (file == nullptr)
			return 0;


		SDL_RWclose(file);
		return 1;
		
	}
	char SDLIOSystem::getOsSeparator() const
	{
		return '/';
	}
	Assimp::IOStream* SDLIOSystem::Open(const std::string& pFile, const std::string& pMode)
	{
		SDLIOStream * stream = new SDLIOStream();

		stream->file = SDL_RWFromFile(pFile.c_str(), pMode.c_str());

		return stream;
	}
	Assimp::IOStream * 	SDLIOSystem::Open(const char *pFile, const char *pMode)
	{
		SDL_RWops *file = SDL_RWFromFile(pFile, pMode);
		if (file == nullptr)
		{
			BH3D_LOGGERERROR("Can't opent the file : "+std::string(pFile));
			return nullptr;
		}
			
		BH3D_LOGGER("File opened : " + std::string(pFile));
		SDLIOStream * stream = new SDLIOStream();
		stream->file = file;
		return stream;
	}
	SDLIOSystem::~SDLIOSystem()
	{ 
		
	}



}


/*
SDL_RWops *file = SDL_RWFromFile(fullName.c_str(), "rb");
if (file == nullptr)
{
	BH3D_LOGGERERROR("Can't open the file : " << fullName);
	return BH3D_ERROR;
}

std::size_t bufferLenght = SDL_RWseek(file, 0, RW_SEEK_END);
if (bufferLenght <= 1)
BH3D_LOGGERERROR("File is empty : " << fullName);

buffer.resize(bufferLenght, 0);
SDL_RWseek(file, 0, RW_SEEK_SET);
size_t n_blocks = SDL_RWread(file, (void*)&buffer.front(), 1, bufferLenght);
SDL_RWclose(file);

return BH3D_OK;

 */
 
 
 