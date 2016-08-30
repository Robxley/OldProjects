//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#



#ifndef _RXWATER_H_
#define _RXWATER_H_


#include <string>

#include "RxMath3D.h"
#include "RxObject3D.h"
#include "RxSimpleMesh.h"
#include "RxShader.h"
#include "RxFBO.h"
#include "RxCamera.h"
#include "RxConst.h"





using namespace std;

#define RX_REFLECT_AND_REFRACT	0
#define RX_REFLECT				1
#define	RX_TRANSPARENCY			2



#define RX_SIZE_PLANE 1000
//#define RX_WATER_VERT_FILE "shaders/waterdot3.vert"
//#define RX_WATER_FRAG_FILE "shaders/waterdot3.frag"

#define RX_WATER_VERT_FILE "shaders/water.vert"
#define RX_WATER_FRAG_FILE "shaders/water.frag"
//nom des variables uniformes du shader pour l'eau
#define RX_WATER_UNIFORM_NAME_REFLECTION_MAP "reflectionMap"
#define RX_WATER_UNIFORM_NAME_REFRACTION_MAP "refractionMap"
#define RX_WATER_UNIFORM_NAME_NORMAL_MAP "normalMap"
#define RX_WATER_UNIFORM_NAME_NOISE_MAP "noiseMap"
#define RX_WATER_UNIFORM_NAME_TIME "time"

class RxWater: public RxItem3D
{
	private:
		RxPlane waterPlane;
		RxSurface waterSurface;
		vec4 color;
		RxCamera *camera;
		mat4 reflectMatrix;
		
		char mode; // 0 : reflection + refraction  RX_REFRACT_AND_REFLECT  (default)
				   // 1 : reflection			   RX_REFLECT
				   // 2 : transparence			   RX_TRANSPARENCY    

		//definition du shader pour le rendu de l'eau
		RxShader waterShader;
		string vertFile, fragFile; //filename des fichiers fraguement et vertex
		//valeur des uniformes à passer au shader
		RxIDTexture normalMap, noiseMap;
		float wavetime;
		
		//definition du FBO utilisé
		RxFBO reflectionFBO;
		RxFBO refractionFBO;
		ivec2 sizeFBO;

		//variable de sauvegarde nécessaire à la restitution apres calcul de la reflexion/refraction
		GLint viewportSaver[4];
		GLfloat clearcolorSaver[4];
		
	public:
		RxWater();
		~RxWater();
		void Init(RxCamera *cam = NULL);
		
		void EnableReflection();
		void DisableReflection();
		void EnableRefraction();
		void DisableRefraction();

		void Draw();
		void Draw(vec3 cpos, vec3 dir);

		//seter
		/*void setSize(float w, float h);
		void setSize(vec2 s);
		void setPosition(vec3 p);
		void setPosition(float x, float y, float z);*/
		void setCamera(RxCamera *cam);
		void setNoiseMap(RxIDTexture id);
		void setNormalMap(RxIDTexture id);
		void setTime(float t);
		void setWaterPlane(RxPlane wp);
		void setWaterColor(float r, float g, float b, float a);
		void setWaterColor(vec4 c);
        

		//geter
		//float* getMatrixReflect(){return &reflectMatrix[0];}
		mat4 &getMatrixReflect(){return reflectMatrix;}
		RxIDTexture getReflectionTex();
		RxIDTexture getRefractionTex();
		vec4 getWaterColor();
		RxPlane getWaterPlane();
		RxSurface *getWaterSurface(){return &waterSurface;}
};

#endif