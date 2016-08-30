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


#ifndef _BH3D_VBO_H_
#define _BH3D_VBO_H_


#include <glm/glm.hpp>
#include <vector>

#include "Common.hpp"
#include "MultiPlatformOpenGL.hpp"
#include "Assertion.hpp"
#include "GLErrorLogger.hpp"

#define BH3D_ATTRIB_VEC 0	// glVertexAttribPointer

#ifndef BH3D_USE_OPENGLES2  
#define BH3D_ATTRIB_IVEC 1	// glVertexAttribIPointer (no exist with opengles2.0)
#else
#define BH3D_ATTRIB_IVEC 0	// glVertexAttribIPointer (no exist with opengles2.0)
//#define BH3D_ATTRIB_LVEC 2  // glVertexAttribLPointer -- Vraiment utile ? (non supporté)
#endif

namespace BH3D
{
	class VBO
	{
	public:

		~VBO();

		void Destroy();
		bool Create(GLenum mod = GL_STATIC_DRAW);

		// VAO activation/desactivation
		inline void Enable() const;
		inline void Disable() const;

		inline bool IsValid() const; //Retourne : ArrayBufferID || ElementBufferID

		inline GLuint GetArrayBufferID() const;
		inline GLuint GetElementBufferID() const;

#ifndef BH3D_USE_OPENGLES2 //(no exist with opengles2.0)
		inline GLuint GetVertexArraysID() const;
#endif

		//il est important que les accès mémoires à *data restent disponible jusqu'a l'appel de VBO::Create(...)
		void AddElementBufferData(const void *data, std::size_t byteSize, GLenum dataType);
		void AddArrayBufferData(GLuint indexAttrib, const void *data, std::size_t byteSize, GLint vertexSize, GLenum dataType);

		//surcharge element buffer
		inline void AddElementBufferData(const unsigned int* data, std::size_t size);
		inline void AddElementBufferData(const std::vector<unsigned int> & data);

		//surcharge float type
		inline void AddArrayBufferData(GLuint indexAttrib, const float* data, std::size_t size, GLint vertexSize = 3);

		inline void AddArrayBufferData(GLuint indexAttrib, const glm::vec2* data, std::size_t size);
		inline void AddArrayBufferData(GLuint indexAttrib, const glm::vec3* data, std::size_t size);
		inline void AddArrayBufferData(GLuint indexAttrib, const glm::vec4* data, std::size_t size);

		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<float> &data, GLuint vertexSize = 3);
		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::vec2> &data);
		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::vec3> &data);
		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::vec4> &data);

		//surcharge int type
		inline void AddArrayBufferData(GLuint indexAttrib, const int* data, std::size_t size, GLint vertexSize = 3);

		inline void AddArrayBufferData(GLuint indexAttrib, const glm::ivec2* data, std::size_t size);
		inline void AddArrayBufferData(GLuint indexAttrib, const glm::ivec3* data, std::size_t size);
		inline void AddArrayBufferData(GLuint indexAttrib, const glm::ivec4* data, std::size_t size);

		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<int> &data, GLuint vertexSize = 3);
		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::ivec2> &data);
		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::ivec3> &data);
		inline void AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::ivec4> &data);

		//surcharge structure abstraite
#ifndef BH3D_USE_OPENGLES2 //Because glVertexAttribIPointer (no exist with opengles2.0)
		void AddStructArrayBufferData(GLuint nbAttrib, const GLuint *ptAttribIndex, const GLenum *ptAttribType, const GLuint *ptAttribSize, const GLuint *ptAttribOffsetSize, GLuint  stride,  std::size_t size, const void *data, const unsigned int * ptAttribInType = nullptr);
		void AddStructArrayBufferData(GLuint nbAttrib, const std::vector<GLuint> &tAttribIndex, const std::vector<GLenum> &tAttribType, const std::vector<GLuint> &tAttribSize, const std::vector<GLuint> &tAttribOffsetSize, GLuint stride, std::size_t size, const void *data, const std::vector<GLuint> &tAttribInType = std::vector<GLuint>());
#else
		void AddStructArrayBufferData(GLuint nbAttrib, const GLuint *ptAttribIndex, const GLenum *ptAttribType, const GLuint *ptAttribSize, const GLuint *ptAttribOffsetSize, GLuint stride, std::size_t size, const void *data);
		void AddStructArrayBufferData(GLuint nbAttrib, const std::vector<GLuint> &tAttribIndex, const std::vector<GLenum> &tAttribType, const std::vector<GLuint> &tAttribSize, const std::vector<GLuint> &tAttribOffsetSize, GLuint stride, std::size_t size, const void *data);
#endif

	private:

#ifdef BH3D_USE_OPENGLES2
		void VAOSimulation() const; //use with opengles2 to simulate VAO
#else
		void BuildVAO();
#endif

#ifndef BH3D_USE_OPENGLES2 //(no exist with opengles2.0)
		GLuint vertexArraysID = 0;  //VAO 
