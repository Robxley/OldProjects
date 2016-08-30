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


 
#ifndef _BH3D_SPHERE_H_
#define _BH3D_SPHERE_H_

#include <glm/glm.hpp>

#include "Mesh.hpp"

namespace BH3D
{
	class Sphere : public Mesh
	{
	public:
		//add a sphere as a sub mesh to a mesh
		static void AddSubMesh(Mesh * pMesh, float radius = 0.5f, int definition = 12, glm::vec3 scaleAxis = glm::vec3(1.0f), glm::vec2 latitudeBounds = glm::vec2(0.0f, 180.0f), glm::vec2 longitudeBounds = glm::vec2(0.0f, 360.0f));
	public:
		Sphere(float radius = 0.5f, int def = 12, glm::vec3 scaleAxis = glm::vec3(1.0f), glm::vec2 latitudeBounds = glm::vec2(0.0f, 180.0f), glm::vec2 longitudeBounds = glm::vec2(0.0f, 360.0f));
		inline void Create(float radius, int def = 12, glm::vec3 scaleAxis = glm::vec3(1.0f), glm::vec2 latitudeBounds = glm::vec2(0.0f, 180.0f), glm::vec2 longitudeBounds = glm::vec2(0.0f, 360.0f));
		inline float GetRadius();
		inline int GetDefinition();

		inline void ComputeBoundingBox();

	private:

		inline void Create();

		float radius;				//rayon de la sphère
		unsigned int definition;	//definition de la sphère
		glm::vec3 scaleAxis;		//étirement des axes pour faire des ellipses
		glm::vec2 latitudeBounds, longitudeBounds;

	};

	inline float Sphere::GetRadius() { return radius; }
	inline int Sphere::GetDefinition() { return definition; }
	inline void Sphere::Create()
	{
		Sphere::AddSubMesh (this, radius, definition, scaleAxis, latitudeBounds, longitudeBounds);
		ComputeMesh();
	}
	
	inline void Sphere::Create(float _radius, int _def, glm::vec3 _scaleAxis, glm::vec2 _latitudeBounds, glm::vec2 _longitudeBounds)
	{

		radius = _radius;
		definition = _def;
		scaleAxis = _scaleAxis;
		latitudeBounds = _latitudeBounds;
		longitudeBounds = _longitudeBounds;
		Destroy();
		Create();
	}
	inline void Sphere::ComputeBoundingBox() {
		boundingBox.size = glm::vec3(radius, radius, radius);
		boundingBox.position = glm::vec3();
	}
}
#endif //
