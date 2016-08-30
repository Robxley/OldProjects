

#include "RxAssimpLoader.h"



RxAssimpLoader::RxAssimpLoader() : importer(NULL), paiScene(NULL)
{
	
}

RxAssimpLoader::RxAssimpLoader(const std::string &pFile) : importer(NULL), paiScene(NULL)
{
	Open3DFile(pFile);
}

RxAssimpLoader::~RxAssimpLoader()
{
	Close3DFile();
}

void RxAssimpLoader::Close3DFile()
{
	textureFile.clear();
	if(importer)
		delete importer;

	importer = NULL;
	paiScene = NULL;

}

bool RxAssimpLoader::Open3DFile(const std::string &pFile)
{
	if(!importer)
	{
		importer = new Assimp::Importer;
		if(!importer)
		{
			RXLOGGERERROR("[RxAssimpLoader][Open3DFile] : Importer Allocation FAIL");
			return RX_ERROR;
		}
	}


	cfile.set(pFile);
	//verification de l'existance du fichier
	std::ifstream fin(pFile.c_str());
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		//le fichier n'existe pas
		RXLOGGERERROR("[RxAssimpLoader][Open3DFile] no exist file : "<<pFile);
		return RX_ERROR;
	}



	//Supprime les points et lignes, on affiche que des faces.
	importer->SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	
	/* max bone weight : 4	(RX_MAX_WEIGHTS_BY_VERTEX)	*/
	importer->SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, RX_MAX_WEIGHTS_BY_VERTEX);


	//chargement du fichier par Assimp
	paiScene = importer->ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality | aiProcess_ValidateDataStructure | aiProcess_OptimizeMeshes);
	if( !paiScene)
	{
		RXLOGGERERROR("[RxAssimpLoader][Open3DFile] : File Fail :"<<pFile<<" ; error : "<<importer->GetErrorString());
		return RX_ERROR;
	}

	
	RXLOGGER("[RxAssimpLoader][Open3DFile] : Import of scene : " + pFile + ".");



	return RX_OK;
}

void RxAssimpLoader::LoadGLTextures()
{


	if (paiScene->HasTextures()) return; //pas de texture

	//lecture des emplacements textures
	for (unsigned int m=0; m<paiScene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			//chemin d'acces de la texture
			texFound = paiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			if(texFound == AI_SUCCESS)
			{
				textureFile[path.data] = RxTexture();
				texIndex++;
			}
		}
	}

	int numTextures = textureFile.size();


	std::map<std::string,RxTexture>::iterator itr = textureFile.begin();

	//chargement des textures
	for (; itr != textureFile.end(); itr++)
	{
		std::string filename = itr->first;  //nom de la texture

		RxFile fileloc(cfile.Dir()+"/" + filename);	//chemin d'acces
		if(!fileloc.Exists())
		{
			RxFile fname(filename);
			fileloc.set(cfile.Dir()+"/" + fname.Filename());
		}
		textureFile[filename] =  RxTexManager::Instance().LoadTexture(fileloc.Fullname().c_str());
		if (itr->second.glID == RxTexManager::Instance().getDefaultTexture().glID) 
		{
			/* texture non trouvee */
			RXLOGGER("[RxAssimpLoader][LoadGLTextures] Texture no found : " << fileloc.Fullname());
		}
		
		if(RX_LOAD_NORMAL_MAP_TEXTURE)
		{
			//lecture de la normal map
			RxFile fileloc2(fileloc.Dir()+"/"+fileloc.ShortFilename()+RX_NORMAL_MAP_TEXTURE_EXT+"."+fileloc.Extension() );
			RxTexture normalMap =  RxTexManager::Instance().LoadTexture(fileloc2.Fullname().c_str());
			RxTexture defTex = RxTexManager::Instance().getDefaultTexture();
			if(normalMap.glID == defTex.glID)
				normalMap.glID = 0;

			normalTextureFile[filename] = normalMap;
		}

	}


}

