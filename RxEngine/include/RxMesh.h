//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXMESH_H_
#define _RXMESH_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <vector>
#include "RxObject3D.h"
#include "RxConst.h"
#include "RxVBO.h"
#include "RxLogger.h"
#include "RxShader.h"
#include "RxMath3D.h"


// -------------------------------
// Objet 3D
// -------------------------------

class RxMesh : public RxItem3D
{
	public:	
		typedef struct _RxSubMesh
		{
			RxMaterial			 nMaterial;	//material attaché au subMesh
			std::vector<RxFace>	 tFace;		//les faces constituant le subMesh
			unsigned int faceOffset;	//offset sur le element array pour les indices des faces
			unsigned int vertexOffset;		//offset sur les vertex arrays pour le mesh concerné
			_RxSubMesh():faceOffset(0),vertexOffset(0){}
		} RxSubMesh;
	public:

		//permet de rajouter un subMesh à l'ensemble des données. Attention apres le dernier appel de AddSubMesh, il est nécessaire de faire appel à ComputeMesh();
		//nb_vertex = nombre de vertices (!= nombre de vertices * taille du vertices). ex : vec3 tab[10] -> nb_vertex = 10 (!= 3*10);
		virtual bool AddSubMesh(GLuint nbFaces , const GLuint *ptFaces, GLuint nbVertices, const GLfloat * ptPositions, const GLfloat * ptNormals = NULL, const GLfloat * ptTexCoords = NULL, bool _formatTexCoord3 = false, const GLfloat *ptColors = NULL, bool _formatColor4 = 0, const RxMaterial *pMaterial = NULL);
	

		//Fonction à rappeler dans le cas d'un changement de flag pour que celui ci soit appliqué au mesh
		//il n'est pas nécessaire de faire appelle à cette fonction lors de la creation d'un mesh par les fonctions Load
		virtual bool ComputeMesh();

		//libère la mémoire des tableaux des vertices (tPosition.clear(), tNormal.clear()...)
		//le VBO est tjrs viable
		virtual void FreeArrayVertex();

		//vrai si ComputeMesh a été appelé et VBO du mesh OK
		virtual bool IsValide();
	
		//vide la mémoire de tout l'objet, données, vbo...
		virtual void Destroy();
	
		//affichage
		virtual void Draw();
		virtual void Draw(GLuint group);



		//Transformation sur le mesh définitivement en le multipliant par la matrice mat
		//Appeler avant ComputeMesh();
		void TransformMesh(glm::mat4 &mat); //Transformation sur les positions, normales et tangentes
		void ScaleMesh(glm::vec3 scale); //Transformation sur les positions
		void CenterDataToOrigin(glm::vec3 offset = glm::vec3(0));
		void NormalizeData(bool keepRatio = 1);	


		//Accesseur
		std::vector<glm::vec3>&	 getTabPosition();
		std::vector<glm::vec3>&	 getTabNormal();
		std::vector<glm::vec2>&	 getTabTexCoord2();
		std::vector<glm::vec3>&	 getTabTexCoord3();
		std::vector<glm::vec3>&  getTabColor3();
		std::vector<glm::vec4>&  getTabColor4();
		std::vector<glm::vec3>&  getTabTangent();
		std::vector<RxSubMesh>&  getTabSubMeshes();






		//applique la texture / le materiel à l'ensemble des groupes
		void setTexture(RxTexture &texture);
		void setTexture(GLuint id);
		void setTexture(std::string &filename);
		void setNormalMap(GLuint id);
		void setMaterial(RxMaterial &m);
		void setColor(glm::vec4 _color);

		//applique la texture, le material, couleur à un ensemble donnée
		void setTextureSubMesh(unsigned int idg, RxTexture &texture);
		void setTextureSubMesh(unsigned int idg, GLuint id);
		void setTextureSubMesh(unsigned int idg, std::string &filename);
		void setMaterialSubMesh(unsigned int idg, const RxMaterial &m);
		void setColorSubMesh(unsigned int idg, glm::vec4 &color);
		RxMaterial &getMaterialSubMesh(unsigned int idg = 0);
	
		RxMesh();
		~RxMesh();

	protected:

		bool LoadSubMesh(GLuint nbFaces, const GLuint *ptFaces, GLuint nbVertices, const GLfloat * ptPositions, const GLfloat * ptNormals , const GLfloat * ptTexCoords, bool _formatTexCoord3, const GLfloat *ptColors, bool _formatColor4, const RxMaterial *pMaterial);
	
		void ComputeBoundingBox();

		
		unsigned int getNbFaces();


	protected:


		std::vector<RxSubMesh>		tSubMeshes;
		unsigned int nbFaces; //nombre de faces de l'ensemble des SubMesh;
		
		
		RxVBO	vbo;
		bool computed;

		std::vector<glm::vec3>	tPosition;
		std::vector<glm::vec3>	tNormal;
		std::vector<glm::vec2>	tTexCoord2;
		std::vector<glm::vec3>	tTexCoord3;
		std::vector<glm::vec3>  tColor3;
		std::vector<glm::vec4>  tColor4;
		std::vector<glm::vec3>  tTangent;

		bool formatTexCoord3, formatColor4;

};

#endif

