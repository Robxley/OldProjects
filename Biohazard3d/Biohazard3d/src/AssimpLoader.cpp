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



#include "AssimpLoader.hpp"
#include "Common.hpp"
#include <fstream> 
 

namespace BH3D
{
	AssimpLoader::AssimpLoader() : importer(nullptr), paiScene(nullptr)
	{

	}

	AssimpLoader::AssimpLoader(const std::string &pFile) : importer(nullptr), paiScene(nullptr)
	{
		LoadFile(pFile);
	}

	AssimpLoader::~AssimpLoader()
	{
		CloseFile();
	}

	void AssimpLoader::CloseFile()
	{
		textureFile.clear();
		if (importer)
			delete importer;

		importer = nullptr;
		paiScene = nullptr;

	}

	bool AssimpLoader::LoadFile(const std::string &pFile)
	{
		if (!importer)
		{
			importer = new Assimp::Importer;
			if (!importer)
			{
				BH3D_LOGGERERROR("Importer Allocation FAIL");
				return BH3D_ERROR;
			}
		}

		//Options de chargement assimp.

		//Supprime les points et lignes, on affiche que des faces.
		importer->SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

		/* max bone weight : 4	(par défaut BH3D_MAX_WEIGHTS_BY_VERTEX)	*/
		importer->SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, BH3D_MAX_WEIGHTS_BY_VERTEX);

