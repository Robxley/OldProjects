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


/*!
\file MultiPlatformOpenGL.hpp
\author Robxley (A.CAILLY)
\version 0.1
\date 09/12/2014
\~english
\brief Opengl multiplatform macros
\details  Describe the opengl macro in a multiplatform context
\~french
\brief Macros opengl multiplatforme
\details  Décription des macros opengl dans un contexte multiplatform
*/




#ifndef _BH3D_MULTIPLATFORMOPENGL_H_
#define _BH3D_MULTIPLATFORMOPENGL_H_


#if defined(__IPHONEOS__) || defined(__ANDROID__)

	#ifdef BH3D_USE_OPENGLES3
		#define BH3D_GL_CONTEXT_MAJOR_VERSION 3
	#elif defined(BH3D_USE_OPENGLES2)
		#define	BH3D_GL_CONTEXT_MAJOR_VERSION 2
	#else
		#define BH3D_USE_OPENGLES2
		#define	BH3D_GL_CONTEXT_MAJOR_VERSION 2
	#endif


	#define BH3D_HAVE_OPENGLES

	#ifdef BH3D_USE_OPENGLES3
		// Include OpenGL ES iOS
		#ifdef __IPHONEOS__
			#include <OpenGLES/ES3/gl.h>
		// Include OpenGL ES Android
		#else  //__ANDROID__
			#include <GLES3/gl3.h>
		#endif
	#elif defined(BH3D_USE_OPENGLES2)
		// Include OpenGL ES iOS
		#ifdef __IPHONEOS__
			#include <OpenGLES/ES2/gl.h>
			// Include OpenGL ES Android
		#else  //__ANDROID__
			#include <GLES2/gl2.h>
		#endif
	#endif

#else

	#ifdef _WIN32
		#ifndef GLEW_STATIC
			#define GLEW_STATIC
		#endif
		#include <GL/glew.h>
	#else
		#define GL3_PROTOTYPES 1
		#include <GL3/gl3.h>
	#endif

	//opengl 3.3
	#define BH3D_GL_CONTEXT_MAJOR_VERSION 3
	#define BH3D_GL_CONTEXT_MINOR_VERSION 3

#endif


#endif //_BH3D_MultiPlatformOpenGL_H_