#endif
		GLuint arrayBufferID = 0;   //VBO
		GLuint elementBufferID = 0; //

		//structure pour les fonctions glVertexAttribXPointer
		typedef struct _ArrayBuffer
		{
		public:
			GLuint nbAttrib = 0;			//nombre d'attribut
			GLuint stride = 0;
			std::size_t byteSize = 0;			//taille en bytes (octets)
			const void *data = nullptr;			//pointeur sur les données
			std::vector<GLuint> tAttribIndex;	//index des Attributs  (glVertexAttribPointer)- tableau de la taille tAttribIndex[nbAttrib]
			std::vector<GLenum> tAttribType;	//le type des attributs - tableau de la taille tAttribType[nbAttrib] GL_INT, GL_FLOAT etc...
			std::vector<GLuint> tAttribSize;    //1,2,3,4 (vecteur a 1,2,3 ou 4 coordonnées)- tableau de la taille tAttribType[tAttribSize]
			std::vector<GLuint> tAttribOffsetStart; //offset en octet entre chaque element de la structure - tableau de la taille tAttribOffsetSize[tAttribSize]

#ifndef BH3D_USE_OPENGLES2 //because of glVertexAttribIPointer (no exist with opengles2.0)
			std::vector<unsigned int> tAttribInType; // BH3D_ATTRIBUT_VEC, BH3D_ATTRIBUT_IVEC, BH3D_ATTRIBUT_LVEC
#endif
			
		}ArrayBuffer;


		typedef struct _ElementBuffer
		{
			std::size_t byteSize = 0;		//taille en byte
			const void *data = nullptr;
			GLenum	type = GL_UNSIGNED_INT;
		} ElementBuffer;

		std::vector<ElementBuffer> tElementBuffers;
		std::vector<ArrayBuffer> tArrayBuffers;

	};

	//inline functions
	inline void VBO::Enable() const
	{
		BH3D_ASSERT_MSG(IsValid(), "Can't Enable the VBO (invalid ID)", );

#ifdef BH3D_USE_OPENGLES2
		VAOSimulation();
#else
		glBindVertexArray(vertexArraysID);
#endif
	}


	inline void VBO::Disable() const
	{

#ifdef BH3D_USE_OPENGLES2
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
		glBindVertexArray(0);
#endif

		BH3D_CHECKGLERROR();
	}

#ifndef BH3D_USE_OPENGLES2
	inline GLuint VBO::GetVertexArraysID() const
	{
		return vertexArraysID;
	}
#endif
	inline GLuint VBO::GetArrayBufferID() const
	{
		return arrayBufferID;
	}
	inline GLuint VBO::GetElementBufferID() const
	{
		return elementBufferID;
	}

	inline bool VBO::IsValid() const
	{
#ifdef BH3D_USE_OPENGLES2
		return arrayBufferID || elementBufferID;
#else
		return (arrayBufferID || elementBufferID) && vertexArraysID;
#endif
	}



	//surcharge inline
	//---------------------------------------
	//float
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const float* data, std::size_t size, GLint vertexSize)
	{
		AddArrayBufferData(indexAttrib, data, sizeof(float)*size, vertexSize, GL_FLOAT);
	}
	//glm::vec
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const glm::vec2* data, std::size_t dataSize)
	{
		AddArrayBufferData(indexAttrib, (const float*)data, dataSize * 2, 2);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const glm::vec3* data, std::size_t dataSize)
	{
		AddArrayBufferData(indexAttrib, (const float*)data, dataSize * 3, 3);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const glm::vec4* data, std::size_t dataSize)
	{
		AddArrayBufferData(indexAttrib, (const float*)data, dataSize * 4, 4);
	}

	//<vector> float
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<float> &data, GLuint vertexSize)
	{
		AddArrayBufferData(indexAttrib, (const float*)(&data[0]), data.size(), vertexSize);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::vec2> &data)
	{
		AddArrayBufferData(indexAttrib, (const float*)(&data[0].x), data.size() * 2, 2);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::vec3> &data)
	{
		AddArrayBufferData(indexAttrib, (const float*)(&data[0].x), data.size() * 3, 3);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::vec4> &data)
	{
		AddArrayBufferData(indexAttrib, (const float*)(&data[0].x), data.size() * 4, 4);
	}
	
	//int glm::ivec
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const glm::ivec2* data, std::size_t size)
	{
		AddArrayBufferData(indexAttrib, (const int*)data, size * 2, 2);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const glm::ivec3* data, std::size_t size)
	{
		AddArrayBufferData(indexAttrib, (const int*)data, size * 3, 3);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const glm::ivec4* data, std::size_t size)
	{
		AddArrayBufferData(indexAttrib, (const int*)data, size * 4, 4);
	}

	//int <vector> GLM::iVECx
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<int> &data, GLuint vertexSize)
	{
		AddArrayBufferData(indexAttrib, (const int*)(&data[0]), data.size(), vertexSize);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::ivec2> &data)
	{
		AddArrayBufferData(indexAttrib, (const int*)(&data[0].x), data.size() * 2, 2);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::ivec3> &data)
	{
		AddArrayBufferData(indexAttrib, (const int*)(&data[0].x), data.size() * 3, 3);
	}
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const std::vector<glm::ivec4> &data)
	{
		AddArrayBufferData(indexAttrib, (const int*)(&data[0].x), data.size() * 4, 4);
	}
	//surcharge element buffer
	inline void VBO::AddElementBufferData(const unsigned int* data, std::size_t size)
	{
		AddElementBufferData(data, size*sizeof(unsigned int), GL_UNSIGNED_INT);
	}

	inline void VBO::AddElementBufferData(const std::vector<unsigned int> & data)
	{
		AddElementBufferData(&data[0], data.size());
	}

	//surcharge type int
	inline void VBO::AddArrayBufferData(GLuint indexAttrib, const int* data, std::size_t size, GLint vertexSize)
	{
		AddArrayBufferData(indexAttrib, data, sizeof(int)*size, vertexSize, GL_UNSIGNED_INT);
	}
}
#endif //_BH3D_VBO_H_