void RxAssimpLoader::ExportTo(RxMesh *pMesh)
{

	

	#ifdef _DEBUG
		if(paiScene == NULL){
			RXLOGGERERROR("[RxAssimpLoader][ImportMeshToVBO] aiScene pointeur = null");
			return;
		}

		if (!paiScene->HasMeshes()){
			RXLOGGERERROR("[RxAssimpLoader][ImportMeshToVBO] no mesh :"<<cfile.Filename());
			return;
		}

		if (!pMesh){
			RXLOGGERERROR("[RxAssimpLoader][ImportMeshToVBO] pMesh pointeur = null"<<cfile.Filename());
			return;
		}
	#endif

	if( (pMesh == 0) || (paiScene == 0) || (!paiScene->HasMeshes()))
		return;
	

	pMesh->Destroy();
	ClearContainers();

	//mapage
	LoadGLTextures();
	ComputeNodeTransformationsForStaticMesh();

	unsigned int nbMeshes = paiScene->mNumMeshes; //nombre de meshes

	std::vector<glm::vec3>&	 tPosition		= pMesh->getTabPosition();
	std::vector<glm::vec3>&	 tNormal		= pMesh->getTabNormal();
	std::vector<glm::vec2>&	 tTexCoord2		= pMesh->getTabTexCoord2();
	std::vector<glm::vec4>&  tColor4		= pMesh->getTabColor4();
	std::vector<glm::vec3>&	 tTangent		= pMesh->getTabTangent();
	std::vector<RxMesh::RxSubMesh>&  tSubMeshes		= pMesh->getTabSubMeshes();


	//Préparation de la mémoire en fonction du nombre de mesh
	//on parcourt la liste des Meshes
	unsigned int nbAllVertices = 0;	//nombre total de vertices de tous les meshes
	tSubMeshes.resize(nbMeshes);

	for(unsigned int i = 0; i < nbMeshes; i++)
	{
		const struct aiMesh *aimesh = paiScene->mMeshes[i];
		nbAllVertices += aimesh->mNumVertices; //on cumule le nombre de vertice
		tSubMeshes[i].tFace.reserve(aimesh->mNumFaces); //nombre de faces
	}
	tPosition.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasNormals())
		tNormal.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasVertexColors(0))
		tColor4.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasTextureCoords(0))
		tTexCoord2.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasTangentsAndBitangents())
		tTangent.reserve(nbAllVertices);



	unsigned int vertexOffset = 0;
	unsigned int faceOffset = 0;
	unsigned int indexGroup = 0;


	glm::mat4 meshTransform;

	//on copie les meshes
	for (unsigned int n = 0; n < nbMeshes; n++)
	{

		const struct aiMesh* aimesh = paiScene->mMeshes[n];
		
		//récupétation de la texutre
		int texIndex = 0;
		aiString texPath;	
		RxMaterial material;
		if(AI_SUCCESS == paiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
		{
			 material.colorMap = textureFile[texPath.data];
			 if(RX_LOAD_NORMAL_MAP_TEXTURE)
				material.normalMap = normalTextureFile[texPath.data];
		}

		meshTransform = mapTransformByMesh[n];
		glm::vec4 tmp;
		//enregistrement des vertices
		for(unsigned int k = 0; k < aimesh->mNumVertices; k++)
		{
			if(aimesh->HasNormals())
			{
				tmp = (meshTransform*glm::vec4(aimesh->mNormals[k].x,aimesh->mNormals[k].y,aimesh->mNormals[k].z,1));
				tNormal.push_back( glm::vec3(tmp) );
			}

			if(aimesh->HasVertexColors(0))
				tColor4.push_back(glm::vec4(aimesh->mColors[0][k].r,aimesh->mColors[0][k].g,aimesh->mColors[0][k].b,aimesh->mColors[0][k].a));

			if(aimesh->HasTextureCoords(0))
				tTexCoord2.push_back(glm::vec2(aimesh->mTextureCoords[0][k].x, 1 - aimesh->mTextureCoords[0][k].y));

			if(aimesh->HasTangentsAndBitangents())
			{
				tmp = meshTransform*glm::vec4(aimesh->mTangents[k].x,aimesh->mTangents[k].y,aimesh->mTangents[k].z,1);
				tTangent.push_back( glm::vec3(tmp) );
			}
			tmp =  meshTransform*glm::vec4(aimesh->mVertices[k].x,aimesh->mVertices[k].y,aimesh->mVertices[k].z,1);
			tPosition.push_back (glm::vec3(tmp)  );
		}
		
		
		tSubMeshes[n].tFace.resize(aimesh->mNumFaces); //allocation du nombre de faces
		unsigned int *pfaces = &tSubMeshes[n].tFace[0].id[0]; //place le pointeur au début du tableau d'indices
		tSubMeshes[n].vertexOffset = vertexOffset;
		tSubMeshes[n].faceOffset = faceOffset;

		//copie des faces
		unsigned int c = 0;
		for (unsigned int t = 0; t < aimesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &aimesh->mFaces[t];
			for(unsigned int i = 0; i < 3; i++)		
			{
				pfaces[c] = face->mIndices[i]+vertexOffset;
				c++;
			}
		}
		tSubMeshes[n].nMaterial = material;
		
		vertexOffset += aimesh->mNumVertices;
		
	}

}

