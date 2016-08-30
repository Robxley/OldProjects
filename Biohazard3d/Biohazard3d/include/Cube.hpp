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


 
 
 
#ifndef _BH3D_CUBE_H_
#define _BH3D_CUBE_H_

#include "Mesh.hpp"

namespace BH3D
{



	class Cube : public Mesh
	{
		public:

			//add a sub mesh as a cube
			static void AddSubMesh(Mesh * pMesh, glm::vec3 size = glm::vec3(1.0f));

		public:

			//Ajout directement un submesh de type cube à la création
			Cube(glm::vec3);
			Cube(float size = 1.0f);

			inline void setSize(glm::vec3 size);
			inline glm::vec3 getSize() const;

		private:
			inline void Create();
			inline void ComputeBoundingBox();
			glm::vec3 size;
	};

	//inline functions
	inline void Cube::setSize(glm::vec3 _size)
	{
		if (_size == size)
			return;
		size = _size;
		Destroy();
		Create();
	}
	inline glm::vec3 Cube::getSize() const
	{
		return size;
	}
	inline void Cube::Create()
	{
		Cube::AddSubMesh(this, size);
		ComputeMesh();
	}
	inline void Cube::ComputeBoundingBox()
	{
		boundingBox.size = size;
	}

}
#endif //
