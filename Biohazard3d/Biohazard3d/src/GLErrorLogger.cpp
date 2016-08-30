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



#include "GLErrorLogger.hpp"
#include "Logger.hpp"

namespace BH3D
{

#ifndef NDEBUG

	GLenum CheckGLError(const char *func, const char* file, int line)
	{

		GLenum error;
		error = glGetError();
		switch (error)
		{
		case GL_NO_ERROR:
			//BH3D_LOGGER("[CHECK_GL_ERROR] NONE ");
			break;
		case GL_INVALID_ENUM:
			BH3D_LOGGER("[CHECK_GL_ERROR] INVALID_ENUM " << " -- FUNC <" << func << "> -- FILE <" << file << "> -- LINE <" << line << ">" << std::endl);
			break;
		case GL_INVALID_VALUE:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_INVALID_VALUE " << " -- FUNC <" << func << "> -- FILE <" << file << "> -- LINE <" << line << ">" << std::endl);
			break;
		case GL_INVALID_OPERATION:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_INVALID_OPERATION " << " -- FUNC <" << func << "> -- FILE <" << file << "> -- LINE <" << line << ">" << std::endl);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_INVALID_FRAMEBUFFER_OPERATION " << " -- FUNC <" << func << "> -- FILE <" << file << "> -- LINE <" << line << ">" << std::endl);
			break;
		case GL_OUT_OF_MEMORY:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_OUT_OF_MEMORY " << " -- FUNC <" << func << "> -- FILE <" << file << "> -- LINE <" << line << ">" << std::endl);
			break;
		}
		return error;
	}
#else
	GLenum CheckGLError()
	{

		GLenum error;
		error = glGetError();
		switch (error)
		{
		case GL_NO_ERROR:
			//BH3D_LOGGER("[CHECK_GL_ERROR] NONE ");
			break;
		case GL_INVALID_ENUM:
			BH3D_LOGGER("[CHECK_GL_ERROR] INVALID_ENUM " << std::endl);
			break;
		case GL_INVALID_VALUE:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_INVALID_VALUE " << std::endl);
			break;
		case GL_INVALID_OPERATION:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_INVALID_OPERATION " << std::endl);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_INVALID_FRAMEBUFFER_OPERATION " << std::endl);
			break;
		case GL_OUT_OF_MEMORY:
			BH3D_LOGGER("[CHECK_GL_ERROR] GL_OUT_OF_MEMORY " << std::endl);
			break;
		}
		return error;
	}

#endif	

}