void RxAssimpLoader::ComputeNodeTransformationsForStaticMesh()
{
	if(paiScene == NULL) return;

	mapTransformByMesh.clear();
	mTransform.Reset();
	RecursiveNodePathForStaticMesh(paiScene->mRootNode);
}

void RxAssimpLoader::RecursiveNodePathForStaticMesh(const struct aiNode * parent)
{

	
	glm::mat4 m;
	aiToglmMat( parent->mTransformation,m);//copie la matrice de transformation
	
	mTransform.PushMatrix();		//glPushMatrix(); sauvegarde de la pile de matrice
	mTransform.MultMatrix( m ); //multiplication de la pile de matrice
	for(unsigned int k=0; k < parent->mNumMeshes; k++)
	{
		unsigned int id_mesh = parent->mMeshes[k];
		mapTransformByMesh[id_mesh] = mTransform.Top();
	}

	for(unsigned int k=0; k < parent->mNumChildren; k++)
	{
		RecursiveNodePathForStaticMesh(parent->mChildren[k]);
	}
	mTransform.PopMatrix();
}

void RxAssimpLoader::ExportTo(RxAnimMesh *pMesh)
{

	#ifdef _DEBUG
		if(paiScene == NULL){
			RXLOGGERERROR("[RxAssimpLoader][ImportMeshToVBO] aiScene pointeur = null");
			return;
		}

		if (!paiScene->HasMeshes()){
			RXLOGGERERROR("[RxAssimpLoader][ImportMeshToVBO] no mesh :"<<cfile.Filename());
			return;
		}

		if (!pMesh){
			RXLOGGERERROR("[RxAssimpLoader][ImportMeshToVBO] pMesh pointeur = null"<<cfile.Filename());
			return;
		}
	#endif

	if( (pMesh == 0) || (paiScene == 0) || (!paiScene->HasMeshes()))
		return;
	
	pAnimMesh = pMesh;

	//on démarre sur des objets vierges
	pAnimMesh->Destroy();
	ClearContainers();

	//mapage par mesh et chargement des textures
	LoadGLTextures();



	unsigned int nbMeshes = paiScene->mNumMeshes; //nombre de meshes
	

	std::vector<glm::vec3>&	 tPosition		= pAnimMesh->getTabPosition();
	std::vector<glm::vec3>&	 tNormal		= pAnimMesh->getTabNormal();
	std::vector<glm::vec2>&	 tTexCoord2		= pAnimMesh->getTabTexCoord2();
	std::vector<glm::vec4>&  tColor4		= pAnimMesh->getTabColor4();
	std::vector<glm::vec3>&	 tTangent		= pAnimMesh->getTabTangent();
	std::vector<RxMesh::RxSubMesh>&  tSubMeshes		= pAnimMesh->getTabSubMeshes();




	//Préparation de la mémoire en fonction du nombre de mesh
	//on parcourt la liste des Meshes
	unsigned int nbAllVertices = 0;	//nombre total de vertices pour l'ensemble des meshes
	unsigned int nbAllBones = 0; //nombre total de bones pour l'ensemble des meshes
	tSubMeshes.resize(nbMeshes);
	for(unsigned int i = 0; i < nbMeshes; i++)
	{
		const struct aiMesh *aimesh = paiScene->mMeshes[i];
		nbAllVertices += aimesh->mNumVertices; //on cumule le nombre de vertice
		tSubMeshes[i].tFace.reserve(aimesh->mNumFaces); //nombre de faces
		nbAllBones+=aimesh->mNumBones;
	}



	tPosition.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasNormals())
		tNormal.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasVertexColors(0))
		tColor4.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasTextureCoords(0))
		tTexCoord2.reserve(nbAllVertices);
	if(paiScene->mMeshes[0]->HasTangentsAndBitangents())
		tTangent.reserve(nbAllVertices);


/*********animation******************************/
	std::vector<RxAnimMesh::RxVertexBoneInfo>& tVertexBoneInfo = pAnimMesh->getTabVertexBoneInfo();

	tVertexBoneInfo.resize(nbAllVertices);
	ComputeNodeInfo();
	pAnimMesh->getSkeleton().tBones.reserve(nbAllBones);

