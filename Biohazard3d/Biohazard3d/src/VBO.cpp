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

/**
* \file VBO.cpp
* \brief Classe pour la gestion des "Vertex Buffer Objects"
* \author Robxley
* \version 0.1
* \date 09/12/2014
*
* \details Template comment pour doxygen doxygen
*
*/


#include "VBO.hpp"
#include "Assertion.hpp"
#include "Logger.hpp"
#include "GLErrorLogger.hpp"

namespace BH3D
{

	VBO::~VBO()
	{
		Destroy();
	}

	bool VBO::Create(GLenum mod)
	{

		if (arrayBufferID || elementBufferID)
			Destroy();

		//creation vbo
		if (tArrayBuffers.size())
		{
			std::size_t fullSize = 0;

			//calcul la taille max
			for (const auto &buffer : tArrayBuffers)
				fullSize += buffer.byteSize;
			
			//creation du VBO
			glGenBuffers(1, &arrayBufferID); //génération d'un buffer
			glBindBuffer(GL_ARRAY_BUFFER, arrayBufferID);  //activation du buffer
			glBufferData(GL_ARRAY_BUFFER, fullSize, nullptr, mod); //allocation mémoire du buffer array

			//copie des données dans le vbo
			std::size_t offset = 0;
			for (const auto &buffer : tArrayBuffers)
			{
				glBufferSubData(GL_ARRAY_BUFFER, offset, buffer.byteSize, buffer.data);
				offset += buffer.byteSize; //on décale l'offset aux données suivantes.
			}
		}

		//creation element array
		if (tElementBuffers.size())
		{
			//calcul la taille maximal
			std::size_t fullSize = 0;

			for (const auto &buffer : tElementBuffers)
				fullSize += buffer.byteSize;

			glGenBuffers(1, &elementBufferID); //génération d'un buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);  //activation du buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, fullSize, nullptr, mod); //allocation memoire du buffer
			
			//remplissage du buffer element
			std::size_t offset = 0;
			for (const auto &buffer : tElementBuffers)
			{
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, buffer.byteSize, buffer.data);
				offset += buffer.byteSize; //on décale l'offset aux données suivantes.
			}

		}

		BH3D_CHECKGLERROR();

		#ifdef BH3D_USE_OPENGLES2
			return (arrayBufferID && elementBufferID);
		#else
			BuildVAO();
			return (arrayBufferID && elementBufferID && vertexArraysID);
		#endif

	}



#ifdef BH3D_USE_OPENGLES2
	void VBO::VAOSimulation() const
	{

		//Liaison avec Element Array
		if (elementBufferID)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

		if (arrayBufferID)
		{
			//Liaison avec le VBO
			glBindBuffer(GL_ARRAY_BUFFER, arrayBufferID);

			std::size_t offset = 0;
			for (const auto & buffer : tArrayBuffers)
			{
				//pour chaque attribut
				for (unsigned int k = 0; k < buffer.nbAttrib; k++)
				{
					glEnableVertexAttribArray(buffer.tAttribIndex[k]);
					glVertexAttribPointer(buffer.tAttribIndex[k], buffer.tAttribSize[k], buffer.tAttribType[k], GL_FALSE, buffer.stride, BH3D_BUFFER_OFFSET(offset + buffer.tAttribOffsetStart[k]));
				}
				offset += buffer.byteSize;
			}
		}
	}
