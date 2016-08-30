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

#ifndef _BH3D_ASSIMPLOADER_H_
#define _BH3D_ASSIMPLOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.hpp"
#include "AnimMesh.hpp"
#include "StuffMath.hpp"
#include "SDLAssimpIO.hpp"

#define BH3D_DEFAULT_TICKS_PER_SECOND 25.0f

#define BH3D_LOAD_NORMAL_MAP_TEXTURE 0
#define BH3D_NORMAL_MAP_TEXTURE_EXT "_normal"

namespace BH3D
{

	class AssimpLoader
	{
		public:
			AssimpLoader();
			AssimpLoader(const std::string &pFile);
			~AssimpLoader();

			bool LoadFile(const std::string &pFile);

			void ExportTo(Mesh *pMesh);

			void ExportTo(AnimMesh *pMesh);

			void CloseFile();


		private:

			//---------------------------------------------------------------------------------------
			// Fonctions & data pour mesh static
			//---------------------------------------------------------------------------------------

			void ComputeNodeTransformationsForStaticMesh();
			void RecursiveNodePathForStaticMesh(const struct aiNode * parent);
			std::map<unsigned int, glm::mat4> mapTransformByMesh;

			//---------------------------------------------------------------------------------------
			// Fonctions & data pour mesh animée
			//---------------------------------------------------------------------------------------
			void ClearContainers(); //supprime tous les containers en prévision d'une nouvelle importation

			void ComputeNodeInfo(); //création du tableau des Joints à partir des aiNodes + mappage de données
			void NodeInfo(const struct aiNode * ainode, unsigned int parent_ID);

			//map mesh/node
			//std::vector<const struct aiNode*> tpaiNodeByMesh; //contient le pointeur du node pour le mesh  tpaiNodeFromMesh[id_mesh] = id_ptr_node
			std::vector<unsigned int> mapJointByMesh; //contient l'indice du node pour le mesh tJointFromMesh[id_mesh] = id_joint
			std::map<std::string, unsigned int> mapJointByName;
			AnimMesh * pAnimMesh;

			//---------------------------------------------------------------------------------------
			// Fonctions & data pour mesh animée et static
			//--------------------------------------------------------------------------------------
			void LoadGLTextures();

			void aiToglmMat(const aiMatrix4x4 &aim, glm::mat4 &glmm);
			glm::vec3 aiToglm(const  aiVector3D &v);
			glm::quat aiToglm(const  aiQuaternion &q);






			unsigned int FindJointFromMesh(unsigned int id_mesh);


			MatStack mTransform;

			// Creation d'une instance de la classe Importer
			Assimp::Importer *importer;

			// the global Assimp scene object
			const aiScene* paiScene;




			//map image filenames to textureIds
			std::map<std::string, Texture> textureFile;
			std::map<std::string, Texture> normalTextureFile;




			File cfile;

			std::string basepath;


	};
}
#endif //