/******************************************************/




	unsigned int vertexoffset = 0;



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
		RxMaterial material;
		if(AI_SUCCESS == paiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
		{
			 material.colorMap = textureFile[texPath.data];
			 if(RX_LOAD_NORMAL_MAP_TEXTURE)
				material.normalMap = normalTextureFile[texPath.data];
		}

		//copie des vertices du mesh
		for(unsigned int k = 0; k < aimesh->mNumVertices; k++)
		{
			if(aimesh->HasNormals())
				tNormal.push_back( glm::vec3(aimesh->mNormals[k].x,aimesh->mNormals[k].y,aimesh->mNormals[k].z) );

			if(aimesh->HasVertexColors(0))
				tColor4.push_back(glm::vec4(aimesh->mColors[0][k].r,aimesh->mColors[0][k].g,aimesh->mColors[0][k].b,aimesh->mColors[0][k].a));

			if(aimesh->HasTextureCoords(0))
				tTexCoord2.push_back(glm::vec2(aimesh->mTextureCoords[0][k].x, 1 - aimesh->mTextureCoords[0][k].y));

			if(aimesh->HasTangentsAndBitangents())
				tTangent.push_back( glm::vec3(aimesh->mTangents[k].x,aimesh->mTangents[k].y,aimesh->mTangents[k].z) );

			tPosition.push_back ( glm::vec3(aimesh->mVertices[k].x,aimesh->mVertices[k].y,aimesh->mVertices[k].z) );
		}
		
		//copie des faces du mesh
		tSubMeshes[n].tFace.resize(aimesh->mNumFaces); //allocation du nombre de faces
		unsigned int *pfaces = &tSubMeshes[n].tFace[0].id[0]; //place le pointeur au début du tableau d'indices
		
		//copie des faces
		unsigned int c = 0;
		for (unsigned int t = 0; t < aimesh->mNumFaces; ++t)
		{
			const struct aiFace* face = &aimesh->mFaces[t];
			for(unsigned int i = 0; i < 3; i++)		
			{
				pfaces[c] = face->mIndices[i]+vertexoffset;
				c++;
			}
		}
		tSubMeshes[n].nMaterial = material;
		


		/*******************************************************************************/
		/*** LECTURE DE LA PARTIE DYNAMIC DU MESH									****/
		/*******************************************************************************/
		
		//pour l'ensemble des bones du mesh
		for(unsigned int b = 0; b < aimesh->mNumBones; b++)
		{
			const struct aiBone * aibone = aimesh->mBones[b];

			RxBone bone;
			aiToglmMat(aibone->mOffsetMatrix, bone.offsetPose); //copie la matrice d'offset du bone


			std::string name = aibone->mName.data; //nom du node associé
			bone.jointID = mapJointByName[name]; //liaison avec le joint correspondant.
			pAnimMesh->getSkeleton().tJoints[bone.jointID].isBone = 1;
			pAnimMesh->getSkeleton().tJoints[bone.jointID].offsetPose = bone.offsetPose;
			
			pAnimMesh->getSkeleton().tBones.push_back(bone);

			//copie des poids par vertex
			for(unsigned int w = 0; w < aibone->mNumWeights; w++)
			{
				unsigned int vid = aibone->mWeights[w].mVertexId+vertexoffset;
				tVertexBoneInfo[vid].Add(bone.jointID,aibone->mWeights[w].mWeight);
			}

		} //fin boucle sur bone

		vertexoffset += aimesh->mNumVertices;
		
	} //fin boucle sur mesh

		/*******************************************************************************/
		/*** LECTURE DES ANIMATIONS													****/
		/*******************************************************************************/

	std::vector<RxAnimation> & tAnimation = pAnimMesh->getSkeleton().tAnimations;
	tAnimation.resize(paiScene->mNumAnimations);
	

	//copie des animations
	for(unsigned int a = 0; a < paiScene->mNumAnimations; a++)
	{
		const struct aiAnimation* aianim = paiScene->mAnimations[a];
		tAnimation[a].duration = (float) aianim->mDuration;
		tAnimation[a].ticksPerSecond = (float) (aianim->mTicksPerSecond != 0 ? aianim->mTicksPerSecond : RX_DEFAULT_TICKS_PER_SECOND);
		tAnimation[a].tAnimKey.resize(aianim->mNumChannels);
		tAnimation[a].name = aianim->mName.data;

		
		for(unsigned int c = 0; c < aianim->mNumChannels; c++)
		{
			const struct aiNodeAnim* ainodeanim = aianim->mChannels[c];
			std::string name = ainodeanim->mNodeName.data;
			tAnimation[a].tAnimKey[c].joint = (mapJointByName[name]); //le joint concerné par l'animation
			
			//pAnimMesh->getSkeleton().tJoints[mapJointByName[name]].animKeyID[a] = c;

			unsigned int q;
			//copie des key frames
			tAnimation[a].tAnimKey[c].tPosKeys.resize(ainodeanim->mNumPositionKeys);
			for(q = 0; q < aianim->mChannels[c]->mNumPositionKeys; q++)
			{
				tAnimation[a].tAnimKey[c].tPosKeys[q].times = (float) ainodeanim->mPositionKeys[q].mTime;
				tAnimation[a].tAnimKey[c].tPosKeys[q].value = aiToglm( ainodeanim->mPositionKeys[q].mValue);
			}
			tAnimation[a].tAnimKey[c].tRotKeys.resize(ainodeanim->mNumRotationKeys);
			for(q = 0; q < aianim->mChannels[c]->mNumRotationKeys; q++)
			{
				tAnimation[a].tAnimKey[c].tRotKeys[q].times = (float) ainodeanim->mRotationKeys[q].mTime;
				tAnimation[a].tAnimKey[c].tRotKeys[q].value = aiToglm( ainodeanim->mRotationKeys[q].mValue);
			}
			tAnimation[a].tAnimKey[c].tScaleKeys.resize(aianim->mChannels[c]->mNumScalingKeys);
			for(q = 0; q < aianim->mChannels[c]->mNumScalingKeys; q++)
			{
				tAnimation[a].tAnimKey[c].tScaleKeys[q].times = (float) ainodeanim->mScalingKeys[q].mTime;
				tAnimation[a].tAnimKey[c].tScaleKeys[q].value = aiToglm( ainodeanim->mScalingKeys[q].mValue);
			}
		}

	}




}