#else

	void VBO::BuildVAO()
	{
		
		//création et définition du VAO
		glGenVertexArrays(1, &vertexArraysID);
		glBindVertexArray(vertexArraysID);

		//Liaison avec Element Array
		if (elementBufferID)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

		if (arrayBufferID)
		{
			//Liaison avec le VBO
			glBindBuffer(GL_ARRAY_BUFFER, arrayBufferID);

			std::size_t offset = 0;
			for (const auto & buffer : tArrayBuffers)
			{
				int lastAttribIndex = -1;
				//pour chaque attribut
				for (unsigned int k = 0; k < buffer.nbAttrib; k++)
				{
					if (lastAttribIndex == buffer.tAttribIndex[k]) continue; //seul le premier est pris en compte en cas de plusieurs attribut identique
					lastAttribIndex = buffer.tAttribIndex[k];

					glEnableVertexAttribArray(buffer.tAttribIndex[k]);

					if (buffer.tAttribInType.size() && buffer.tAttribInType[k] == BH3D_ATTRIB_IVEC)
						glVertexAttribIPointer(buffer.tAttribIndex[k], buffer.tAttribSize[k], buffer.tAttribType[k], buffer.stride, BH3D_BUFFER_OFFSET(offset + buffer.tAttribOffsetStart[k]));
					else
						glVertexAttribPointer(buffer.tAttribIndex[k], buffer.tAttribSize[k], buffer.tAttribType[k], GL_FALSE, buffer.stride, BH3D_BUFFER_OFFSET(offset + buffer.tAttribOffsetStart[k]));
				}
				offset += buffer.byteSize;
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			}
	}
#endif

	void VBO::Destroy()
	{
		if (arrayBufferID) glDeleteBuffers(1, &arrayBufferID);
		if (elementBufferID) glDeleteBuffers(1, &elementBufferID);

		arrayBufferID = 0;
		elementBufferID = 0;

		#ifndef BH3D_USE_OPENGLES2
			if (elementBufferID) glDeleteVertexArrays(1, &vertexArraysID);
			vertexArraysID = 0;
		#endif

		tElementBuffers.clear();
		tArrayBuffers.clear();
	}


	//Array buffer
	//-------------------------------

	void VBO::AddArrayBufferData(GLuint indexAttrib, const void *data, std::size_t byteSize, GLint vertexSize, GLenum dataType)
	{
		BH3D_ASSERT(byteSize > 0 && data != nullptr);

		if (byteSize == 0 || data == nullptr)
			return;

		tArrayBuffers.push_back(ArrayBuffer());
		tArrayBuffers.back().nbAttrib = 1;
		tArrayBuffers.back().data = data;
		tArrayBuffers.back().byteSize = byteSize;
		tArrayBuffers.back().tAttribIndex.push_back(indexAttrib);
		tArrayBuffers.back().tAttribOffsetStart.push_back(0);
		tArrayBuffers.back().tAttribSize.push_back(vertexSize);
		tArrayBuffers.back().tAttribType.push_back(dataType);
	}

	//element array buffer
	void VBO::AddElementBufferData(const void *data, std::size_t size, GLenum type)
	{
		ElementBuffer tmp;
		tmp.byteSize = size;
		tmp.data = data;
		tmp.type = type;
		tElementBuffers.push_back(tmp);

	}

	//Structure array buffer
	//----------------------------------
#ifndef BH3D_USE_OPENGLES2
	void VBO::AddStructArrayBufferData(GLuint nbAttrib, const std::vector<GLuint> &tAttribIndex, const std::vector<GLenum> &tAttribType, const std::vector<GLuint> &tAttribSize, const std::vector<GLuint> &tAttribOffsetStart, GLuint stride, std::size_t size, const void *data, const std::vector<unsigned int> &tAttribInType)
#else
	void VBO::AddStructArrayBufferData(GLuint nbAttrib, const std::vector<GLuint> &tAttribIndex, const std::vector<GLenum> &tAttribType, const std::vector<GLuint> &tAttribSize, const std::vector<GLuint> &tAttribOffsetStart, GLuint stride, std::size_t size, const void *data)
#endif
	{

#ifndef BH3D_USE_OPENGLES2
		const unsigned int *ptAttribInType = nullptr;
		if (tAttribInType.size())
			ptAttribInType = &tAttribInType[0];
#endif
		AddStructArrayBufferData(nbAttrib, &tAttribIndex[0], &tAttribType[0], &tAttribSize[0], &tAttribOffsetStart[0], stride, size, data
#ifndef BH3D_USE_OPENGLES2
			,ptAttribInType);
#else
			);
#endif
	}

#ifndef BH3D_USE_OPENGLES2
	void VBO::AddStructArrayBufferData(GLuint nbAttrib, const GLuint *ptAttribIndex, const GLenum *ptAttribType, const GLuint *ptAttribSize, const GLuint *ptAttribOffsetStart, GLuint stride, std::size_t size, const void *data, const unsigned int *ptAttribInType)
#else
	void VBO::AddStructArrayBufferData(GLuint nbAttrib, const GLuint *ptAttribIndex, const GLenum *ptAttribType, const GLuint *ptAttribSize, const GLuint *ptAttribOffsetStart, GLuint stride, std::size_t size, const void *data)
#endif
	{
		tArrayBuffers.push_back(ArrayBuffer());

		tArrayBuffers.back().nbAttrib = nbAttrib;
		tArrayBuffers.back().byteSize = size;
		tArrayBuffers.back().data = data;
		tArrayBuffers.back().stride = stride;

		tArrayBuffers.back().tAttribIndex.resize(nbAttrib);
		tArrayBuffers.back().tAttribSize.resize(nbAttrib);
		tArrayBuffers.back().tAttribOffsetStart.resize(nbAttrib);
		tArrayBuffers.back().tAttribType.resize(nbAttrib);

		#ifndef BH3D_USE_OPENGLES2
			tArrayBuffers.back().tAttribInType.resize(nbAttrib);
		#endif

		for (unsigned int i = 0; i < nbAttrib; i++)
		{
			tArrayBuffers.back().tAttribIndex[i] = ptAttribIndex[i];
			tArrayBuffers.back().tAttribType[i] = ptAttribType[i];
			tArrayBuffers.back().tAttribSize[i] = ptAttribSize[i];
			tArrayBuffers.back().tAttribOffsetStart[i] = ptAttribOffsetStart[i];
			#ifndef BH3D_USE_OPENGLES2
				if (ptAttribInType)
					tArrayBuffers.back().tAttribInType[i] = ptAttribInType[i];
			#endif
		}


	}



}