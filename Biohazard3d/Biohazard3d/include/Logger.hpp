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


#ifndef _BH3D_LOGGER_H_
#define _BH3D_LOGGER_H_



#define BH3D_VERBOSE

#ifndef BH3D_VERBOSE

#define BH3D_LOGGER(msg)		
#define BH3D_LOGGERERROR(msg)
#define BH3D_LOGGERWARNING(msg)

#else

#include <SDL.h>
#include <sstream>
#define BH3D_LOGGER_FILE "BH3Dlogger.log"




#if defined(__ANDROID__) || defined(__IPHONEOS__)
	#define BH3D_USE_SDL_LOGGER
#elif defined(WIN32)
	#define BH3D_USE_COUT_LOGGER
#else
	#define BH3D_USE_FILE_LOGGER
#endif



#ifdef BH3D_USE_SDL_LOGGER

#define BH3D_LOGGER(msg)		{std::ostringstream a; a <<"------------"<<msg<<std::endl ; SDL_Log(a.str().c_str());}
#define BH3D_LOGGERERROR(msg)	{std::ostringstream a; a <<"<ERROR>-----"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ; SDL_Log(a.str().c_str());}
#define BH3D_LOGGERWARNING(msg) {std::ostringstream a; a <<"<WARNING>---"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ; SDL_Log(a.str().c_str());}

#elif defined(BH3D_USE_COUT_LOGGER)

#include<iostream>
#define BH3D_LOGGER(msg)		{std::cout<<"------------"<<msg<<std::endl;}
#define BH3D_LOGGERERROR(msg)	{std::cout<<"<ERROR>-----"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ;}
#define BH3D_LOGGERWARNING(msg) {std::cout<<"<WARNING>---"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl ;}



#elif defined(BH3D_USE_FILE_LOGGER)
	
	#define BH3D_LOGGER(msg) BH3D::Logger::Instance().getFileLogger()			<<"------------"<<msg<<std::endl
	#ifndef NDEBUG
	#define BH3D_LOGGERERROR(msg) BH3D::Logger::Instance().getFileLogger()		<<"<ERROR>-----"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl
	#define BH3D_LOGGERWARNING(msg) BH3D::Logger::Instance().getFileLogger()	<<"<WARNING>---"<<msg<<" -- FUNC <"<<__FUNCTION__<<"> -- FILE <"<<__FILE__<<"> -- LINE <"<<__LINE__<<">"<<std::endl
	#else
	#define BH3D_LOGGERERROR(msg) BH3D::Logger::Instance().getFileLogger()		<<"<ERROR>-----"<<msg<<std::endl
	#define BH3D_LOGGERWARNING(msg) BH3D::Logger::Instance().getFileLogger()	<<"<WARNING>---"<<msg<<std::endl
	#endif
	


#include <fstream>
#include <iomanip>
#include <sstream>


namespace BH3D
{
	/*!
	\author 	Robxley (A.CAILLY)
	\version	0.1
	\date		08/12/2014
	\~english
	\brief		Logger Class to log what you want in a speficique file. (See macro BH3D_LOGGER_FILE "BH3Dlogger.log").
	\remark		It's a singleton. See macros BH3D_LOGGER(msg), BH3D_LOGGERERROR(msg) and BH3D_LOGGERWARNING(msg).
	\~french
	\brief		Classer logger. Pour écrire ce que tu veux dans un fichier spécifique (voir macro BH3D_LOGGER_FILE "BH3Dlogger.log")
	\remark		C'est un singleton. Voir les macros BH3D_LOGGER(msg), BH3D_LOGGERERROR(msg) and BH3D_LOGGERWARNING(msg).
	*/
	class Logger
	{
		private:
			std::ofstream      logFile;
			/**
			*\~english
			*\brief		Private Constructor - Creates the file define by the macro BH3D_LOGGER_FILE.
			*\~french
			*\brief		Private Constructor - Créée le fichier défini par la macro BH3D_LOGGER_FILE.
			*/
			Logger();

			/**
			*\~english
			*\brief		Private Constructor - Closes the file.
			*\~french
			*\brief		Private Constructor - Ferme le fichier.
			*/
			~Logger();

		public:

			/**
			*\~english
			*\brief		Function to access to the logger.
			*\return	return a reference on logger.
			*\~french
			*\brief		Utiliser cette fonction pour avoir accès au logger.
			*\return	retourne une reference sur le logger.
			*/
			static Logger &Instance();

			/**
			*\~english
			*\brief		gets the logger ofstream.
			*\return	return a reference on the used ofstream.
			*\~french
			*\brief		Pour obtenir le "ofstream" du logger.
			*\return	retourne une reference sur le ofstream utilisé par le logger.
			*/
			std::ofstream& getFileLogger();


	};

}
#endif //
#endif

#endif