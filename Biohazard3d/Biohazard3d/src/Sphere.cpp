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

#include "Sphere.hpp"
#include "StuffMath.hpp"

namespace BH3D
{

	void Sphere::AddSubMesh(Mesh * pMesh, float radius, int definition, glm::vec3 scaleAxis, glm::vec2 latitudeBounds, glm::vec2 longitudeBounds)
	{
		if (pMesh == nullptr) return;

		float a;
		float b;

		glm::vec3 tmp3;
		glm::vec2 tmp2;

		std::vector<glm::vec3>  tPosition;
		std::vector<glm::vec2>  tTexCoord2;
		std::vector<glm::vec3>  tNormal;

		for (b = latitudeBounds[0]; b <= latitudeBounds[1] - definition; b += definition)
		{
			for (a = longitudeBounds[0]; a <= longitudeBounds[1] - definition; a += definition)
			{

				tmp3.x = radius * sin((a) / 180 * BH3D_PI) * sin((b) / 180 * BH3D_PI) * scaleAxis.x;
				tmp3.z = radius * cos((a) / 180 * BH3D_PI) * sin((b) / 180 * BH3D_PI) * scaleAxis.y;
				tmp3.y = radius * cos((b) / 180 * BH3D_PI) * scaleAxis.z;
				tmp2.y = (2 * b) / 360;
				tmp2.x = (a) / 360;
				tPosition.push_back(tmp3);
				tTexCoord2.push_back(tmp2);
				tmp3 = glm::normalize(tmp3);
				tNormal.push_back(tmp3);


				tmp3.x = radius * sin((a) / 180 * BH3D_PI) * sin((b + definition) / 180 * BH3D_PI)* scaleAxis.x;
				tmp3.z = radius * cos((a) / 180 * BH3D_PI) * sin((b + definition) / 180 * BH3D_PI)* scaleAxis.y;
				tmp3.y = radius * cos((b + definition) / 180 * BH3D_PI)* scaleAxis.z;
				tmp2.y = (2 * (b + definition)) / 360;
				tmp2.x = (a) / 360;
				tPosition.push_back(tmp3);
				tTexCoord2.push_back(tmp2);
				tmp3 = glm::normalize(tmp3);
				tNormal.push_back(tmp3);

				tmp3.x = radius * sin((a + definition) / 180 * BH3D_PI) * sin((b) / 180 * BH3D_PI)* scaleAxis.x;
				tmp3.z = radius * cos((a + definition) / 180 * BH3D_PI) * sin((b) / 180 * BH3D_PI)* scaleAxis.y;
				tmp3.y = radius * cos((b) / 180 * BH3D_PI)* scaleAxis.z;
				tmp2.y = (2 * b) / 360;
				tmp2.x = (a + definition) / 360;
				tPosition.push_back(tmp3);
				tTexCoord2.push_back(tmp2);
				tmp3 = glm::normalize(tmp3);
				tNormal.push_back(tmp3);


				tmp3.x = radius * sin((a + definition) / 180 * BH3D_PI) * sin((b + definition) / 180 * BH3D_PI)* scaleAxis.x;
				tmp3.z = radius * cos((a + definition) / 180 * BH3D_PI) * sin((b + definition) / 180 * BH3D_PI)* scaleAxis.y;
				tmp3.y = radius * cos((b + definition) / 180 * BH3D_PI) * scaleAxis.z;
				tmp2.y = (2 * (b + definition)) / 360;
				tmp2.x = (a + definition) / 360;
				tPosition.push_back(tmp3);
				tTexCoord2.push_back(tmp2);
				tmp3 = glm::normalize(tmp3);
				tNormal.push_back(tmp3);
			}
		}


		std::vector<Face> tFace;
		tFace.reserve(tPosition.size() - 1);

		unsigned int i;
		Face f;
		for (i = 0; i < tPosition.size() - 2; i++)
		{
			if (!(i % 2)) //paire
				f.set(i, i + 1, i + 2);
			else		//impaire
				f.set(i, i + 2, i + 1);

			tFace.push_back(f);
		}

		pMesh->AddSubMesh(tFace, tPosition, tTexCoord2, tNormal);
	}


	Sphere::Sphere(float _radius, int _def, glm::vec3 _scaleAxis, glm::vec2 _latitudeBounds, glm::vec2 _longitudeBounds)
		: radius(_radius), definition(_def), scaleAxis(_scaleAxis),
		latitudeBounds(_latitudeBounds), longitudeBounds(_longitudeBounds)
	{
		Create();
	}


}
 
 