
//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#		http://ephenationopengl.blogspot.fr/2012/06/doing-animations-in-opengl.html												       #
//#============================================================#


#pragma once

#ifndef _RXASSIMPLOADER_H_
#define _RXASSIMPLOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RxMesh.h"
#include "RxAnimMesh.h"
#include "RxSimpleMesh.h"
#include "RxLogger.h"
#include "RxTexManager.h"
#include "RxFile.h"
#include "RxTools.h"


#define RX_DEFAULT_TICKS_PER_SECOND 25.0f

#define RX_LOAD_NORMAL_MAP_TEXTURE 0
#define RX_NORMAL_MAP_TEXTURE_EXT "_normal"

/****************************************************/
/* assimp property for loading mesh		:			*/
/* triangulate, no line, no point					*/
/* max bone weight : 4	(RX_MAX_WEIGHTS_BY_VERTEX)	*/
/****************************************************/


class RxAssimpLoader
{
	public:
		RxAssimpLoader();
		RxAssimpLoader(const std::string &pFile);
		~RxAssimpLoader();

		bool Open3DFile(const std::string &pFile);

		void ExportTo(RxMesh *pMesh);

		void ExportTo(RxAnimMesh *pMesh);

		void Close3DFile();


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
		std::map<std::string,unsigned int> mapJointByName;
		RxAnimMesh * pAnimMesh;

		//---------------------------------------------------------------------------------------
		// Fonctions & data pour mesh animée et static
		//--------------------------------------------------------------------------------------
		void LoadGLTextures();		
		
		void aiToglmMat(const aiMatrix4x4 &aim, glm::mat4 &glmm);
		glm::vec3 aiToglm(const  aiVector3D &v);
		glm::quat aiToglm(const  aiQuaternion &q);






		unsigned int FindJointFromMesh(unsigned int id_mesh);

	
		mat4stack mTransform;

		// Creation d'une instance de la classe Importer
		Assimp::Importer *importer;

		// the global Assimp scene object
		const aiScene* paiScene;
		
		
		

		//map image filenames to textureIds
		std::map<std::string,RxTexture> textureFile;	
		std::map<std::string,RxTexture> normalTextureFile;
	



		RxFile cfile;

		std::string basepath;



};



#endif