void RxAssimpLoader::ClearContainers()
{
	textureFile.clear();
	normalTextureFile.clear();
	//tpaiNodeByMesh.clear();
	mapJointByMesh.clear();
}

void RxAssimpLoader::ComputeNodeInfo()
{
	mapJointByName.clear();
	mapJointByMesh.clear();
	mTransform.Reset();

	//mappage entre les meshes et nodes
	mapJointByMesh.resize(paiScene->mNumMeshes);

	NodeInfo(paiScene->mRootNode,0);
}

void RxAssimpLoader::NodeInfo(const struct aiNode * ainode, unsigned int parent_ID)
{

	glm::mat4 m;
	aiToglmMat( ainode->mTransformation,m);//copie la matrice de transformation

	//creation du joint à partir du ainode
	RxJoint joint;
	joint.localPose = m;

	mTransform.PushMatrix();		//glPushMatrix(); sauvegarde de la pile de matrice
	mTransform.MultMatrix( m ); //multiplication de la pile de matrice
	joint.globalPose = m;

	joint.parentID = parent_ID;
	pAnimMesh->getSkeleton().tJoints.push_back(joint); //ajoute le nouveau joint
	
	unsigned int jointID = pAnimMesh->getSkeleton().tJoints.size()-1;
	mapJointByName[ainode->mName.data] = jointID;

	pAnimMesh->getSkeleton().tJoints.back().tMeshIDs.reserve(ainode->mNumMeshes);
	for(unsigned int i = 0; i < ainode->mNumMeshes; i++)
	{
		unsigned int id_mesh = ainode->mMeshes[i];
		//tpaiNodeByMesh[id_mesh] = ainode;
		pAnimMesh->getSkeleton().tJoints.back().tMeshIDs.push_back(id_mesh);
		mapJointByMesh[id_mesh] = jointID; //relève l'id du joint pour le mesh associé
	}

	for(unsigned int i =0; i < ainode->mNumChildren; i++)
		NodeInfo(ainode->mChildren[i], jointID);

	mTransform.PopMatrix();
}



void RxAssimpLoader::aiToglmMat(const  aiMatrix4x4 &aim, glm::mat4 &glmm)
{
	//glmm = *( (glm::mat4*) (&aim) );
	memcpy(&glmm,&aim,sizeof(float) * 16);
	glmm = glm::transpose(glmm);
}

glm::vec3 RxAssimpLoader::aiToglm(const aiVector3D &v)
{
	return glm::vec3(v.x,v.y,v.z);

}

glm::quat RxAssimpLoader::aiToglm(const aiQuaternion &q)
{
	
		return glm::quat(q.w,q.x,q.y,q.z);
}