//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXHEIGHTMAP_H_
#define _RXHEIGHTMAP_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif


#include <vector>
#include <noise/noise.h>
#include <noise/noiseutils.h>

#include "RxMesh.h"
#include "RxTools.h"
#include "RxVBO.h"
#include "RxShader.h"
#include "RxObject3D.h"
#include "RxQuadTree.h"
#include "RxLogger.h"
#include "RxMath3D.h"


//#define RX_HEIGHTMAP_VERT_FILE "shaders/hterrain.vert"
//#define RX_HEIGHTMAP_FRAG_FILE "shaders/hterrain.frag"

#define RX_HEIGHTMAP_VERT_FILE "shaders/hterrain3.vert"
#define RX_HEIGHTMAP_FRAG_FILE "shaders/hterrain3.frag"
//#define RX_HEIGHTMAP_VERT_FILE "shaders/item2d.vert"
//#define RX_HEIGHTMAP_FRAG_FILE "shaders/item2d.frag"


#define RX_GEOMETRY_VERT_FILE "shaders/geometry.vert"
#define RX_GEOMETRY_FRAG_FILE "shaders/geometry.frag"



//descripteur d'un noeud du terrain
struct RxNodeMap
{
	float height; //hauteur du noeud
	glm::vec3 pos;	  //position
	glm::vec3 color;	  //couleur
	glm::vec2 texCoord;//coordonnées texture associée
	glm::vec3 normal;  //normale associée
	GLuint idTex; //identifiant texture associée
	
	RxNodeMap()
	{
		height = 0.0f; idTex = 0;
	}


	void setH(float H){height = H; pos.y = H;}
	void setPosition(glm::vec3 p){pos = p; height = pos.y;}
	void setHorizontalPos(glm::vec2 p){pos.x = p.x; pos.z = p.y;}
	void setHorizontalPos(float x,float z){pos.x = x; pos.z = z;}
};



//classe de generation / affichage du terrain
class RxHeightMap:public RxMesh
{

	private:
	
		std::vector<RxNodeMap> hmap;	   //ensemble des noeuds formant notre terrain
		unsigned int w,h; //dimension horizontal (largeur, longueur)

		//origine en repere local du terrain est au indice (0,0)
		glm::vec3 pos;  //facteur de dimension et position du terrain
		
		//fonctions de traitement des données
		void ComputeNormals();
		void ComputeTriangles();
		void ComputeRandomTex();
		void ComputeVBO();
		void ComputeShader();
		


		//Méthode de génération du terrain, aléatoire, à partir d'une image
		bool HeightMapFile(const char * filename); //charge la géométrie du terrain par rapport à une image
		bool HeightMapPerlin(float hmin, float hmax, Rx2DPerlinNoise perlinNoise);
		void HeightMapRand(float hmin, float hmax);
		void SmoothFunction(int n); //n : taille du noyau de convolution

		GLuint RandTex(unsigned int max = 0); //génère un nombre aléatoire entre [0 - max]; si max = 0  => max correspond au nombre de textures
		GLuint GenCoordTexHeight(float height, float max = 30);
		int randomTexturing;  //generation aléatoire de la disposition des textures
		
		//accesseur tableau simple indice, double indice
		inline unsigned int Find_i(unsigned int x, unsigned int z);
		inline glm::ivec2 Find_xz(unsigned int i); 

		//variable d'état, si notre terrain est valide
		bool valide;        //terrain valide

		//shader utilisé pour l'affichage de la carte
		RxShader shadermap, geometryshader;
		std::string vertFile; //par defaut : "data/RxHeightMap.vert"
		std::string fragFile; //par defaut : "data/RxHeightMap.frag"

		
		/**************  quadTree - subdivision & optimisation terrain **********************/
		/*****quadTree******/
		RxQuadTree QTree; //Arbre QuadTree
		bool SubdivisionMapQTree(void *data, void **data0, void **data1, void **data2, void **data3); //fonction de subdivision
		
		typedef struct dataQuad
		{
			glm::ivec2 begin;
			glm::ivec2 end;
			bool visible;
		}DQuad;

		std::vector<DQuad> dataQTree; //vector de donnée.
		int minQuad; //valeur minimal du quad (longueur et hauteur);
		
		/*****Frustrum Optimisation******/
		//calcule les sections de terrain visible par le frustrum.
		void ComputeFrustrumOptimisation(glm::vec3 cpos, glm::vec3 cdir);
		std::vector<glm::ivec2> sections;// sections d'indices (i,i+n) des n vertexs visibles pour la section i

		//précision de la vérification de la visibilité de triangle par la camera.
		int step_tr; //par défaut 1/10ème du min(w,h).
		
		//variable de contrôle du frustrum pr le calcul des faces visible.
		float ratio_wh; //par défaut 4/3 (ex 1024/768)
		float fovy; //angle d'ouverture par défaut 70° 
		/**************************************************************/

		void DrawGeometryOnly();
		void DrawGeometryOnly(glm::vec3 cpos, glm::vec3 cdir);	


		void DrawAll();
		void DrawAll(glm::vec3 cpos, glm::vec3 cdir);
	public:
		RxHeightMap(void);
		~RxHeightMap(void);

		//génére notre terrain
		// A partir d'une image (HeightMap)
		bool Load(const char *filename, glm::vec3 _pos = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.0f));


		bool EnableGradientColor();
		bool ClearGradientColor();
		

		//Génération aléatoire.
		bool CreatePerlinMap(glm::vec3 _pos, glm::vec3 _scale, int _w, int _h, float hmin = 0, float hmax = 255, Rx2DPerlinNoise perlinNoise = Rx2DPerlinNoise(), int smooth = 0);
		bool CreateRandomMap(glm::vec3 _pos, glm::vec3 _scale, int _w, int _h, float hmin = 0, float hmax = 255, int smooth = 3);

		//Fonction de dessin
		void Draw(); //dessine le terrain entier
		void Draw(glm::vec3 cpos, glm::vec3 cdir); //dessine seulement les parties visibles par la caméra
	
		//Fonction permettant de définir les options d'optimisation pour Draw(vec3 cpos, vec3 cdir);
		//fovy : angle d'ouverture, ratio : rapport w/h du frustrum.
		//_step : définit la précision du calcul des faces visibles. 0 : valeur choisie automatiquement.(step = [1 ,min(w,h)]) 
		void setDrawOptions(float _fovy = 70, float _ratio = 1.33333333333f, unsigned int _step =0);

		/**************************************************************/
		//accesseur
		unsigned int getWidth() {return w;} 
		unsigned int getHeight(){return h;}	
		unsigned int getSize(){return w*h;} 
		float getRealSize(){return w*h*scale.x*scale.y;}
		//vec3 getScale(){return scale;}
		glm::vec3 getPosition(){return pos;}
		std::vector<RxNodeMap> &getHeightMap(){return hmap;}
		
		//std::vector<RxFace> &getTriangleMap(){return getFacesOfGroup(0);}
		std::vector<RxFace> &getTriangleMap(){return tSubMeshes[0].tFace;}


		
		//OPTIONS DU TERRAIN SUPPLEMENTAIRE
		//fontion à appeler avant la fontion compute
		void setShaderFile( const char* vert, const char * frag)
		{vertFile = vert;fragFile = frag;}
		
		//génère le texturage aléatoirement à partir de la texture
		void EnableRandomTexturing();
		
		glm::ivec3 getNearbyFaces(unsigned int k);

		glm::vec3 scale;
	
};

#endif