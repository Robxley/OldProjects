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






#ifndef _BH3D_GLERRORLOGGER_H_
#define _BH3D_GLERRORLOGGER_H_



#include "MultiPlatformOpenGL.hpp"


/**
*\~english
*\brief		Macro error log
*\param[in]	Expression to be tested
*\~french
*\brief		Macro assertion utilisée par biohazard
*\param[in]	Expression à tester
*/
#ifndef NDEBUG
#define BH3D_CHECKGLERROR() BH3D::CheckGLError(__FUNCTION__,__FILE__,__LINE__)
#else
#define BH3D_CHECKGLERROR() BH3D::CheckGLError()
#endif


namespace BH3D
{
#ifndef NDEBUG

	/**
	*\~english
	*\brief		Checks the glGetError function and writes the return in logger.txt file. Only defined in _DEBUG configuration.
	*\param[in]	macro __FUNCTION__
	*\param[in]	macro __FILE__
	*\param[in]	macro __LINE__
	*\return	GLenum Opengl error.
	*\~french
	*\brief		Fonction de controle des erreurs opengl de la fonction glGetError et enregistre le retour dans le ficher logger.txt. Fonction définie qu'en configuration _DEBUG
	*\param[in]	macro __FUNCTION__
	*\param[in]	macro __FILE__
	*\param[in]	macro __LINE__
	*\return	GLenum Opengl error.
	*/
	GLenum CheckGLError(const char *func, const char* file, int line);
#else
	/**
	*\~english
	*\brief		Checks the glGetError function and writes the return in logger.txt file. Only defined in _DEBUG configuration.
	*\return	GLenum Opengl error.
	*\~french
	*\brief		Fonction de controle des erreurs opengl de la fonction glGetError et enregistre le retour dans le ficher logger.txt. Fonction définie qu'en configuration _DEBUG
	*\return	GLenum Opengl error.
	*/
	GLenum CheckGLError();
#endif
}


#endif //
