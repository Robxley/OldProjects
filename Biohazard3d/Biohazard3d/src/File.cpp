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


 
 
#include "File.hpp"
#include <algorithm>
#include <fstream>
#include "Logger.hpp"
#include "Assertion.hpp"
#include "Common.hpp"


#ifdef __ANDROID__
#include <SDL.h>  //utilisation de la fonction SDL_RWFromFile pour ouvrir un fichier
#include "SDLSmart.hpp"
#endif


namespace BH3D
{
	File::File(const File & file) : fullName(file.fullName)
	{

	}
	File::File(const std::string& file) :
		fullName(file)
	{
		 //std::replace(fullName.begin(), fullName.end(), '\\', '/');
	}



	File::File(const char* file) :
		fullName(file)
	{
		//std::replace(fullName.begin(), fullName.end(), '\\', '/');
	}

	File::operator std::string() const
	{
		return fullName;
	}
	File::operator std::string&()
	{
		return fullName;
	}


	bool File::ReadAndSaveIn(std::string & buffer) const
	{
#ifdef __ANDROID__

		SDL_RWops *file = SDL_RWFromFile(fullName.c_str(), "rb");
		if (file == nullptr)
		{
			BH3D_LOGGERERROR("Can't open the file : " << fullName);
			return BH3D_ERROR;
		}

		std::size_t bufferLenght = SDL_RWseek(file, 0, RW_SEEK_END);
		if(bufferLenght <= 1)
			BH3D_LOGGERERROR("File is empty : " << fullName);

		buffer.resize(bufferLenght,0);
		SDL_RWseek(file, 0, RW_SEEK_SET);
		size_t n_blocks = SDL_RWread(file, (void*)&buffer.front(), 1, bufferLenght);
		SDL_RWclose(file);
		file = nullptr;

		return BH3D_OK;

#else
		// On ouvre le fichier du code source
		std::ifstream ifs(fullName);

		// On verifie que le fichier existe
		if (!ifs.is_open())
		{
			BH3D_LOGGERERROR("File no exist : " << fullName);
			return BH3D_ERROR;
		}

		//save the file data in the buffer
		buffer.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		ifs.close();
		//buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());


		if (buffer.empty())
		{
			BH3D_LOGGERERROR("File Empty : " << fullName);
			return BH3D_ERROR;
		}
		return BH3D_OK;
#endif
	}


	void File::WriteIn(const std::string & data, bool erase)
	{
		std::ofstream ofs;

		if (erase)
			ofs.open(fullName);
		else
			ofs.open(fullName,std::ofstream::app);

		ofs.write(data.c_str(), data.size());
		
		ofs.close();
	}




	//----------------------------------------------------------
	// Constructeur à partir d'un std::string
	//----------------------------------------------------------
	void File::set(const std::string& name)
	{
		fullName = name;
	}

	////////////////////////////////////////////////////////////
	// Indique si le fichier existe ou non
	//
	// [retval] True si le fichier existe
	//
	////////////////////////////////////////////////////////////
	bool File::Exists() const
	{
		std::ifstream File(fullName.c_str());

		return File.is_open();
	}


	////////////////////////////////////////////////////////////
	// Renvoie le nom du fichier avec son chemin complet
	//
	// [retval] Chemin complet du fichier
	//
	////////////////////////////////////////////////////////////
	const std::string& File::Fullname() const
	{
		return fullName;
	}


	////////////////////////////////////////////////////////////
	// Renvoie le nom du fichier sans son chemin
	//
	// [retval] Nom du fichier
	//
	////////////////////////////////////////////////////////////
	std::string File::Filename() const
	{
		std::string::size_type Pos = fullName.find_last_of("\\/");

		if (Pos != std::string::npos)
			return fullName.substr(Pos + 1, std::string::npos);
		else
			return fullName;
	}


	////////////////////////////////////////////////////////////
	// Renvoie le nom du fichier sans extension ni chemin
	//
	// [retval] Nom du fichier
	//
	////////////////////////////////////////////////////////////
	std::string File::ShortFilename() const
	{
		return Filename().substr(0, Filename().find_last_of("."));
	}


	////////////////////////////////////////////////////////////
	// Renvoie l'extension du fichier
	//
	// [retval] Extension du fichier
	//
	////////////////////////////////////////////////////////////
	std::string File::Extension() const
	{
		std::string::size_type Pos = fullName.find_last_of(".");
		if (Pos != std::string::npos)
			return fullName.substr(Pos + 1, std::string::npos);
		else
			return "";
	}

	std::string File::Dir() const
	{
		std::string::size_type Pos = fullName.find_last_of("\\/");
		if (Pos != std::string::npos)
			return fullName.substr(0, Pos);
		else
			return "";
	}



}