		// put my custom IO handling in place
		importer->SetIOHandler(new SDLIOSystem());

		
		//chargement du fichier par Assimp
		paiScene = importer->ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes);
		if (!paiScene)
		{
			BH3D_LOGGERERROR("3D File Fail :" << pFile << " ; assimp error : " << importer->GetErrorString());
			return BH3D_ERROR;
		}

		cfile = pFile;
		BH3D_LOGGER("3D file loaded : " + pFile + ".");



		return BH3D_OK;
	}

	void AssimpLoader::LoadGLTextures()
	{


		if (paiScene->HasTextures()) return; //pas de texture

		//lecture des emplacements textures
		for (unsigned int m = 0; m < paiScene->mNumMaterials; m++)
		{
			int texIndex = 0;
			aiReturn texFound = AI_SUCCESS;

			aiString path;	// filename

			while (texFound == AI_SUCCESS)
			{
				//chemin d'acces de la texture
				texFound = paiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
				if (texFound == AI_SUCCESS)
				{
					textureFile[path.data] = Texture();
					texIndex++;
				}
			}
		}

		unsigned int numTextures = (unsigned int) textureFile.size();

		//chargement des textures
		for (auto & itr : textureFile)
		{
			std::string filename = itr.first;  //nom de la texture

			File fileloc(cfile.Dir() + "/" + filename);	//chemin d'acces
			if (!fileloc.Exists())
			{
				File fname(filename);
				fileloc.set(cfile.Dir() + "/" + fname.Filename());
			}
			
			Texture * pTex = SDLTextureManager::Instance().Load(fileloc.Fullname().c_str());
			if (pTex == nullptr)
			{
				/* texture non trouvee */
				BH3D_LOGGERWARNING("Assimp Texture no found : " << fileloc.Fullname());
			}
			else
				textureFile[filename].SetTexture(pTex);

			if (BH3D_LOAD_NORMAL_MAP_TEXTURE)
			{
				//lecture de la normal map
				File fileloc2(fileloc.Dir() + "/" + fileloc.ShortFilename() + BH3D_NORMAL_MAP_TEXTURE_EXT + "." + fileloc.Extension());
				Texture * pTexNormalMap = SDLTextureManager::Instance().Load(fileloc2.Fullname().c_str());
				if (pTexNormalMap != nullptr)
					normalTextureFile[filename].SetTexture(pTexNormalMap);
			}

		}//fin for


	}

	void AssimpLoader::ExportTo(Mesh *pMesh)
	{

#ifdef _DEBUG
		if (paiScene == nullptr){
			BH3D_LOGGERERROR("aiScene pointeur = null");
			return;
		}

		if (!paiScene->HasMeshes()){
			BH3D_LOGGERERROR("aiScene has no mesh :" << cfile.Filename());
			return;
		}

		if (!pMesh){
			BH3D_LOGGERERROR("pMesh pointeur = null" << cfile.Filename());
			return;
		}
#endif

		if ((pMesh == 0) || (paiScene == 0) || (!paiScene->HasMeshes()))
			return;


		pMesh->Destroy();		//on supprime les éventuelles données du Mesh.
		ClearContainers();		//réinitialise les containers

		//mappages
		LoadGLTextures(); //mappage des textures
		ComputeNodeTransformationsForStaticMesh(); //calcul des matrices de transformation pour l'ensemble des joints

		//combien de meshes contient la scène ?
		unsigned int nbMeshes = paiScene->mNumMeshes; //nombre de meshes

		//variables de travail....(je suis feignant ?)
		std::vector<glm::vec3>&	 tPosition = pMesh->GetTabPosition();
		std::vector<glm::vec3>&	 tNormal = pMesh->GetTabNormal();
		std::vector<glm::vec2>&	 tTexCoord2 = pMesh->GetTabTexCoord2();
		std::vector<glm::vec4>&  tColor4 = pMesh->GetTabColor4();
		std::vector<glm::vec3>&	 tTangent = pMesh->GetTabTangent();
		std::vector<Mesh::SubMesh>&  tSubMeshes = pMesh->GetTabSubMeshes();
		std::vector<Face>&  tFace = pMesh->GetTabFace();

		//Préparation de la mémoire en fonction du nombre de mesh
		unsigned int nbAllVertices = 0;		//nombre total de vertices de tous les meshes
		unsigned int nbAllFaces = 0;
		


		for (unsigned int i = 0; i < nbMeshes; i++)
		{
			const struct aiMesh *aimesh = paiScene->mMeshes[i];
			nbAllVertices += aimesh->mNumVertices;			//on calcule le nombre de vertices
			nbAllFaces += aimesh->mNumFaces;				//on calcule le nombre de faces
			
		}


		//allocation de la mémoire
		tSubMeshes.resize(nbMeshes);		//allocation du nombre de meshes
		tFace.reserve(nbAllFaces); //nombre de faces

		tPosition.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasNormals())
			tNormal.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasVertexColors(0))
			tColor4.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasTextureCoords(0))
			tTexCoord2.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasTangentsAndBitangents())
			tTangent.reserve(nbAllVertices);



		unsigned int vertexOffset = 0;
		unsigned int faceOffset = 0;

		glm::mat4 meshTransform;

		//on copie les meshes
		for (unsigned int n = 0; n < nbMeshes; n++)
		{

			const struct aiMesh* aimesh = paiScene->mMeshes[n];

			//récupétation de la texture
			int texIndex = 0;
			aiString texPath;
			Material material;
			if (AI_SUCCESS == paiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
			{
				material.SetColorMap( textureFile[texPath.data]);
				if (BH3D_LOAD_NORMAL_MAP_TEXTURE)
					material.SetNormalMap(normalTextureFile[texPath.data]);
			}

			meshTransform = mapTransformByMesh[n];
			glm::vec4 tmp;
			//enregistrement des vertices
			for (unsigned int k = 0; k < aimesh->mNumVertices; k++)
			{
				if (aimesh->HasNormals())
				{
					tmp = (meshTransform*glm::vec4(aimesh->mNormals[k].x, aimesh->mNormals[k].y, aimesh->mNormals[k].z, 1));
					tNormal.push_back(glm::vec3(tmp));
				}

				if (aimesh->HasVertexColors(0))
					tColor4.push_back(glm::vec4(aimesh->mColors[0][k].r, aimesh->mColors[0][k].g, aimesh->mColors[0][k].b, aimesh->mColors[0][k].a));

				if (aimesh->HasTextureCoords(0))
					tTexCoord2.push_back(glm::vec2(aimesh->mTextureCoords[0][k].x, 1 - aimesh->mTextureCoords[0][k].y));

				if (aimesh->HasTangentsAndBitangents())
				{
					tmp = meshTransform*glm::vec4(aimesh->mTangents[k].x, aimesh->mTangents[k].y, aimesh->mTangents[k].z, 1);
					tTangent.push_back(glm::vec3(tmp));
				}
				tmp = meshTransform*glm::vec4(aimesh->mVertices[k].x, aimesh->mVertices[k].y, aimesh->mVertices[k].z, 1);
				tPosition.push_back(glm::vec3(tmp));
			}


			//tSubMeshes[n].tFace.resize(aimesh->mNumFaces); //allocation du nombre de faces
			//unsigned int *pfaces = &tFace[faceOffset].id[0]; //place le pointeur au début du tableau d'indices
			
			tSubMeshes[n].vertexOffset = vertexOffset;
			tSubMeshes[n].faceOffset = faceOffset;

			tSubMeshes[n].nFaces = aimesh->mNumFaces;
			tSubMeshes[n].nVertices = aimesh->mNumVertices;

			//copie des faces
			//unsigned int c = 0;
			Face f;
			for (unsigned int t = 0; t < aimesh->mNumFaces; ++t)
			{
				const struct aiFace* face = &aimesh->mFaces[t];
				f.set(face->mIndices[0] + vertexOffset, face->mIndices[1] + vertexOffset, face->mIndices[2] + vertexOffset);
				tFace.push_back(f);
				
				/*for (unsigned int i = 0; i < 3; i++)
				{
					pfaces[c] = face->mIndices[i] + vertexOffset;
					c++;
				}*/
			}
			tSubMeshes[n].nMaterial = material;

			vertexOffset += aimesh->mNumVertices;
			faceOffset += aimesh->mNumFaces;

		} //fin de la copie des meshes

	}

	void AssimpLoader::ComputeNodeTransformationsForStaticMesh()
	{
		if (paiScene == nullptr) return;

		mapTransformByMesh.clear();
		mTransform.Reset();
		RecursiveNodePathForStaticMesh(paiScene->mRootNode);
	}

	void AssimpLoader::RecursiveNodePathForStaticMesh(const struct aiNode * parent)
	{


		glm::mat4 m;
		aiToglmMat(parent->mTransformation, m);//copie la matrice de transformation

		mTransform.Push();				//glPushMatrix(); sauvegarde de la pile de matrice
		mTransform.Mult(m);				//multiplication de la pile de matrice
		for (unsigned int k = 0; k < parent->mNumMeshes; k++)
		{
			unsigned int id_mesh = parent->mMeshes[k];
			mapTransformByMesh[id_mesh] = mTransform.Top();
		}

		for (unsigned int k = 0; k < parent->mNumChildren; k++)
		{
			RecursiveNodePathForStaticMesh(parent->mChildren[k]);
		}
		mTransform.Pop();
	}
	/**/
	void AssimpLoader::ExportTo(AnimMesh *pMesh)
	{

#ifdef _DEBUG
		if (paiScene == nullptr){
			BH3D_LOGGERERROR("aiScene pointeur = null");
			return;
		}

		if (!paiScene->HasMeshes()){
			BH3D_LOGGERERROR(" no mesh :" << cfile.Filename());
			return;
		}

		if (!pMesh){
			BH3D_LOGGERERROR("pMesh pointeur = null" << cfile.Filename());
			return;
		}
#endif

		if ((pMesh == 0) || (paiScene == 0) || (!paiScene->HasMeshes()))
			return;

		pAnimMesh = pMesh;

		//on démarre sur des objets vierges
		pAnimMesh->Destroy();
		ClearContainers();

		//mapage par mesh et chargement des textures
		LoadGLTextures();



		unsigned int nbMeshes = paiScene->mNumMeshes; //nombre de meshes


		std::vector<glm::vec3>&	 tPosition = pAnimMesh->GetTabPosition();
		std::vector<glm::vec3>&	 tNormal = pAnimMesh->GetTabNormal();
		std::vector<glm::vec2>&	 tTexCoord2 = pAnimMesh->GetTabTexCoord2();
		std::vector<glm::vec4>&  tColor4 = pAnimMesh->GetTabColor4();
		std::vector<glm::vec3>&	 tTangent = pAnimMesh->GetTabTangent();
		std::vector<Mesh::SubMesh>&  tSubMeshes = pAnimMesh->GetTabSubMeshes();
		std::vector<Face>&  tFace = pAnimMesh->GetTabFace();


		//Préparation de la mémoire en fonction du nombre de mesh
		//on parcourt la liste des Meshes
		unsigned int nbAllVertices = 0;	//nombre total de vertices pour l'ensemble des meshes
		unsigned int nbAllFaces = 0;	//nombre total de face pour l'ensemble des meshes
		unsigned int nbAllBones = 0; //nombre total de bones pour l'ensemble des meshes
		tSubMeshes.resize(nbMeshes);
		for (unsigned int i = 0; i < nbMeshes; i++)
		{
			const struct aiMesh *aimesh = paiScene->mMeshes[i];
			nbAllVertices += aimesh->mNumVertices; //on cumule le nombre de vertice
			nbAllFaces += aimesh->mNumFaces;
			nbAllBones += aimesh->mNumBones;
		}

		tFace.reserve(nbAllFaces); //nombre de faces

		tPosition.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasNormals())
			tNormal.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasVertexColors(0))
			tColor4.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasTextureCoords(0))
			tTexCoord2.reserve(nbAllVertices);
		if (paiScene->mMeshes[0]->HasTangentsAndBitangents())
			tTangent.reserve(nbAllVertices);


		/*********animation******************************/
		std::vector<AnimMesh::VertexBoneInfo>& tVertexBoneInfo = pAnimMesh->getTabVertexBoneInfo();

		tVertexBoneInfo.resize(nbAllVertices);
		ComputeNodeInfo();
		pAnimMesh->getSkeleton().tBones.reserve(nbAllBones);

		/******************************************************/

		unsigned int vertexOffset = 0;
		unsigned int faceOffset = 0;

		//on copie les meshes
		for (unsigned int n = 0; n < nbMeshes; n++)
		{
			/*******************************************************************************/
			/*** LECTURE DE LA PARTIE DATA MESH											****/
			/*******************************************************************************/
			const struct aiMesh* aimesh = paiScene->mMeshes[n];

			//récupétation de la texutre
			int texIndex = 0;
			aiString texPath;
			Material material;
			if (AI_SUCCESS == paiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
			{
				material.SetColorMap(textureFile[texPath.data]);
				if (BH3D_LOAD_NORMAL_MAP_TEXTURE)
					material.SetNormalMap(normalTextureFile[texPath.data]);
			}

			//copie des vertices du mesh
			for (unsigned int k = 0; k < aimesh->mNumVertices; k++)
			{
				if (aimesh->HasNormals())
					tNormal.push_back(glm::vec3(aimesh->mNormals[k].x, aimesh->mNormals[k].y, aimesh->mNormals[k].z));

				if (aimesh->HasVertexColors(0))
					tColor4.push_back(glm::vec4(aimesh->mColors[0][k].r, aimesh->mColors[0][k].g, aimesh->mColors[0][k].b, aimesh->mColors[0][k].a));

				if (aimesh->HasTextureCoords(0))
					tTexCoord2.push_back(glm::vec2(aimesh->mTextureCoords[0][k].x, 1 - aimesh->mTextureCoords[0][k].y));

				if (aimesh->HasTangentsAndBitangents())
					tTangent.push_back(glm::vec3(aimesh->mTangents[k].x, aimesh->mTangents[k].y, aimesh->mTangents[k].z));

				tPosition.push_back(glm::vec3(aimesh->mVertices[k].x, aimesh->mVertices[k].y, aimesh->mVertices[k].z));
			}

			//copie des faces du mesh
			tSubMeshes[n].vertexOffset = vertexOffset;
			tSubMeshes[n].faceOffset = faceOffset;

			tSubMeshes[n].nFaces = aimesh->mNumFaces;
			tSubMeshes[n].nVertices = aimesh->mNumVertices;

			Face f;
			for (unsigned int t = 0; t < aimesh->mNumFaces; ++t)
			{
				const struct aiFace* face = &aimesh->mFaces[t];
				f.set(face->mIndices[0] + vertexOffset, face->mIndices[1] + vertexOffset, face->mIndices[2] + vertexOffset);
				tFace.push_back(f);
			}
			tSubMeshes[n].nMaterial = material;



			/*******************************************************************************/
			/*** LECTURE DE LA PARTIE DYNAMIC DU MESH									****/
			/*******************************************************************************/

			//pour l'ensemble des bones du mesh
			for (unsigned int b = 0; b < aimesh->mNumBones; b++)
			{
				const struct aiBone * aibone = aimesh->mBones[b];

				Bone bone;
				aiToglmMat(aibone->mOffsetMatrix, bone.offsetPose); //copie la matrice d'offset du bone


				std::string name = aibone->mName.data; //nom du node associé
				bone.jointID = mapJointByName[name]; //liaison avec le joint correspondant.
				pAnimMesh->getSkeleton().tJoints[bone.jointID].isBone = 1;
				pAnimMesh->getSkeleton().tJoints[bone.jointID].offsetPose = bone.offsetPose;

				pAnimMesh->getSkeleton().tBones.push_back(bone);

				//copie des poids par vertex
				for (unsigned int w = 0; w < aibone->mNumWeights; w++)
				{
					unsigned int vid = aibone->mWeights[w].mVertexId + vertexOffset;
					tVertexBoneInfo[vid].Add(bone.jointID, aibone->mWeights[w].mWeight);
				}

			} //fin boucle sur bone

			vertexOffset += aimesh->mNumVertices;
			faceOffset += aimesh->mNumFaces;

		} //fin boucle sur mesh

		/*******************************************************************************/
		/*** LECTURE DES ANIMATIONS													****/
		/*******************************************************************************/

		std::vector<Animation> & tAnimation = pAnimMesh->getSkeleton().tAnimations;
		tAnimation.resize(paiScene->mNumAnimations);


		//copie des animations
		for (unsigned int a = 0; a < paiScene->mNumAnimations; a++)
		{
			const struct aiAnimation* aianim = paiScene->mAnimations[a];
			tAnimation[a].duration = (float)aianim->mDuration;
			tAnimation[a].ticksPerSecond = (float)(aianim->mTicksPerSecond != 0 ? aianim->mTicksPerSecond : BH3D_DEFAULT_TICKS_PER_SECOND);
			tAnimation[a].tAnimKey.resize(aianim->mNumChannels);
			tAnimation[a].name = aianim->mName.data;


			for (unsigned int c = 0; c < aianim->mNumChannels; c++)
			{
				const struct aiNodeAnim* ainodeanim = aianim->mChannels[c];
				std::string name = ainodeanim->mNodeName.data;
				tAnimation[a].tAnimKey[c].joint = (mapJointByName[name]); //le joint concerné par l'animation

				//pAnimMesh->getSkeleton().tJoints[mapJointByName[name]].animKeyID[a] = c;

				unsigned int q;
				//copie des key frames
				tAnimation[a].tAnimKey[c].tPosKeys.resize(ainodeanim->mNumPositionKeys);
				for (q = 0; q < aianim->mChannels[c]->mNumPositionKeys; q++)
				{
					tAnimation[a].tAnimKey[c].tPosKeys[q].times = (float)ainodeanim->mPositionKeys[q].mTime;
					tAnimation[a].tAnimKey[c].tPosKeys[q].value = aiToglm(ainodeanim->mPositionKeys[q].mValue);
				}
				tAnimation[a].tAnimKey[c].tRotKeys.resize(ainodeanim->mNumRotationKeys);
				for (q = 0; q < aianim->mChannels[c]->mNumRotationKeys; q++)
				{
					tAnimation[a].tAnimKey[c].tRotKeys[q].times = (float)ainodeanim->mRotationKeys[q].mTime;
					tAnimation[a].tAnimKey[c].tRotKeys[q].value = aiToglm(ainodeanim->mRotationKeys[q].mValue);
				}
				tAnimation[a].tAnimKey[c].tScaleKeys.resize(aianim->mChannels[c]->mNumScalingKeys);
				for (q = 0; q < aianim->mChannels[c]->mNumScalingKeys; q++)
				{
					tAnimation[a].tAnimKey[c].tScaleKeys[q].times = (float)ainodeanim->mScalingKeys[q].mTime;
					tAnimation[a].tAnimKey[c].tScaleKeys[q].value = aiToglm(ainodeanim->mScalingKeys[q].mValue);
				}
			}

		}




	}
	
	void AssimpLoader::ClearContainers()
	{
		textureFile.clear();
		normalTextureFile.clear();
		//tpaiNodeByMesh.clear();
		mapJointByMesh.clear();
	}

	void AssimpLoader::ComputeNodeInfo()
	{
		mapJointByName.clear();
		mapJointByMesh.clear();
		mTransform.Reset();

		//mappage entre les meshes et nodes
		mapJointByMesh.resize(paiScene->mNumMeshes);

		NodeInfo(paiScene->mRootNode, 0);
	}

	void AssimpLoader::NodeInfo(const struct aiNode * ainode, unsigned int parent_ID)
	{

		glm::mat4 m;
		aiToglmMat(ainode->mTransformation, m);//copie la matrice de transformation

		//creation du joint à partir du ainode
		Joint joint;
		joint.localPose = m;

		mTransform.Push();		//glPushMatrix(); sauvegarde de la pile de matrice
		mTransform.Mult(m); //multiplication de la pile de matrice
		joint.globalPose = m;

		joint.parentID = parent_ID;
		pAnimMesh->getSkeleton().tJoints.push_back(joint); //ajoute le nouveau joint

		unsigned int jointID = (unsigned int) pAnimMesh->getSkeleton().tJoints.size() - 1;
		mapJointByName[ainode->mName.data] = jointID;

		pAnimMesh->getSkeleton().tJoints.back().tMeshIDs.reserve(ainode->mNumMeshes);
		for (unsigned int i = 0; i < ainode->mNumMeshes; i++)
		{
			unsigned int id_mesh = ainode->mMeshes[i];
			//tpaiNodeByMesh[id_mesh] = ainode;
			pAnimMesh->getSkeleton().tJoints.back().tMeshIDs.push_back(id_mesh);
			mapJointByMesh[id_mesh] = jointID; //relève l'id du joint pour le mesh associé
		}

		for (unsigned int i = 0; i < ainode->mNumChildren; i++)
			NodeInfo(ainode->mChildren[i], jointID);

		mTransform.Pop();
	}



	void AssimpLoader::aiToglmMat(const  aiMatrix4x4 &aim, glm::mat4 &glmm)
	{
		//glmm = *( (glm::mat4*) (&aim) );
		memcpy(&glmm, &aim, sizeof(float) * 16);
		glmm = glm::transpose(glmm);
	}

	glm::vec3 AssimpLoader::aiToglm(const aiVector3D &v)
	{
		return glm::vec3(v.x, v.y, v.z);

	}

	glm::quat AssimpLoader::aiToglm(const aiQuaternion &q)
	{

		return glm::quat(q.w, q.x, q.y, q.z);
	}
}