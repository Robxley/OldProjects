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




#ifndef _BH3D_MESH_H_
#define _BH3D_MESH_H_

#include "VBO.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Face.hpp"
#include "BoundingBox.hpp"

namespace BH3D
{

	/*!
	\author 	Robxley (A.CAILLY)
	\version	0.1
	\date		08/12/2014
	\~english
	\brief		This class is used to define a classic Mesh with a triangular face description. (vertex positions, normals, colors ...)
	\remark		It is a static mesh corresponding to bind pose.
	\~french
	\brief		Cette classe definie un mesh classique à face triangulaire.
	\remark		Le mesh est un mesh static.
	*/
	class Mesh
	{
		public:
			typedef struct _SubMesh
			{
				Material			 nMaterial;	//material attaché au subMesh
				std::size_t faceOffset = 0;	//offset sur le element array pour les indices des faces
				std::size_t vertexOffset = 0;	//offset sur les vertex arrays pour le mesh concerné
				std::size_t nFaces = 0;		//nombre de faces du mesh
				std::size_t nVertices = 0;		//nombre de vertices du mesh
			} SubMesh;
		public:

			~Mesh();

			/**
			*\~english
			*\brief		Adds a submesh to the mesh. All submeshes have to be with the same format for the texture and color vertex.
			*\param[in]	number of faces. (It's not the size of _ptFaces array. size = _nFaces*3)
			*\param[in] pointer on the array of faces.
			*\param[in]	number of vertices (It's not the size of vertices array)
			*\param[in] pointer on the array of positions.
			*\param[in] pointer on the array of texture coordinates. Can be nullptr.
			*\param[in] Format of the texture coordinates. Can be 2 or 3, other value will be set to 2.
			*\param[in] pointer on the array of normals. Can be nullptr.
			*\param[in] pointer on the array of colors. Can be nullptr.
			*\param[in] Format of the color. Can be 3 or 4, other value will be set to 3.
			*\param[in] Material of the submesh. Can be nullptr.
			*\return	returns BH3D_OK or BH3D_ERROR if the texture and color formats aren't different with the other submeshes.
			*\remark	The mesh become invalid after the call of the function. You have to call the ComputeMesh function to turn it valid.
			*\~french
			*\brief		Ajoute un sous-mesh au mesh. Tous les sous-meshes doivent avoir le meme format pour les textures, couleurs, etc...
			*\param[in]	Nombre de faces. (Ce n'est la taille du tableau des indices de face)
			*\param[in] Pointeur sur le tableau des faces.
			*\param[in]	Nombre de vertices. (ce n'est pas la taille du tableau de vertex mais 3 fois moins)
			*\param[in] Pointeur sur le tableau des positions.
			*\param[in] Pointeur sur le tableau des coordonnées textures. Peut être nullptr.
			*\param[in] Format des coordonnées textures. Peut être 2 ou 3. Les autres valeurs seront forcées à 2.
			*\param[in] Pointeur sur le tableau des normales.  Peut être nullptr.
			*\param[in] Pointeur sur le tableaux des couleurs. Peut être nullptr.
			*\param[in] Format des couleurs. Peut être 3 ou 4. Les autres valeurs seront forcées à 3.
			*\param[in] Material of the submesh
			*\return	Retourne BH3D_OK ou BH3D_ERROR si le format des couleurs et textures ne correspondent pas avec ceux des autres meshes.
			*\remark	L'ajout d'un submesh rend le mesh invalide. Il est nécessaire d'appeler la fonction ComputeMesh pour le rendre à nouveau valide.
			*/
			virtual bool AddSubMesh(std::size_t nFaces, const unsigned int *ptFaces, std::size_t nVertices, const float * ptPositions, const float * ptTexCoords = nullptr, char textureFormat = 2, const float * ptNormals = nullptr, const float *ptColors = nullptr, char colorFormat = 0, const Material *pMaterial = nullptr);
			bool AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec2> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec3> & tColor, const Material *pMaterial = nullptr);
			bool AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec3> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec3> & tColor, const Material *pMaterial = nullptr);

			bool AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec2> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec4> & tColor, const Material *pMaterial = nullptr);
			bool AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec3> & tTexCoord, std::vector<glm::vec3> & tNormal, const std::vector<glm::vec4> & tColor, const Material *pMaterial = nullptr);

			bool AddSubMesh(const std::vector<Face> & tFace, const std::vector<glm::vec3> & tPosition, const std::vector<glm::vec2> & tTexCoord, std::vector<glm::vec3> & tNormal);


			/**
			*\~english
			*\brief		Computes a valid mesh and his VBO.
			*\return	0 or 1 . Can fail if there is not enough memory
			*\~french
			*\brief		Calcule un mesh valide et son VBO.
			*\return	0 or 1 . Peut échouer si pas assez de mémoire
			*/
			virtual bool ComputeMesh();

			/**
			*\~english
			*\brief		Releases the array memory in the "CPU side". The VBO on GPU are not destroyed.
			*\remark	Can be called after ComputeMesh function, if you know  you don't need to add more submesh.
			*\~french
			*\brief		Libère la mémoire des tableaux de stockage des vertex "coté cpu". Le VBO sur le GPU n'est pas détruit.
			*\remark	Peut-être utilisé après la fonction ComputeMesh s'il n'y a plus besoin de rajouter des sous meshes par la suite.
			*/
			virtual void FreeArrays();

			/**
			*\~english
			*\brief		Checks if the mesh is valid. (The mesh becomes valid after the call of the function ComputeMesh.)
			*\return	Returns BH3D_OK (true) or BH3D_ERROR (false).
			*\~french
			*\brief		Si le mesh est valide. (Le mesh devient valide après l'appel de la fonction ComputeMesh.)
			*/
			inline virtual bool IsValid() const;

			/**
			*\~english
			*\brief		Destroys the mesh ( submeshes, vbo...) to restart with a new mesh. (GPU and CPU data)
			*\~french
			*\brief		Détruit le mesh pour recommencer avec un nouveau mesh. (données GPU et CPU)
			*/
			virtual void Destroy();

			/**
			*\~english
			*\brief		Draws the mesh in the current shader.
			*\~french
			*\brief		Dessine le mesh dans le shader courant.
			*/
			virtual void Draw() const;

			/**
			*\~english
			*\brief		Draws the mesh in the given shader.
			*\param[in]	The shader used to draw the mesh
			*\~french
			*\brief		Dessine le mesh dans le shader donné en paramétre.
			*\param[in] Le shader utilisé pour dessiner le mesh.
			*/
			//virtual void Draw(const Shader * shader);

			/**
			*\~english
			*\brief		Draws a submesh in the current shader.
			*\param[in]	Submesh id.
			*\~french
			*\brief		Dessine le submesh dans le shader courant.
			*\param[in] Id du submesh
			*/
			virtual void DrawSubMesh(unsigned int id = 0) const;

			/**
			*\~english
			*\brief		Draws a submesh in the given shader.
			*\param[in]	Submesh id.
			*\param[in]	The shader used to draw the mesh
			*\~french
			*\brief		Dessine le submesh dans le shader donné en paramétre.
			*\param[in] Id du submesh
			*\param[in]	Le shader utilisé pour dessiner le submesh.
			*/
			//virtual void DrawSubMesh(unsigned int id, const Shader * shader);


			/**
			*\~english
			*\brief		Transforms the mesh or a submesh.
			*\param[in]	Transform matrix.
			*\param[in]	Id of submesh. negative value to apply on all submeshes.
			*\remark	The mesh become invalid.
			*\~french
			*\brief		Pour transformer un mesh.
			*\param[in] Matrice de transformation.
			*\param[in]	Si inférieur à 0 application à l'ensemble des submeshes.
			*\remark	Le mesh devient invalide.
			*/
			void TransformMesh(glm::mat4 &mat, int submeshid = -1);

			/**
			*\~english
			*\brief		Works like TransformMesh to scale the mesh
			*\param[in]	scale vector.
			*\param[in]	id of submesh
			*\remark	The mesh become invalid.
			*\~french
			*\brief		Marche comme pour TransformMesh
			*\param[in] vecteur de redimentionnement.
			*\param[in]	Si inférieur à 0 application à l'ensemble des submesh.
			*\remark	Le mesh devient invalide.
			*/
			void ScaleMesh(glm::vec3 scale, int submeshid = -1);
			void ScaleMesh(float scale, int submeshid = -1);

			/**
			*\~english
			*\brief		To Center the Mesh on the origin with a offset.
			*\param[in]	offset position
			*\remark	The mesh become invalid.
			*\~french
			*\brief		Pour centrer le mesh sur l'origine avec un offset.
			*\param[in] vecteur offset
			*\remark	Le mesh devient invalide.
			*/
			void CenterDataToOrigin(glm::vec3 offset = glm::vec3(0));

			/**
			*\~english
			*\brief		Scales the mesh in box with the size (1,1,1)
			*\param[in]	keep the ratio on x,y,z size
			*\remark	The mesh become invalid.
			*\~french
			*\brief		Redimensionne le mesh pour être contenu dans un cube de dimension (1,1,1)
			*\param[in] ratio entre les différentes dimensions.
			*\remark	Le mesh devient invalide.
			*/
			void NormalizeData(bool keepRatio = 1);


			/**
			*\~english
			*\brief		Requests that the vertex capacity be at least enough to contain n meshes.
			*\param[in]	face number for all submeshes.
			*\param[in]	vertex number for all submeshes.
			*\remark	Container allocation.
			*\~french
			*\brief		Permets de définir la taille des containers mémoires.
			*\param[in] Nombre de faces qui sera utilisé par l'ensemble des submeshes
			*\param[in] Nombre de vertices qui sera utilisé par l'ensemble des submeshes
			*\remark	Permets d'éviter l'allocation/désallocations des containers à chaque ajouts de mesh
			*/
			inline void ReserveMemory(unsigned int faceNumber, unsigned int vertexNumber, unsigned int meshNumber = 0);


			//Accesseur/Getter
			inline std::vector<glm::vec3>&	 GetTabPosition();
			inline std::vector<glm::vec3>&	 GetTabNormal();
			inline std::vector<glm::vec2>&	 GetTabTexCoord2();
			inline std::vector<glm::vec3>&	 GetTabTexCoord3();
			inline std::vector<glm::vec3>&  GetTabColor3();
			inline std::vector<glm::vec4>&  GetTabColor4();
			inline std::vector<glm::vec3>&  GetTabTangent();
			inline std::vector<Face>&  GetTabFace();
			inline std::vector<Mesh::SubMesh>&  GetTabSubMeshes();

			//applique un meme et unique material à tous les submeshes
			inline void SetMaterial(const Material & m);

			//application a un seul mesh
			inline void SetMaterialSubMesh(unsigned int submeshid, const Material &m);

			//idem mais pour une texture
			inline void SetTexture(const Texture &t);
			inline void SetTexture(const Texture * t);
			inline void SetTexture(const char * filename);
			inline void SetTextureSubMesh(unsigned int submeshid, const Texture & texture);
			inline void SetTextureSubMesh(unsigned int submeshid, const char * filename);

			//idem pour la couleur
			inline void SetColor(const glm::vec4 & color);
			inline void SetColorSubMesh(unsigned int submeshid, const glm::vec4 & color);

			//retourne nullptr si indice invalide
			inline Material * GetSubMeshMaterial(unsigned int submeshid);

		protected:

			bool LoadSubMesh(std::size_t _nFaces, const unsigned int *_ptFaces, std::size_t _nVertices, const float * _ptPositions, const float * _ptTexCoords = nullptr, char _textureFormat = 2, const float * _ptNormals = nullptr, const float *_ptColors = nullptr, char _colorFormat = 0, const Material *_pMaterial = nullptr);

			virtual void ComputeBoundingBox();

		protected:


			std::vector<SubMesh>		tSubMeshes;

			VBO	vbo;			  //vbo utiliser par le mesh

			bool computed = 0;

			unsigned int reserveFaceNumber = 0, reserveVertexNumber = 0, reserveMeshNumber = 0;
			
			std::vector<glm::vec3>	tPosition;
			std::vector<glm::vec3>	tNormal;
			std::vector<glm::vec2>	tTexCoord2;
			std::vector<glm::vec3>	tTexCoord3;
			std::vector<glm::vec3>  tColor3;
			std::vector<glm::vec4>  tColor4;
			std::vector<glm::vec3>  tTangent;
			std::vector<Face>	 tFace;		

			char textureFormat = 2; //2 or 3
			char colorFormat = 3;	//3 or 4

			BoundingBox boundingBox;

	};

	//Inline functions

	inline bool Mesh::IsValid() const
	{
		return (vbo.IsValid() && computed);
	}

	inline std::vector<glm::vec3>&	Mesh::GetTabPosition()
	{
		return tPosition;
	}

	inline std::vector<glm::vec3>&	Mesh::GetTabNormal()
	{
		return tNormal;
	}

	inline std::vector<glm::vec2>&	Mesh::GetTabTexCoord2()
	{
		return tTexCoord2;
	}

	inline std::vector<glm::vec3>&	Mesh::GetTabTexCoord3()
	{
		return tTexCoord3;
	}

	inline std::vector<glm::vec3>&  Mesh::GetTabColor3()
	{
		return tColor3;
	}

	inline std::vector<glm::vec4>&  Mesh::GetTabColor4()
	{
		return tColor4;
	}

	inline std::vector<glm::vec3>&  Mesh::GetTabTangent()
	{
		return tTangent;
	}

	inline std::vector<Face>&  Mesh::GetTabFace()
	{
		return tFace;
	}

	inline std::vector<Mesh::SubMesh>&  Mesh::GetTabSubMeshes()
	{
		return tSubMeshes;
	}

	inline void Mesh::ScaleMesh(float scale, int submeshid)
	{
		ScaleMesh(glm::vec3(scale), submeshid);
	}

	inline void Mesh::SetMaterial(const Material &m)
	{
		for (auto & subMesh : tSubMeshes)
			subMesh.nMaterial = m;
	}

	inline void Mesh::SetTexture(const Texture &t)
	{
		for (auto & subMesh : tSubMeshes)
			subMesh.nMaterial.SetColorMap(t);
	}

	inline void Mesh::SetTexture(const Texture * t)
	{
		BH3D_ASSERT_MSG(t, "nullptr Pointer", );
		if (t != nullptr)
			for (auto & subMesh : tSubMeshes)
				subMesh.nMaterial.SetColorMap(*t);
	}

	inline void Mesh::SetTexture(const char * filename)
	{
		Texture * texture = BH3D_LoadTexture(filename);
		if (texture != nullptr)
			for (auto & subMesh : tSubMeshes)
				subMesh.nMaterial.SetColorMap(*texture);
	}
	
	inline void Mesh::SetTextureSubMesh(unsigned int submeshid, const Texture & texture)
	{
		if (submeshid < tSubMeshes.size())
		{
			tSubMeshes[submeshid].nMaterial.SetColorMap(texture);
		}
	}

	inline void Mesh::SetTextureSubMesh(unsigned int submeshid, const char * filename)
	{
		Texture * texture = BH3D_LoadTexture(filename);
		if (texture != nullptr)
			if (submeshid < tSubMeshes.size())
			{
				tSubMeshes[submeshid].nMaterial.SetColorMap(*texture);
			}
	}
	inline void Mesh::SetMaterialSubMesh(unsigned int submeshid, const Material &m)
	{
		if (submeshid < tSubMeshes.size())
			tSubMeshes[submeshid].nMaterial = m;
	}
	inline void Mesh::SetColor(const glm::vec4 & color)
	{
		for (auto & subMesh : tSubMeshes)
			subMesh.nMaterial.color = color;
	}
	inline void Mesh::SetColorSubMesh(unsigned int submeshid, const glm::vec4 & color)
	{
		if (submeshid < tSubMeshes.size())
			tSubMeshes[submeshid].nMaterial.color = color;
	}

	inline Material * Mesh::GetSubMeshMaterial(unsigned int submeshid)
	{
		if (submeshid < tSubMeshes.size())
			return (&tSubMeshes[submeshid].nMaterial);
		else
			return nullptr;
	}

	inline void Mesh::ReserveMemory(unsigned int faceNumber, unsigned int vertexNumber, unsigned int meshNumber)
	{
		reserveFaceNumber = faceNumber; reserveVertexNumber = vertexNumber, reserveMeshNumber = meshNumber;
	}
}
#endif //_BH3D_MESH_H_
