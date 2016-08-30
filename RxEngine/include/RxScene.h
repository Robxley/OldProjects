//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#pragma once

#ifndef _RXSCENE_H_
#define _RXSCENE_H_

#include <tinyxml.h>
#include <vector>
#include <string>

#include "RxConst.h"
#include "RxMath3D.h"
#include "RxActor.h"
#include "RxHeightMap.h"
#include "RxMesh.h"
#include "RxObject3D.h"
#include "RxShader.h"
#include "RxShadow.h"
#include "RxSkybox.h"
#include "RxWater.h"
#include "RxTexManager.h"
#include "RxTools.h"
#include "RxFile.h"
#include "RxLogger.h"
#include "RxAssimpLoader.h"




//#define DVERTSHADER "shaders/lighting.vert"
//#define DFRAGSHADER "shaders/lighting.frag"

#define DVERTSHADER "shaders/dot3.vert"
#define DFRAGSHADER "shaders/dot3.frag"


using namespace std;

enum 
{
	RX_DEFAULT_SCENE,
	RX_GEOMETRY_ONLY,
	RX_GEOMETRY_AND_TEXTURE,
	RX_SIMPLE_WATER_MODE,
	
};


class RxScene
{
	private:
		int flagWaterQuality; // 0 : eau sans effet, 1 : reflection seulement, 2 : reflection, refraction
		int flagShadowMode; //0 : pas d'ombre, 1 : shadow volume, 2 : shadow mapping

	private:
		vector<RxActor*> ptabActor;
		vector<bool> ptabActoritself;
		vector<RxItem3D*> ptabItem3D;
		vector<RxShader> tabShader;
		vector<RxLight> tabLight;
		vector<vector<vec3>> tabDataVec3;
	
		vector<vector<unsigned int>> tabDataUInt;

		RxShader dShader; //shader par défaut

		bool skyBoxitself;
		RxSkyBox *skyBox;
		
		RxShadowVolume shadowVolume;
		RxShadowMapping shadowMapping;

		RxPhysic * physicscene;
		bool physicsceneitself;

		RxSmartCamera * smartCam;
		RxSmartCamera *defSmartCam;

		bool smartCamitself;
		bool autoDraw;

		string filename;
		
		string name;

		unsigned int *elapseTime;
		bool isValide;
		//fonction d'initialisation
		void Init();

		
	
		//suppression des données
		void deleteTabItem3D(); 
		void deleteTabActor();
		void deleteData();

		const char *loadDataFrom(string filname){return NULL;}

		RxItem3D *findItem3D(unsigned int id);
		RxActor *findActor(unsigned int idxml);
	
		//permet de déterminer les parametres de lissage de trajectoire en fontion de str
		void getTrajectSmoothParameter(const char *str, unsigned int *nb_seg, unsigned int *mode);
		
		//fonction d'affichage
		void DrawActorsWithoutWater(int option = RX_DEFAULT_SCENE);
		void DrawWaters(int option = RX_DEFAULT_SCENE);
		void DrawShadows(int option = RX_DEFAULT_SCENE); 
		void DrawShadowVolume(int option );
        void DrawShadowMapping(int option );

		void DrawLight();
		void EnableLights();

		//fonction et sous fonction de lecture des données du fichier xml
		void TextureReader(TiXmlElement* pElem);

		void ShaderReader(TiXmlElement* pElem);

		void ItemReader(TiXmlElement* pElem);

		void DataReader(TiXmlElement* pElem);

		void ActorReader(TiXmlElement* pElem);
		void ActorApproxReader(RxActor * actor, string &approx);

		void SceneComponentReader(TiXmlElement* pElem);
		
		//données de lecture global
		//tableau de correspondance d'id xml/opengl
		vector<idXML2idGL> idtextab;
		vector<idXML2idGL> idshadertab;
		vector<idXML2idGL> iddatatab;
		vector<idXML2idGL> idActor;
		vector<idXML2idGL> idLight;

		//variable opengl fixe
		GLint max_gllights; //relève le nombre max de lumière opengl gérer par le hardware
		

		void (*OptionalDrawingFct)();
	public:

		RxScene();	
		~RxScene();

		bool Load(const char* _filename, RxPhysic * pscene = NULL, RxSmartCamera *cam =NULL);
		void Draw(int option = RX_DEFAULT_SCENE, RxSmartCamera *cam = NULL);
		void AddActor(RxActor *_actor);
		void Destroy();

		void setPtrElapstime(unsigned int *ptr){elapseTime = ptr;}
		void log(); //fct de débugage
		
		void CreateDefaultShader();
		RxLight* getLight(unsigned int ID){return &tabLight[ID]; }

		void ManualSkyBoxDraw();

		void setOptionalDrawing( void (*_OptionalDrawingFct)() )
		{OptionalDrawingFct = _OptionalDrawingFct;}


		RxActor *FindActorByName(const char *nameActor);
};


#endif