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


#include "Mesh.hpp"
#include "Logger.hpp"
#include "Common.hpp"
#include "Assertion.hpp"
#include <algorithm>

namespace BH3D
{



	Mesh::~Mesh()
	{
		Destroy();
	}

	bool Mesh::AddSubMesh(std::size_t _nFaces, const unsigned int *_ptFaces, std::size_t _nVertices, const float * _ptPositions, const float * _ptTexCoords, char _textureFormat, const float * _ptNormals, const float *_ptColors, char _colorFormat, const Material *_pMaterial)
	{

		if (!_nFaces || _ptFaces == nullptr || !_nVertices || _ptPositions == nullptr)
		{
			BH3D_LOGGERERROR("invalid param");
			return BH3D_ERROR;
		}

		boundingBox.Reset();
		computed = 0;

		if (!LoadSubMesh(_nFaces, _ptFaces,  _nVertices, _ptPositions, _ptTexCoords,  _textureFormat,  _ptNormals, _ptColors, _colorFormat, _pMaterial))
		{
			BH3D_LOGGERERROR("Fucking big FAIL !!!! Can destroy your computer");
			return BH3D_ERROR;
		}

		return BH3D_OK;
	}
	bool Mesh::AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec2> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec3> & tColor, const Material *_pMaterial)
	{
		std::size_t nFaces = tFace.size();
		const unsigned int *ptFaces = (tFace.empty() ? nullptr : tFace[0].id);

		std::size_t nVertex = tPosition.size();
		const float * ptPosition = tPosition.empty() ? nullptr : &tPosition[0].x;
		const float * ptTexCoord = tTexCoord.empty() ? nullptr : &tTexCoord[0].x;
		const float * ptNormal = tNormal.empty() ? nullptr : &tNormal[0].x;
		const float * ptColor = tColor.empty() ? nullptr : &tColor[0].x;
		return AddSubMesh(nFaces, ptFaces, nVertex, ptPosition, ptTexCoord, 2, ptNormal, ptColor, 0, _pMaterial);
	}
	bool Mesh::AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec3> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec3> & tColor, const Material *_pMaterial)
	{
		std::size_t nFaces = tFace.size();
		const unsigned int *ptFaces = (tFace.empty() ? nullptr : tFace[0].id);

		std::size_t nVertex = tPosition.size();
		const float * ptPosition = tPosition.empty() ? nullptr : &tPosition[0].x;
		const float * ptTexCoord = tTexCoord.empty() ? nullptr : &tTexCoord[0].x;
		const float * ptNormal = tNormal.empty() ? nullptr : &tNormal[0].x;
		const float * ptColor = tColor.empty() ? nullptr : &tColor[0].x;
		return AddSubMesh(nFaces, ptFaces, nVertex, ptPosition, ptTexCoord, 3, ptNormal, ptColor, 0, _pMaterial);
	}

	bool Mesh::AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec2> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec4> & tColor, const Material *_pMaterial)
	{
		std::size_t nFaces = tFace.size();
		const unsigned int *ptFaces = (tFace.empty() ? nullptr : tFace[0].id);

		std::size_t nVertex = tPosition.size();
		const float * ptPosition = tPosition.empty() ? nullptr : &tPosition[0].x;
		const float * ptTexCoord = tTexCoord.empty() ? nullptr : &tTexCoord[0].x;
		const float * ptNormal = tNormal.empty() ? nullptr : &tNormal[0].x;
		const float * ptColor = tColor.empty() ? nullptr : &tColor[0].x;
		return AddSubMesh(nFaces, ptFaces, nVertex, ptPosition, ptTexCoord, 2, ptNormal, ptColor, 1, _pMaterial);
	}
	bool Mesh::AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec3> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec4> & tColor, const Material *_pMaterial)
	{
		std::size_t nFaces = tFace.size();
		const unsigned int *ptFaces = (tFace.empty() ? nullptr : tFace[0].id);

		std::size_t nVertex = tPosition.size();
		const float * ptPosition = tPosition.empty() ? nullptr : &tPosition[0].x;
		const float * ptTexCoord = tTexCoord.empty() ? nullptr : &tTexCoord[0].x;
		const float * ptNormal = tNormal.empty() ? nullptr : &tNormal[0].x;
		const float * ptColor = tColor.empty() ? nullptr : &tColor[0].x;
		return AddSubMesh(nFaces, ptFaces, nVertex, ptPosition, ptTexCoord, 3, ptNormal, ptColor, 1, _pMaterial);
	}
	bool Mesh::AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec2> & tTexCoord, std::vector<glm::vec3> & tNormal)
	{
		std::size_t nFaces = tFace.size();
		const unsigned int *ptFaces = (tFace.empty() ? nullptr : tFace[0].id);

		std::size_t nVertex = tPosition.size();
		const float * ptPosition = tPosition.empty() ? nullptr : &tPosition[0].x;
		const float * ptTexCoord = tTexCoord.empty() ? nullptr : &tTexCoord[0].x;
		const float * ptNormal = tNormal.empty() ? nullptr : &tNormal[0].x;
		return AddSubMesh(nFaces, ptFaces, nVertex, ptPosition, ptTexCoord, 2, ptNormal, nullptr, 0, nullptr);
	}

	void Mesh::FreeArrays()
	{
		//clear ne libère pas la mémoire ;D
		//tPosition.clear();
		//tNormal.clear();
		//tTexCoord2.clear();
		//tTexCoord3.clear();
		//tColor3.clear();
		//tColor4.clear();
		//tTangent.clear();

		//libère vraiment la mémoire.
		std::vector<glm::vec3>().swap(tPosition);
		std::vector<glm::vec3>().swap(tNormal);
		std::vector<glm::vec2>().swap(tTexCoord2);
		std::vector<glm::vec3>().swap(tTexCoord3);
		std::vector<glm::vec3>().swap(tColor3);
		std::vector<glm::vec4>().swap(tColor4);
		std::vector<glm::vec3>().swap(tTangent);

		reserveFaceNumber = 0;
		reserveVertexNumber = 0;
		reserveMeshNumber = 0;

	}


	bool Mesh::ComputeMesh()
	{
		//already computed
		if (IsValid())
		{
			BH3D_LOGGERWARNING("Mesh is already computed")
			return BH3D_OK;
		}

		if (tPosition.empty())
		{
			BH3D_LOGGERERROR("No vertices to compute - Sucker");
			BH3D_ASSERT(0);
			return BH3D_ERROR;
		}
		
		ComputeBoundingBox();
		//
		//construction du vbo
		//

		vbo.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, tPosition);

		if (tNormal.size())
			vbo.AddArrayBufferData(BH3D_NORMAL_ATTRIB_INDEX, tNormal);

		if (tTexCoord2.size())
			vbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tTexCoord2);
		else if (tTexCoord3.size())
			vbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tTexCoord3);

		if (tColor3.size())
			vbo.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, tColor3);

		else if (tColor4.size())
			vbo.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, tColor4);

		if (tTangent.size())
			vbo.AddArrayBufferData(BH3D_DATA0_ATTRIB_INDEX, tTangent);

		vbo.AddElementBufferData(tFace[0].id, tFace.size() * 3);
		
		if (vbo.Create())
		{
			computed = BH3D_OK;
			return BH3D_OK;
		}
		else
		{
			BH3D_LOGGERERROR("VBO Fail");
		}

		return BH3D_ERROR;
	}




	void Mesh::Destroy()
	{

		vbo.Destroy();

		tSubMeshes.clear();
		computed = 0;

		tPosition.clear();
		tNormal.clear();
		tTexCoord2.clear();
		tTexCoord3.clear();
		tColor3.clear();
		tColor4.clear();
		tTangent.clear();

		reserveFaceNumber = 0;
		reserveVertexNumber = 0;
		reserveMeshNumber = 0;
	

	}


	bool Mesh::LoadSubMesh(std::size_t _nFaces, const unsigned int *_ptFaces, std::size_t _nVertices, const float * _ptPositions, const float * _ptTexCoords, char _textureFormat, const float * _ptNormals, const float *_ptColors, char _colorFormat, const Material *_pMaterial)
	{
		//les données de chaque groupe doivent avoir le même format de vertex (4,3,2...)
		if (tSubMeshes.size())
		{
			if (textureFormat != _textureFormat)
			{
				BH3D_LOGGERERROR("textureFormat mismatch");
				BH3D_ASSERT(0);
				return BH3D_ERROR; //coordonnées textures : format différent
			}

			if (colorFormat != _colorFormat)
			{

				BH3D_LOGGERERROR("colorFormat mismatch");
				BH3D_ASSERT(0);
				return BH3D_ERROR;//couleurs : format différent
			}
		}
		else		//premier SubMesh on sauvegarde le format des coordonnées textures et couleurs
		{
			textureFormat = _textureFormat;
			colorFormat = _colorFormat;
		}
		
		if (tNormal.size() && _ptNormals == nullptr)
		{
			BH3D_LOGGERERROR("New mesh description not match with existing meshes : normal missing");
			BH3D_ASSERT(0);
			return BH3D_ERROR;//couleurs : format différent
		}
		if ((tTexCoord2.size() || tTexCoord3.size()) && _ptTexCoords == nullptr)
		{
			BH3D_LOGGERERROR("New mesh description not match with existing meshes : texture Coordonates missing");
			BH3D_ASSERT(0);
			return BH3D_ERROR;//couleurs : format différent
		}
		if ((tColor4.size() || tColor3.size()) && _ptColors == nullptr)
		{
			BH3D_LOGGERERROR("New mesh description not match with existing meshes : color missing");
			BH3D_ASSERT(0);
			return BH3D_ERROR;//couleurs : format différent
		}

		std::size_t positionOffset = tPosition.size();
		std::size_t offsetFace = tFace.size();

		//allocation de la mémoire
		if (reserveFaceNumber && tSubMeshes.empty())
			tFace.reserve(reserveFaceNumber);
		else if (reserveFaceNumber == 0)
			tFace.reserve(offsetFace + _nFaces);

		if (reserveVertexNumber && tSubMeshes.empty())
		{
			tPosition.reserve(reserveVertexNumber);
			if (_ptNormals != nullptr)
				tNormal.reserve(reserveVertexNumber);
			if (_ptTexCoords != nullptr)
			{
				if (textureFormat == 3)
					tTexCoord3.reserve(reserveVertexNumber);
				else
					tTexCoord2.reserve(reserveVertexNumber);
			}
			if (_ptColors != nullptr)
			{
				if (colorFormat == 4)
					tColor4.reserve(reserveVertexNumber);
				else
					tColor3.reserve(reserveVertexNumber);
			}
		}
		else if (reserveVertexNumber == 0)
		{
			tPosition.reserve(positionOffset + _nVertices);
			if (_ptNormals != nullptr)
				tNormal.reserve(positionOffset + _nVertices);
			if (_ptTexCoords != nullptr)
			{
				if (textureFormat == 3)
					tTexCoord3.reserve(positionOffset + _nVertices);
				else
					tTexCoord2.reserve(positionOffset + _nVertices);
			}
			if (_ptColors != nullptr)
			{
				if (colorFormat == 4)
					tColor4.reserve(positionOffset + _nVertices);
				else
					tColor3.reserve(positionOffset + _nVertices);
			}
		}

		if (reserveMeshNumber && tSubMeshes.empty())
			tSubMeshes.reserve(reserveMeshNumber);
		
		tSubMeshes.push_back(Mesh::SubMesh());
		auto & subMesh = tSubMeshes.back();

		subMesh.vertexOffset = positionOffset;
		subMesh.faceOffset = offsetFace;
		subMesh.nFaces = _nFaces;
		subMesh.nVertices = _nVertices;

		//recopie des vertices
		for (unsigned int i = 0; i < _nVertices; i++)
		{
			tPosition.push_back(glm::vec3(_ptPositions[i * 3], _ptPositions[i * 3 + 1], _ptPositions[i * 3 + 2]));
			if (_ptNormals != nullptr)
				tNormal.push_back(glm::vec3(_ptNormals[i * 3], _ptNormals[i * 3 + 1], _ptNormals[i * 3 + 2]));

			if (_ptTexCoords != nullptr)
			{
				if (textureFormat == 3)
					tTexCoord3.push_back(glm::vec3(_ptTexCoords[i * 3], _ptTexCoords[i * 3 + 1], _ptTexCoords[i * 3 + 2]));
				else
					tTexCoord2.push_back(glm::vec2(_ptTexCoords[i * 2], _ptTexCoords[i * 2 + 1]));

			}
			if (_ptColors != nullptr)
			{
				if (colorFormat == 4)
					tColor4.push_back(glm::vec4(_ptColors[i * 4], _ptColors[i * 4 + 1], _ptColors[i * 4 + 2], _ptColors[i * 4 + 3]));
				else
					tColor3.push_back(glm::vec3(_ptColors[i * 3], _ptColors[i * 3 + 1], _ptColors[i * 3 + 2]));
			}
		}
		
		Face face;
		for (unsigned int i = 0; i < _nFaces; i++)
		{
			face.id[0] = _ptFaces[i * 3 + 0] + positionOffset;
			face.id[1] = _ptFaces[i * 3 + 1] + positionOffset;
			face.id[2] = _ptFaces[i * 3 + 2] + positionOffset;
			tFace.push_back(face);
		}
		if (_pMaterial)
			subMesh.nMaterial = (*_pMaterial);

		return BH3D_OK;
	}

	void Mesh::ScaleMesh(glm::vec3 scale, int submeshid)
	{
		BH3D_ASSERT_MSG(tPosition.size(),"Empty Mesh (no vertex)",);

		computed = 0;
		boundingBox.Reset();

		if (scale == glm::vec3(0.0f, 0.0f, 0.0f) || scale == glm::vec3(1.0f, 1.0f, 1.0f)) return;

		std::size_t i, start = 0, end = tPosition.size();

		if (submeshid >= 0 && submeshid < tSubMeshes.size())
		{
			start = tSubMeshes[submeshid].vertexOffset;
			end = start + tSubMeshes[submeshid].nVertices;
		}

		for (i = start; i < end; i++)
		{
			tPosition[i].x *= scale.x;
			tPosition[i].y *= scale.y;
			tPosition[i].z *= scale.z;
		}

		boundingBox.Reset();
	}
	void Mesh::Draw() const
	{

		BH3D_ASSERT_MSG(IsValid(),"No valid Mesh, can't draw it",);
		vbo.Enable();
		//glDrawElements(GL_TRIANGLES, tFace.size()*3, GL_UNSIGNED_INT, 0);

		for (unsigned int i = 0; i < tSubMeshes.size(); i++)
		{

			tSubMeshes[i].nMaterial.Bind();

			glDrawElements(GL_TRIANGLES, (GLsizei)tSubMeshes[i].nFaces * 3, GL_UNSIGNED_INT, BH3D_BUFFER_OFFSET(tSubMeshes[i].faceOffset * 3 * sizeof(unsigned int)));
			
		}
#ifndef NDEBUG
		vbo.Disable();
#endif

	}

	void Mesh::DrawSubMesh(unsigned int id) const
	{

		BH3D_ASSERT_MSG(IsValid(), "No valid Mesh, can't draw it",);

		vbo.Enable();
		tSubMeshes[id].nMaterial.Bind();

		glDrawElements(GL_TRIANGLES, (GLsizei)tSubMeshes[id].nFaces * 3, GL_UNSIGNED_INT, BH3D_BUFFER_OFFSET( tSubMeshes[id].faceOffset * 3 * sizeof(unsigned int)) );

#ifndef NDEBUG
		vbo.Disable();
#endif

	}

	void Mesh::ComputeBoundingBox()
	{
		BH3D_ASSERT_MSG(tPosition.size(),"Mesh is empty, (No vertices)",);

		if (boundingBox.IsValid()) return;

		glm::vec3 vmin, vmax;
		vmin = vmax = tPosition[0]; //initialisation
		for (const auto & v : tPosition)
		{
			for (int j = 0; j < 3; j++) //pour chacune des composantes
			{
				if (vmin[j] > v[j]) vmin[j] = v[j];

				if (vmax[j] < v[j]) vmax[j] = v[j];
			}
		}

		boundingBox.size = (vmax - vmin);
		boundingBox.position = 0.5f*(vmax + vmin);

	}

	void Mesh::CenterDataToOrigin(glm::vec3 offset)
	{
		BH3D_ASSERT_MSG(tPosition.size(),"Mesh is empty (no vertices)",);
		if (!boundingBox.IsValid())
			ComputeBoundingBox();

		glm::vec3 origine = boundingBox.position - offset;
		if (boundingBox.position != glm::vec3(0, 0, 0))
		{
			for (auto & v : tPosition)
			{
				v -= (origine);
			}
			boundingBox.position = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		//boundingBox.size = glm::vec3(0.0,0.0,0.0);
	}

	void Mesh::NormalizeData(bool keepRatio)
	{

		BH3D_ASSERT_MSG(tPosition.size(), "Mesh is empty (no vertices)",);
		if (!boundingBox.IsValid())
			ComputeBoundingBox();

		float dx = 1.0f, dy = 1.0f, dz = 1.0f;

		if (boundingBox.size.x)
		{
			dx = 1.0f / boundingBox.size.x;
			boundingBox.size.x = 1.0f;
		}
		if (boundingBox.size.y)
		{
			dy = 1.0f / boundingBox.size.y;
			boundingBox.size.y = 1.0f;
		}
		if (boundingBox.size.z)
		{
			dz = 1.0f / boundingBox.size.z;
			boundingBox.size.z = 1.0f;
		}


		if (keepRatio)
		{
			dx = dy = dz = std::min(std::min(dx, dy), dz);
		}

		ScaleMesh(glm::vec3(dx,dy,dz));

		boundingBox.size = glm::vec3(0.0, 0.0, 0.0);
	}

	void Mesh::TransformMesh(glm::mat4 &mat, int submeshid)
	{
		BH3D_ASSERT_MSG(tPosition.size(), "Mesh is empty (no vertices)",);

		computed = 0;
		boundingBox.Reset();

		std::size_t i, start = 0, end = tPosition.size();

		if (submeshid >= 0 && submeshid < tSubMeshes.size())
		{
			start = tSubMeshes[submeshid].vertexOffset;
			end = start + tSubMeshes[submeshid].nVertices;
		}

		for (i = start; i < end; i++)
			tPosition[i] = glm::vec3(mat*glm::vec4(tPosition[i], 1));

		if (tNormal.size())
		{
			for (i = start; i < end; i++)
				tNormal[i] = glm::vec3(mat*glm::vec4(tNormal[i], 1));
		}

		if (tTangent.size())
		{
			for (i = start; i < end; i++)
				tTangent[i] = glm::vec3(mat*glm::vec4(tTangent[i], 1));
		}


	}

}