
#include "RxScene.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



RxScene::RxScene()
{
	
}
void RxScene::Init()
{
	glGetIntegerv(GL_MAX_LIGHTS,&max_gllights);
	isValide = 0;
	physicsceneitself = 0;
	physicscene = NULL;
	smartCam = defSmartCam = NULL;
	smartCamitself = 0;
	skyBoxitself = 0;
	skyBox = NULL;
	elapseTime = 0;
	autoDraw = true;
		//relève le nombres les variables opengls
	flagWaterQuality = 1; // 0 : eau sans effet, 1 : reflection seulement, 2 : reflection, refraction
	flagShadowMode = 1; //0 : pas d'ombre, 1 : shadow volume, 2 : shadow mapping

	shadowMapping.setCamera(smartCam);


	idtextab.clear();
	idshadertab.clear();


	OptionalDrawingFct = 0;

}
RxScene::~RxScene()
{
	Destroy();

}

bool RxScene::Load(const char* _filename, RxPhysic * pscene, RxSmartCamera *cam )
{
	Init();
	if(!_filename) return 0;
	Destroy();
	if(pscene == NULL)
	{
		physicsceneitself = 1;
		physicscene = new RxPhysic;
	}
	else
		physicscene = pscene;

	if(cam == NULL)
	{
		smartCamitself = 1;
		smartCam = defSmartCam = new RxSmartCamera;
	
	}
	else
		smartCam = defSmartCam = cam;

	filename = _filename;
	
	//lecture du fichier XML
	TiXmlDocument doc(filename.data());
	if(!doc.LoadFile()){ //erreur d'ouverture
		RXLOGGER << "[RxScene] erreur lors du chargement" << endl;
		RXLOGGER << "[RxScene] error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
	    return 0;
	}
	RXLOGGER << "[RxScene] Chargement effectue "<<endl;
    
		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(NULL);
		const char *ptr;
		// block rxscene: lecture nom de la scene
		
			pElem=hDoc.FirstChildElement().Element();
		
			if (!pElem) return 0;
			if(ptr = pElem->Attribute("name") )
				name = ptr; //enregistre le nom de la scene

			// sauvegarde la racine
			hRoot=TiXmlHandle(pElem);
		
//////////////////////////////////////////////////////////////////////////////////////
		// block texturelist: lecture et chargement des images
//////////////////////////////////////////////////////////////////////////////////////
		
			pElem=hRoot.FirstChild( "texturelist" ).FirstChildElement().Element();
			TextureReader( pElem );
		
//////////////////////////////////////////////////////////////////////////////////////
	//block shaderlist
//////////////////////////////////////////////////////////////////////////////////////
		
			pElem=hRoot.FirstChild( "shaderlist" ).FirstChild("shader").Element();
			ShaderReader( pElem );
		
//////////////////////////////////////////////////////////////////////////////////////
		//block itemlist
//////////////////////////////////////////////////////////////////////////////////////
		
			pElem = hRoot.FirstChild( "itemlist" ).FirstChildElement().Element(); //on se place sur le premier item
			ItemReader(pElem);

//////////////////////////////////////////////////////////////////////////////////////
		//block data list
//////////////////////////////////////////////////////////////////////////////////////
			
			pElem = hRoot.FirstChild( "datalist" ).FirstChildElement().Element();
			DataReader(pElem);
			
//////////////////////////////////////////////////////////////////////////////////////
		//block actor list
//////////////////////////////////////////////////////////////////////////////////////
			
			pElem = hRoot.FirstChild( "actorlist" ).FirstChildElement().Element(); //on se place sur le premier actor
			ActorReader(pElem);

//////////////////////////////////////////////////////////////////////////////////////
		//block light list
//////////////////////////////////////////////////////////////////////////////////////

			pElem = hRoot.FirstChild( "scene" ).FirstChildElement().Element(); //on se place sur le premier element
			SceneComponentReader(pElem);

	EnableLights();

	isValide = 1;
	return 1;
}
RxItem3D *RxScene::findItem3D(unsigned int id)
{
	unsigned int i;
	for(i = 0; i < ptabItem3D.size(); i++)
	{
		if(id == ptabItem3D[i]->getID()) return ptabItem3D[i];
	}
	return NULL;
}


void RxScene::Draw(int option, RxSmartCamera *cam)
{
	
	if(cam == NULL) smartCam = defSmartCam;
	else smartCam = cam;

	if(option == RX_DEFAULT_SCENE && !RxItem3D::GeometryOnlyFlag)
	{
		//glCullFace(GL_BACK); //suppression des faces arrières
			
		if( skyBox!=NULL && autoDraw)
			skyBox->Draw(smartCam->pos.x,smartCam->pos.y,smartCam->pos.z);

		EnableLights();
		DrawActorsWithoutWater(option);
		if(OptionalDrawingFct)
			OptionalDrawingFct();

		DrawWaters(option);
		//glDisable(GL_CULL_FACE);
			DrawShadows(option);
		//glEnable(GL_CULL_FACE);
	}
	else if(option == RX_GEOMETRY_ONLY || RxItem3D::GeometryOnlyFlag)
	{
		    //dessine tous les acteurs
	    unsigned int kActor;
		for(kActor = 0; kActor < ptabActor.size(); kActor++)
		{
			//ptabActor[kActor]->Draw(smartCam->pos,smartCam->dir);
			ptabActor[kActor]->Draw();
		}

		if(OptionalDrawingFct)
			OptionalDrawingFct();
	}
	autoDraw = true;


}

void RxScene::ManualSkyBoxDraw()
{
	
			  if( skyBox && defSmartCam)
				skyBox->Draw(defSmartCam->pos.x,defSmartCam->pos.y,defSmartCam->pos.z);

		autoDraw = false;
}
void RxScene::DrawLight()
{
	unsigned int i;
	glEnable(GL_LIGHTING);
	unsigned int nb_light = min( (int) tabLight.size(), max_gllights);
	glPushMatrix();
	
	for(i = 0; i < nb_light ; i++)
	{
		glLightfv(GL_LIGHT0+i, GL_POSITION, tabLight[i].pos);
	}
	glPopMatrix();
}
void RxScene::EnableLights()
{
	unsigned int i;
	glEnable(GL_LIGHTING);
	unsigned int nb_light = min( (int) tabLight.size(), max_gllights);
	glPushMatrix();
	
	for(i = 0; i < nb_light ; i++)
	{
		tabLight[i].EnableGLContext(i);
	}
	glPopMatrix();
	
}
void RxScene::DrawActorsWithoutWater(int option)
{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);

		unsigned int kShader,kActor;
	if(option != RX_SIMPLE_WATER_MODE)
	{
		//pour chacun des shaders
		for(kShader = 0; kShader < tabShader.size(); kShader++)
		{
			unsigned int idShader = tabShader[kShader].Enable();
			//pour chacun des actors correspondants au shader sauf RX_WATER_ITEM3D
			for(kActor = 0; kActor < ptabActor.size(); kActor++)
			{
				if(ptabActor[kActor]->getItem3D()->getIdShader() == idShader &&
				   ptabActor[kActor]->getItem3D()->getType() != RX_WATER_ITEM3D)
				{
					//glBindTexture(GL_TEXTURE_2D,ptabActor[kActor]->getItem3D()->getTexture());
					ptabActor[kActor]->Draw(smartCam->pos, smartCam->dir);
				}
			}

		}
	}



	  dShader.Enable();
	//glUseProgram(0);
	//glDisable(GL_LIGHTING);
				
			//applique le shader par défaut pour les acteurs restant
			for(kActor = 0; kActor < ptabActor.size(); kActor++)
			{
				
				if( ((ptabActor[kActor]->getItem3D()->getIdShader() == 0) || (option == RX_SIMPLE_WATER_MODE)) && 
				   ptabActor[kActor]->getItem3D()->getType() != RX_WATER_ITEM3D)
				{
					//GLuint idtex = ptabActor[kActor]->getItem3D()->getTabIdTex()[0];*/
						//glBindTexture(GL_TEXTURE_2D,ptabActor[kActor]->getItem3D()->getTexture());

						if( dShader.Enable())
						{
								glActiveTexture (GL_TEXTURE1);
								glUniform1i(dShader.getUniform("normalMap"), 1);

 								glActiveTexture (GL_TEXTURE0);
								glUniform1i(dShader.getUniform("colorMap"), 0);

						}
						ptabActor[kActor]->Draw(smartCam->pos, smartCam->dir);
				}
			}
		glUseProgram(0);
}

void RxScene::DrawWaters(int option)
{

	unsigned int kActor;
	for(kActor = 0; kActor < ptabActor.size(); kActor++)
	{
		if( ptabActor[kActor]->getItem3D()->getType() == RX_WATER_ITEM3D)
		{
			RxWater * water = ((RxWater *)ptabActor[kActor]->getItem3D());
			if(flagWaterQuality)
			{
				 
				 glFrontFace(GL_CW); //inversion des données dc des faces avant arrière	
				 glPushMatrix();
					 water->EnableReflection();
						 glColor3f(1.0f,1.0f,1.0f);
						 vec3 tmp =  water->getMatrixReflect()*smartCam->pos;
						 glDisable(GL_CLIP_PLANE0);
						 if(skyBox != NULL)
							skyBox->Draw(tmp.x,tmp.y,tmp.z);

						 glEnable(GL_CLIP_PLANE0);

						 DrawActorsWithoutWater(RX_SIMPLE_WATER_MODE);	//on redessine les acteurs à reflechir

					 water->DisableReflection();
				 glPopMatrix();
				 glFrontFace(GL_CCW); //inversion des données dc des faces avant arrière	
			}
			if(flagWaterQuality == 2)
			{
			
				 //refraction
				 glPushMatrix();
					 water->EnableRefraction();
						 glColor3f(1.0f,1.0f,1.0f);
						  DrawActorsWithoutWater(RX_SIMPLE_WATER_MODE);	//on redessine les acteurs à refracter
					 water->DisableRefraction();
				 glPopMatrix();	
			}
			 //glEnable(GL_BLEND);
			 static float t = 0;
			 
			 if(elapseTime)
			 {  
				 t += (*elapseTime)/60.0f;
				 water->setTime(t);
			 }
			 else  
			 {
				 static float t = 0;
				 t+=0.5;
				 water->setTime(t);
			 }
			 water->Draw(smartCam->pos,smartCam->dir);
			 
		}
	}
}
void RxScene::DrawShadows(int option )
{
	if(flagShadowMode == 1)
		DrawShadowVolume(option);
	else if(flagShadowMode == 2)
		DrawShadowMapping(option);
}
void RxScene::DrawShadowMapping(int option )
{
	unsigned int  kActor;
	
shadowMapping.setCamera(smartCam);
glDisable(GL_TEXTURE_2D);
glDisable(GL_LIGHTING);
glEnable(GL_BLEND);
glUseProgram(0);
glColor4f(1.0,1.0,1.0,1.0);

		RxItem3D::GeometryOnlyFlag = 1;
		shadowMapping.StartShadowMap(); //dessin des ombres à partir de la shadow map
			for(kActor = 0; kActor < ptabActor.size(); kActor++)
			{
				//if(ptabActor[kActor]->getItem3D()->getType() != RX_WATER_ITEM3D)
				{
						//ptabActor[kActor]->Draw(smartCam->pos, smartCam->dir);
						ptabActor[kActor]->Draw();
				}
			}
			if(OptionalDrawingFct)
				OptionalDrawingFct();
		shadowMapping.EndShadowMap();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);


		RxItem3D::GeometryOnlyFlag = 1;
		shadowMapping.EnableShadow(); //dessin des ombres à partir de la shadow map
			for(kActor = 0; kActor < ptabActor.size(); kActor++)
			{
				//if(ptabActor[kActor]->getItem3D()->getType() != RX_WATER_ITEM3D)
				{
						//ptabActor[kActor]->Draw(smartCam->pos, smartCam->dir);
						ptabActor[kActor]->Draw();
				}
			}
			if(OptionalDrawingFct)
				OptionalDrawingFct();
		shadowMapping.DisableShadow();
		RxItem3D::GeometryOnlyFlag = 0;

}
void RxScene::DrawShadowVolume(int option )
{
	unsigned int kLight, kActor;
	

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_FLAT);
	//glUseProgram(0);
	

	glPushMatrix();
		
		for(kLight = 0; kLight < tabLight.size(); kLight++)
		{
			shadowVolume.UpdatedLight(kLight, tabLight[kLight].pos, tabLight[kLight].dir);
		}
		unsigned int kSActor = 0;
		for(kActor = 0; kActor < ptabActor.size(); kActor++)
		{
			//verifie que c'est bien acteur que l'on a rajouté
			if(shadowVolume.IsValideItem(ptabActor[kActor]->getItem3D() ))
			{
			//	if(!ptabActor[kActor]->getNxActor()->isSleeping()) //si l'acteur n'a pas bougé dans pas besoin de mettre à jour le volume d'ombre
						shadowVolume.UpdatedObject(kSActor,ptabActor[kActor]->getGLmat());
				kSActor++;
			}

		}
		shadowVolume.Draw();
		
		//_DEBUG
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glEnable(GL_BLEND);
		glColor4f(1.0,0.0,0.0,0.0);
		shadowVolume.DrawShadowVolumes();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

	glPopMatrix();
	
}
void RxScene::Destroy()
{
	
	

		tabShader.clear();
		tabLight.clear();
		//RxTexManager::Instance().Clear();
		shadowVolume.Clear();
		


		deleteTabItem3D();
		deleteTabActor();
		deleteData();

		if(physicsceneitself)
			delete physicscene;

		physicsceneitself = 0;
		physicscene = NULL;

		if(smartCamitself)
			delete defSmartCam;

		smartCamitself = 0;
		smartCam =defSmartCam= NULL;

		if(skyBoxitself)
			delete skyBox;

		skyBox = NULL;
}
void RxScene::deleteTabItem3D()
{
	//suppression 1 par 1 des items
	for(unsigned int i = 0 ; i < ptabItem3D.size() ; i++)
	{
		delete ptabItem3D[i];
	}
	ptabItem3D.clear();
}
void RxScene::deleteTabActor()
{
	//suppression 1 par 1 des acteurs
	for(unsigned int i = 0 ; i < ptabActor.size(); i++)
	{
		if(ptabActoritself[i])   //détruit que les acteurs que la scène a chargée
			delete ptabActor[i];
	}	
	ptabActor.clear();
	ptabActoritself.clear();

}
void RxScene::deleteData()
{
	for(unsigned int i = 0; i < tabDataVec3.size(); i++)
	{
		tabDataVec3[i].clear();
	}
	tabDataVec3.clear();
}
void RxScene::getTrajectSmoothParameter(const char *str, unsigned int *nb_seg, unsigned int *mode)
{
	char c;
	int i;
	sscanf(str,"%c-%d",&c,&i);
	(*nb_seg) = i;

	if(c == 'B' || c =='b')
		(*mode) = 0;
	else if(c == 'h' || c =='H')
		(*mode) = 1;
	else
		(*mode) = 0;
}
void RxScene::log()
{
	RXLOGGER<<"Info log Scene : "<<filename.data()<<endl
		<<"Nom de la scene : "<<name.data()<<endl
		<<"Nombre d'objets 3D : "<<ptabItem3D.size()<<endl
		<<"Nombre d'acteur : "<<ptabActor.size()<<endl
		<<"Nombre de Shader : "<<tabShader.size()<<endl
		<<"Nombre de lumieres : "<<tabLight.size()<<endl;
}

void RxScene::CreateDefaultShader()
{

/* chargement de notre programme defaut */
	if(dShader.Load(DVERTSHADER,DFRAGSHADER,"DefaultShaderScene"))
	{
		dShader.CreateUniformVar("colorMap");
		dShader.CreateUniformVar("normalMap");
	}
}

void RxScene::TextureReader(TiXmlElement* pElem)
{
	const char *ptr;
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour chacune des textures
	{
	
		string value = pElem->Value();
		if(value.compare("texture") == 0)
		{
			//relève les infos de la texture
			string texFile;
			if(ptr = pElem->Attribute("filename"))
			  texFile = ptr;
			
			idXML2idGL id;
			pElem->QueryIntAttribute("id", &id.idxml);
			
			//enregistre les données et charge la texture
			id.idgl = RxTexManager::Instance().LoadTex(texFile.data());
			
			idtextab.push_back(id);
		}
		else if(value.compare("multitexture")==0)
		{
			//relève les infos de la texture
			string name;
			if(ptr = pElem->Attribute("name"))
			  name = ptr;
			
			ivec2 format(0,0);
			if(ptr = pElem->Attribute("format"))
				format = str2ivec2(ptr);

			idXML2idGL id;
			pElem->QueryIntAttribute("id", &id.idxml);

			vector<string> filenames;
			//relève les textures
			TiXmlElement * pElem2 = pElem->FirstChildElement();
			for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) //pour actor
			{
				if(ptr = pElem2->Attribute("filename"))
				{
					filenames.push_back(ptr);
				}
			}
			
			if( (format.x <= 0) || (format.y <= 0) )
			{
				int d = (int)sqrt((float)filenames.size());
				if( d*d == filenames.size())
					format = ivec2(d,d);
				else
					format = ivec2(d+1,d+1);
			}

			format.x = power_of_two(format.x);
			format.y = power_of_two(format.y);


			//enregistre les données et charge la texture
			id.idgl = RxTexManager::Instance().CreateMultiTex(filenames,format.x, format.y,name.c_str());
			
			idtextab.push_back(id);
		}
		else if(value.compare("arraytexture")==0)
		{
			//relève les infos de la texture
			string name;
			if(ptr = pElem->Attribute("name"))
			  name = ptr;
			


			idXML2idGL id;
			pElem->QueryIntAttribute("id", &id.idxml);

			vector<string> filenames;
			//relève les textures
			TiXmlElement * pElem2 = pElem->FirstChildElement();
			for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) //pour actor
			{
				if(ptr = pElem2->Attribute("filename"))
				{
					filenames.push_back(ptr);
				}
			}
			

			//enregistre les données et charge la texture
			id.idgl = RxTexManager::Instance().CreateArrayTex(filenames,name.c_str());
			
			idtextab.push_back(id);
		}
	}
}



void RxScene::ShaderReader(TiXmlElement* pElem)
{
	tabShader.clear();
	const char *ptr;
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour chacune des textures
	{
		string frag, vert, unif, sha, shaname; //valeur à relever
		idXML2idGL id;
		//on relève les informations du shader
		if ( ptr = pElem->Attribute("filefrag") )
			frag =  ptr; 
		
		if( ptr = pElem->Attribute("filevert") )
			vert = ptr; 
			
		if( ptr = pElem->Attribute("fileuniform") )
			unif = ptr;

		if( ptr = pElem->Attribute("file") )
			sha = ptr;

		if( ptr = pElem->Attribute("id") )
			shaname = ptr;	
		
		pElem->QueryIntAttribute("id", &id.idxml);
		//enregistrement du shader
		RxShader tmp;
		if(sha.empty())
		  id.idgl = tmp.Load(vert.data(), frag.data(),shaname.data()); //on charge le shader
		else
			id.idgl = tmp.Load(sha.data(),shaname.data());  //a ecrire

		idshadertab.push_back(id);
		tabShader.push_back(tmp);
		
		
	} 
}

void RxScene::ItemReader(TiXmlElement* pElem)
{
	const char *ptr;
	deleteTabItem3D(); //suppression des item3d deja créés
    for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour item
	{
		string value = pElem->Value();
		int defidtex = 0;  int defidsha = 0;
		///////////////////////////////////////////////////////////////////////////////////
		//////itemdefault////////////////////////////////////////////////////////////////
		if(value.compare("itemdefault")==0)
		{
			pElem->QueryIntAttribute("idtex",&defidtex); //relève la valeur si elle existe

			defidtex = xml2gl(defidtex, idtextab); //recherche la valeur opengl correspondante

			pElem->QueryIntAttribute("idshader",&defidsha); //relève la valeur si elle existe	
			
			defidsha = xml2gl(defidsha, idshadertab); //recherche la valeur opengl correspondante

		}
		//////////////////////////////////////////////////////////////////////////////////
		/////////plane////////////////////////////////////////////////////////////////////
		else if(value.compare("plane")==0)
		{
			RxPlane *plane = new RxPlane; if(!plane) continue;

			if(ptr = pElem->Attribute("equation")) //relève la valeur si elle existe
				plane->set( str2vec4(ptr) );
			
			int idtex = defidtex;
			if(pElem->QueryIntAttribute("idtex",&idtex)== TIXML_SUCCESS)
			{
				plane->setTexture( xml2gl(idtex, idtextab) );//ne pas oublier de mettre l'id opengl et non xml
			}
			else
				plane->setTexture( defidtex);
			
			int idShader = defidsha;
			//enregistre le shader associé
			pElem->QueryIntAttribute("idshader",&idShader);
			plane->setIdShader( xml2gl(idShader, idshadertab) ); //ne pas oublier de mettre l'id opengl et non xml
			
			int id = 0;
			pElem->QueryIntAttribute("id",&id);
			plane->setID(id); 
			
			ptabItem3D.push_back(plane);

		}
		//////////////////////////////////////////////////////////////////////////////////
		/////////sphere///////////////////////////////////////////////////////////////////
		else if(value.compare("sphere")==0)
		{
			RxSphere *sphere = new RxSphere; if(!sphere) continue;
			//enregistre le rayon
			float radius;

			if(pElem->QueryFloatAttribute("radius",&radius) == TIXML_SUCCESS)//relève la valeur si elle existe
				sphere->set(radius);

			int idtex = defidtex;
			if(pElem->QueryIntAttribute("idtex",&idtex)== TIXML_SUCCESS)
			{
				sphere->setTexture( xml2gl(idtex, idtextab) );//ne pas oublier de mettre l'id opengl et non xml
			}
			else
				sphere->setTexture(defidtex);

			int idShader = defidsha;
			//enregistre le shader associé
			pElem->QueryIntAttribute("idshader",&idShader);
			sphere->setIdShader(xml2gl(idShader, idshadertab));

			int id;
			//enregistre l'id
			if(pElem->QueryIntAttribute("id",&id)== TIXML_SUCCESS)
				sphere->setID(id);

			ptabItem3D.push_back(sphere);	//ajoute l'objet
	
		}
		//////////////////////////////////////////////////////////////////////////////////
		///////cube////////////////////////////////////////////////////////////////////
		else if(value.compare("cube")==0)
		{
			RxCube *cube = new RxCube; if(!cube) continue;
			
			//enregistre la taille
			if(ptr = pElem->Attribute("size")) //relève la valeur si elle existe
			cube->setSize( str2vec3(ptr) );
			
			//enregistre les textures
			int idtex = defidtex;
			if(pElem->QueryIntAttribute("idtex",&idtex)== TIXML_SUCCESS)
			{
				cube->setTexture( xml2gl(idtex, idtextab) );//ne pas oublier de mettre l'id opengl et non xml
			}
			else
				cube->setTexture(defidtex);

			int idShader = defidsha;
			//enregistre le shader associé
			pElem->QueryIntAttribute("idshader",&idShader);
			cube->setIdShader(xml2gl(idShader, idshadertab));

			int id;
			//enregistre l'id
			if(pElem->QueryIntAttribute("id",&id)== TIXML_SUCCESS)
				cube->setID(id);

			ptabItem3D.push_back(cube);

		}
		//////////////////////////////////////////////////////////////////////////////////
		////////capsule/////////////////////////////////////////////////////////////////////
		else if(value.compare("capsule")==0)
		{
			RxCapsule *capsule = new RxCapsule; if(!capsule) continue;
			float radius, height;
			//enregistre la taille
			if(pElem->QueryFloatAttribute("radius",&radius) == TIXML_SUCCESS)
				capsule->setRadius(radius); //relève la valeur si elle existe

			if(pElem->QueryFloatAttribute("height",&height) == TIXML_SUCCESS)
				capsule->setHeight(height);
			
			int idtex = defidtex;
			if(pElem->QueryIntAttribute("idtex",&idtex)== TIXML_SUCCESS)
			{
				capsule->setTexture( xml2gl(idtex, idtextab) );//ne pas oublier de mettre l'id opengl et non xml
			}
			else
				capsule->setTexture(defidtex);

			int idShader = defidsha;
			//enregistre le shader associé
			pElem->QueryIntAttribute("idshader",&idShader);
			capsule->setIdShader(xml2gl(idShader, idshadertab));

			int id;
			//enregistre l'id
			if(pElem->QueryIntAttribute("id",&id)== TIXML_SUCCESS)
				capsule->setID(id);

			ptabItem3D.push_back(capsule);
		}
		//////////////////////////////////////////////////////////////////////////////////
		/////////mesh/////////////////////////////////////////////////////////////////////
		else if(value.compare("mesh")==0)
		{
			RxMesh *mesh = new RxMesh; if(!mesh) continue;
			
			int normalise = 0;
			pElem->QueryIntAttribute("normalise",&normalise);
			RxMesh::FlagNormaliseMesh(normalise);


			//enregistre la taille
			if(ptr = pElem->Attribute("scale")) //relève la valeur si elle existe
				mesh->setScale( str2vec3(ptr) );
			
			

			if(ptr = pElem->Attribute("filename")) 
			{
				RxFile file = ptr;
				RxAssimpLoader assimploader(file.Fullname());
				if(!assimploader.Convert2StaticRxMesh(*mesh))
				{
					RXLOGGER<<"[ERREUR] impossible de charger le mesh : "<<ptr<<endl;
					delete mesh;
					continue; 
				}
				mesh->ComputeMesh();
			}
			else
			{
				RXLOGGER<<"[ERREUR] impossible de charger le mesh : "<<ptr<<endl;
				delete mesh;
				continue;
			}

			int idtex;
			if(pElem->QueryIntAttribute("idtex",&idtex)== TIXML_SUCCESS)
			{
				mesh->setTexture( xml2gl(idtex, idtextab) );//ne pas oublier de mettre l'id opengl et non xml
			}



			int idShader = defidsha;

			//enregistre le shader associé
			pElem->QueryIntAttribute("idshader",&idShader);
			mesh->setIdShader(xml2gl(idShader, idshadertab));

			int id;
			//enregistre l'id
			if(pElem->QueryIntAttribute("id",&id)== TIXML_SUCCESS)
				mesh->setID(id);

			ptabItem3D.push_back(mesh);
		}
		//////////////////////////////////////////////////////////////////////////////////
		///////heightmap////////////////////////////////////////////////////////////////////
		else if(value.compare("heightmap")==0)
		{
			RxHeightMap * hmap = new RxHeightMap;
			
			vec3 scale = vec3(1.0f,1.0f,1.0f);
			vec3 pos = vec3(0.0f,0.0f,0.0f);
			/*if(ptr = pElem->Attribute("position"))
				pos=str2vec3(ptr);*/

			if(ptr = pElem->Attribute("scale"))
				scale=str2vec3(ptr);


			int idtex = defidtex;
			if(pElem->QueryIntAttribute("idtex",&idtex)== TIXML_SUCCESS)
			{
				hmap->setTexture( xml2gl(idtex, idtextab) );//ne pas oublier de mettre l'id opengl et non xml
			}
			else
				hmap->setTexture( defidtex );
			

			 //récupère un pointeur vers la description de la 1ere image du terrain
			RxTexture * texture = &hmap->getTexture();
			
			if(texture!=NULL &&  texture->type == RX_ARRAY_TEX)
			{
				hmap->EnableRandomTexturing();
			}

			if(ptr = pElem->Attribute("filename")) 
			{
				if(!hmap->Load(ptr,pos,scale))//charge le terrain s'il existe
				{
					delete hmap;
					continue; 
				}
			}
			else if(ptr = pElem->Attribute("perlin"))
			{

				int map_w = 32, map_h = 32, min_lvl = 0, max_lvl = 255, nb_octave = 4, smooth = 3;
				int step = -1;
				float persistence = 0.5f;

				pElem->QueryIntAttribute("width",&map_w);
				map_h = map_w;
				pElem->QueryIntAttribute("height",&map_h);
				pElem->QueryIntAttribute("max_level",&max_lvl);
				pElem->QueryIntAttribute("min_level",&min_lvl);
				pElem->QueryIntAttribute("nb_octave",&nb_octave);
				pElem->QueryIntAttribute("smooth",&smooth);
				pElem->QueryIntAttribute("step",&step);
				pElem->QueryFloatAttribute("persistence",&persistence);

				if(!hmap->CreatePerlinMap(pos,scale,map_w, map_h, min_lvl,max_lvl, nb_octave, step, persistence,smooth))
				{
					delete hmap;
					continue; 
				}
			}
			else if(ptr = pElem->Attribute("random"))
			{

				int map_w = 32, map_h = 32, min_lvl = 0, max_lvl = 255, smooth = 3;
				
				pElem->QueryIntAttribute("width",&map_w);
				map_h = map_w;
				pElem->QueryIntAttribute("height",&map_h);
				pElem->QueryIntAttribute("max_level",&max_lvl);
				pElem->QueryIntAttribute("min_level",&min_lvl);

				if(!hmap->CreateRandomMap(pos,scale,map_w, map_h, min_lvl,max_lvl, smooth))
				{
					delete hmap;
					continue; 
				}
			}
			else 
			{
					RXLOGGER<<"[ERREUR] impossible de charger la heightmap : "<<ptr<<endl;
					delete hmap;
					continue; 
			}

			int idShader = defidsha;
			pElem->QueryIntAttribute("idshader",&idShader); 
			hmap->setIdShader(xml2gl(idShader, idshadertab));

			int id;
			if(pElem->QueryIntAttribute("id",&id)== TIXML_SUCCESS)
				hmap->setID(id);
			
			ptabItem3D.push_back(hmap);
		}
		//////////////////////////////////////////////////////////////////////////////////
		//////waterplane////////////////////////////////////////////////////////////////////
		else if(value.compare("waterplane")==0)
		{
			RxWater *water = new RxWater; if(!water ) continue;
			
			vec4 equ = vec4(0.0f,1.0f,0.0f,0.0f);
			if(ptr = pElem->Attribute("equation"))
				equ = str2vec4(ptr);
			
			int idShader = defidsha;
			pElem->QueryIntAttribute("idshader", &idShader); 
			water->setIdShader(xml2gl(idShader, idshadertab));

			int id;
			if(pElem->QueryIntAttribute("id",&id)== TIXML_SUCCESS)
				water->setID(id);
			
			water->setNoiseMap(RxTexManager::Instance().LoadTex("data/water.png"));
			water->setNormalMap(RxTexManager::Instance().LoadTex("data/water.png"));
			RxPlane plane;
			plane.set(equ);

			water->setWaterPlane(plane);
			water->Init((RxCamera*) smartCam);

			ptabItem3D.push_back(water);

		}
		//////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////

	} 
}

void RxScene::DataReader(TiXmlElement* pElem)
{

	deleteData(); 
	const char* ptr;
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour actor
	{
		string value = pElem->Value();
		///////////////////////////////////////////////////////////////////////////////////
		//////trajectory////////////////////////////////////////////////////////////////
		if(value.compare("trajectory")==0) //enregistre les valeurs par défaut
		{
			idXML2idGL id; string type,strdata;
			vector<vec3> data;
			pElem->QueryIntAttribute("id",&id.idxml);
			
			if(ptr = pElem->Attribute("type"))
				type = ptr;
			
			if(type.compare("vertex")==0)
			{
				strdata = pElem->GetText();
			}
			else if(type.compare("file")==0)
			{
				if(ptr = pElem->Attribute("filename") )
				   strdata = loadDataFrom(ptr);
				else continue;
			}
			else continue;
	
			data = str2vectorvec3(strdata);

			id.idgl = tabDataVec3.size();
			iddatatab.push_back(id);
			tabDataVec3.push_back(data);
		}
	}//fin for
}

void RxScene::ActorReader(TiXmlElement* pElem)
{
	//on supprime tout si une scène à déjà été chargée
	deleteTabActor();

	//ensemble des valeurs par défaut
	unsigned int defgrp = 0;
	float defmass = 1.0f,defcelastic = 0.5f, defsfriction = 1.0f, defdfriction = 1.0f;
	vec3 defpos = vec3(0.0f,0.0f,0.0f);
	quat defquat = quat(0.0f,0.0f,0.0,0.0f);
	int defgroup = 0;
	string defapprox;
	string name;

	const char* ptr;

	//debut de la lecture des acteurs
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour actor
	{
		string value = pElem->Value();
		///////////////////////////////////////////////////////////////////////////////////
		//////actordefault////////////////////////////////////////////////////////////////
		if(value.compare("actordefault")==0) //enregistre les valeurs par défaut
		{
			pElem->QueryFloatAttribute("mass",&defmass);
			pElem->QueryFloatAttribute("celastic",&defcelastic);
			pElem->QueryFloatAttribute("sfriction",&defsfriction);
			pElem->QueryFloatAttribute("dfriction",&defdfriction);
			pElem->QueryIntAttribute("group",&defgroup);
			if(ptr = pElem->Attribute("position"))
				defpos = str2vec3(ptr);
			if(ptr = pElem->Attribute("quat"))
				defquat = str2quat(ptr);
			if(ptr = pElem->Attribute("approx"))
				defapprox = ptr;

		}
		///////////////////////////////////////////////////////////////////////////////////
		//////dynamic /static   ////////////////////////////////////////////////////////////////
		else if(value.compare("dynamic")==0 || value.compare("static")==0 )
		{
			bool dynamic_actor = RX_DYNAMIC;
			if(value.compare("static")==0)
				dynamic_actor = RX_STATIC;

			int iditem = 0, group = defgroup, idact;
			float mass=defmass, celastic=defcelastic, sfriction=defsfriction ,dfriction=defdfriction; 
			vec3 pos = defpos; quat quaternion = defquat;
			string approx = defapprox;
			name.clear();
			
			pElem->QueryIntAttribute("iditem",&iditem);

			RxItem3D *item = findItem3D(iditem);
			if(item == NULL ) continue;
			
			pElem->QueryFloatAttribute("mass",&mass);
			pElem->QueryFloatAttribute("celastic",&celastic);
			pElem->QueryFloatAttribute("sfriction",&sfriction);
			pElem->QueryFloatAttribute("dfriction",&dfriction);
			pElem->QueryIntAttribute("group",&group);
			pElem->QueryIntAttribute("id",&idact);
			
			if(ptr = pElem->Attribute("name"))
				name = ptr;

			RxActor * actor = new RxActor; if(!actor) continue;
			actor->CreateActor(item,mass,dynamic_actor,true);

			if(name.size()) actor->setName(name);

			if(item->getType() != RX_WATER_ITEM3D)
			{

				if(ptr = pElem->Attribute("position"))
				{
					pos = str2vec3(ptr);
					actor->setInitGlobalPosition(pos);
				}
				if(ptr = pElem->Attribute("quat"))
				{
					quaternion = str2quat(ptr);
					actor->setOrientation(quaternion.to_matrix());	
				}
				//actor->getNxActor()->setLinearVelocity(NxVec3(0.0,0.0,0.0));
				actor->setMaterial(celastic,sfriction,dfriction);
			}

			if(ptr = pElem->Attribute("approx"))
					approx = ptr;
	
			if(approx.empty()) //aucune approximation souhaitée
				actor->AddAsPhysXActor(physicscene);
			else
				ActorApproxReader(actor,approx);
		


			//actor->setGroup(group);	
			idXML2idGL id;
			id.idgl = ptabActor.size();
			id.idxml = idact;
			idActor.push_back(id);

			ptabActor.push_back(actor);
			ptabActoritself.push_back(1);

		}
		else if(value.compare("kinematic")==0)
		{
			int iditem = 0, group = defgroup, idact;
			float mass=defmass, celastic=defcelastic, sfriction=defsfriction ,dfriction=defdfriction; 
			vec3 pos = defpos; quat quaternion = defquat;
			string smooth ="B-10"; string approx = defapprox;
			float velocity = 1.0;name.clear();name.clear();
			
			int idtraject = 0; int mode = 0;

			pElem->QueryIntAttribute("iditem",&iditem);

			RxItem3D *item = findItem3D(iditem);
			if(item == NULL ) continue;
			
			pElem->QueryFloatAttribute("mass",&mass);
			pElem->QueryFloatAttribute("celastic",&celastic);
			pElem->QueryFloatAttribute("sfriction",&sfriction);
			pElem->QueryFloatAttribute("dfriction",&dfriction);
			pElem->QueryIntAttribute("group",&group);
			pElem->QueryIntAttribute("id",&idact);
			pElem->QueryFloatAttribute("velocity",&velocity);

			if(ptr = pElem->Attribute("name"))
				name = ptr;

			if(ptr = pElem->Attribute("position"))
				pos = str2vec3(ptr);

			if(ptr = pElem->Attribute("quat"))
				quaternion = str2quat(ptr);
			if(ptr = pElem->Attribute("approx"))
				approx = ptr;
			
			pElem->QueryIntAttribute("trajectory",&idtraject);

			pElem->QueryIntAttribute("mode",&mode);

			if(ptr = pElem->Attribute("smooth"))
				smooth = ptr;
			
			
				

			RxKinematicActor * actor = new RxKinematicActor; if(!actor) continue;
			actor->CreateActor(item);
			if(approx.empty()) //aucune approximation souhaitée
				actor->AddAsPhysXActor(physicscene);
			else
				ActorApproxReader(actor,approx);
			actor->setMass(mass);
			actor->setInitGlobalPosition(pos);
			actor->setInitGlobalOrient(quaternion.to_matrix());
			actor->setMaterial(celastic,sfriction,dfriction);
			//actor->setGroup(group);
			actor->setVelocity(velocity);
			
			if (name.size())
				actor->setName(name);

			if(idtraject)
			{
				actor->setOffsetPosition(pos);
				actor->CreateTrajectory(tabDataVec3[xml2gl(idtraject,iddatatab)],mode);
				unsigned int nb_seg,s_mode;
				getTrajectSmoothParameter(smooth.data(),&nb_seg,&s_mode);
				actor->SmoothTrajectory(nb_seg, s_mode); 	
			}

			idXML2idGL id;
			id.idgl = ptabActor.size();
			id.idxml = idact;
			idActor.push_back(id);
			actor->setPosition(pos);
			ptabActor.push_back(actor);
			ptabActoritself.push_back(1);
		}
		else if(value.compare("character")==0)
		{
			int iditem = 0, group = defgroup, idact;
			float mass=defmass, celastic=defcelastic, sfriction=defsfriction ,dfriction=defdfriction; 
			float velocity=0.01f;
			vec3 pos = defpos; quat quaternion = defquat; string approx = defapprox;
			name.clear();

			pElem->QueryIntAttribute("iditem",&iditem);

			RxItem3D *item = findItem3D(iditem);
			if(item == NULL ) continue;
			
			pElem->QueryFloatAttribute("mass",&mass);
			pElem->QueryFloatAttribute("celastic",&celastic);
			pElem->QueryFloatAttribute("sfriction",&sfriction);
			pElem->QueryFloatAttribute("dfriction",&dfriction);
			pElem->QueryIntAttribute("group",&group);
			pElem->QueryIntAttribute("id",&idact);
			pElem->QueryFloatAttribute("velocity",&velocity);

			if(ptr = pElem->Attribute("name"))
				name = ptr;

			if(ptr = pElem->Attribute("position"))
				pos = str2vec3(ptr);
			if(ptr = pElem->Attribute("quat"))
				quaternion = str2quat(ptr);
			if(ptr = pElem->Attribute("approx"))
				approx = ptr;

			RxCharacterActor * actor = new RxCharacterActor; if(!actor) continue;
			actor->CreateActor(item);
			if(approx.empty()) //aucune approximation souhaitée
				actor->AddAsPhysXActor(physicscene);
			else
				ActorApproxReader(actor,approx);
			if(name.size())
				actor->setName(name);
			actor->setMass(mass);
			actor->setInitGlobalPosition(pos);
			actor->setInitGlobalOrient(quaternion.to_matrix());
			actor->setMaterial(celastic,sfriction,dfriction);
			actor->setGroup(group);
			actor->velocity = velocity;
		
			idXML2idGL id;
			id.idgl = ptabActor.size();
			id.idxml = idact;
			idActor.push_back(id);

			ptabActor.push_back(actor);
			ptabActoritself.push_back(1);
		}

	} //fin for
}

void RxScene::SceneComponentReader(TiXmlElement* pElem)
{
	const char* ptr;
	tabLight.clear();
	for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour actor
	{
		string value = pElem->Value();
		//////////////////////////////////////////////////////////////////
		/////////gllight ////////////////////////////////////////////////
		if(value.compare("gllight") == 0)
		{
			RxLight light; idXML2idGL id;
			pElem->QueryIntAttribute("id",&id.idxml);
			id.idgl = tabLight.size();

			TiXmlElement * pElem2 = pElem->FirstChildElement();
			for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) //pour actor
			{
				string value2 = pElem2->Value();
				if(value2.compare("ambient")==0)
				{
					if(ptr = pElem2->Attribute("color"))
						light.ambient = str2vec4(ptr);
				}
				else if(value2.compare("diffuse")==0)
				{
					if(ptr = pElem2->Attribute("color"))
						light.diffuse = str2vec4(ptr);
				}
				else if(value2.compare("specular")==0)
				{
					if(ptr = pElem2->Attribute("color"))
						light.specular = str2vec4(ptr);
				}
				else if(value2.compare("position")==0)
				{
					if(ptr = pElem2->Attribute("position"))
						light.pos = str2vec4(ptr);
				}
				else if(value2.compare("attenuation")==0)
				{
					pElem2->QueryFloatAttribute("constant",&light.attenuation.x);
					pElem2->QueryFloatAttribute("linear",&light.attenuation.y);
					pElem2->QueryFloatAttribute("quadratic",&light.attenuation.z);
				}
			}
			idLight.push_back(id);
			tabLight.push_back(light);
			
		}
		//////////////////////////////////////////////////////////////////
		/////////skybox ////////////////////////////////////////////////
		else if(value.compare("skybox") == 0)
		{
			
			TiXmlElement * pElem2 = pElem->FirstChildElement();
			RxIDTexture f,b,l,r,t,bo;
			for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) //pour actor
			{
				string value2 = pElem2->Value();
				
				if(value2.compare("front")==0)
				{
					if(ptr = pElem2->Attribute("filename"))
						f = RxTexManager::Instance().LoadTex(ptr);
						
				}
				else if(value2.compare("back")==0)
				{
					if(ptr = pElem2->Attribute("filename"))
						b = RxTexManager::Instance().LoadTex(ptr);
				}
				else if(value2.compare("left")==0)
				{
					if(ptr = pElem2->Attribute("filename"))
						l = RxTexManager::Instance().LoadTex(ptr);
				}
				else if(value2.compare("right")==0)
				{
					if(ptr = pElem2->Attribute("filename"))
						r = RxTexManager::Instance().LoadTex(ptr);
				}
				else if(value2.compare("top")==0)
				{
					if(ptr = pElem2->Attribute("filename"))
						t = RxTexManager::Instance().LoadTex(ptr);
				}
				else if(value2.compare("bottom")==0)
				{
					if(ptr = pElem2->Attribute("filename"))
						bo = RxTexManager::Instance().LoadTex(ptr);
				}
			}
			if(skyBox == NULL)
			{
				skyBoxitself = 1;
				skyBox = new RxSkyBox(t,bo,f,b,l,r);
			}
			else
				skyBox->setSkyBoxTexture(t,bo,f,b,l,r);
		}
		//////////////////////////////////////////////////////////////////
		/////////shadow ////////////////////////////////////////////////
		else if(value.compare("shadow") == 0)
		{

			vector<unsigned int> lactors;
			vector<unsigned int> llights;
	
			pElem->QueryIntAttribute("enable",&flagShadowMode);
			if(flagShadowMode == 0) continue; //pas de shadow
			else if(flagShadowMode == 1) //shadowvolume
			{
					if(ptr = pElem->Attribute("idactors"))
					{
						lactors = str2vui(ptr);
						for(unsigned int i = 0; i < lactors.size(); i++)
						{
							unsigned int n = xml2gl(lactors[i],idActor);
							n;
							RxItem3D * item = ptabActor[ n ]->getItem3D();
							shadowVolume.AddItemRef( item );

						}
					}
					else //par defaut on ajoute tous les acteurs de la scene
					{
						for(unsigned int i = 0; i < ptabActor.size(); i++)
							shadowVolume.AddItemRef( ptabActor[ i ]->getItem3D() );
					}
					
					if(ptr = pElem->Attribute("idlights"))
					{
						llights = str2vui(ptr);
						for(unsigned int i = 0; i < llights.size(); i++)
						{
							unsigned int id = xml2gl(llights[i],idLight);
							shadowVolume.AddLight( tabLight[ id ] );
						}
					}
					else //par defaut on ajoute tous le lumière de la scene
					{
						for(unsigned int i = 0; i < tabLight.size(); i++)
							shadowVolume.AddLight( tabLight[ i ] );
					}

					if(ptr = pElem->Attribute("color"))
					{
						vec4 color = str2vec4(ptr);
						shadowVolume.setShadowColor(color);
					}
					
					//si pas de lumière on en met une par défaut
					if(llights.empty())
						shadowVolume.AddLight	(vec4(  1, -1, 1,1));

					shadowVolume.Init();
					shadowVolume.setCameraRef(this->smartCam);

					//initialise au moins une fois tous les acteurs
					unsigned int kLight, kActor;
					for(kLight = 0; kLight < tabLight.size(); kLight++)
					{
						shadowVolume.UpdatedLight(kLight, tabLight[kLight].pos, tabLight[kLight].dir);
					}
					unsigned int kSActor = 0;
					for(kActor = 0; kActor < ptabActor.size(); kActor++)
						//verifie que c'est bien acteur que l'on a rajouté
						if(shadowVolume.IsValideItem(ptabActor[kActor]->getItem3D() ))
						{
									shadowVolume.UpdatedObject(kSActor,ptabActor[kActor]->getGLmat());
							kSActor++;
						}

					

			}
			else if(flagShadowMode == 2) //shadow map
			{
					int mappingsize = 512;
					pElem->QueryIntAttribute("mappingsize",&mappingsize);
					if(mappingsize < 0) mappingsize = 128;
					if(mappingsize > 2048) mappingsize = 2048;
					shadowMapping.Init(mappingsize);


					if(ptr = pElem->Attribute("idlights"))
					{
						llights = str2vui(ptr);
						for(unsigned int i = 0; i < llights.size(); i++)
						{
							unsigned int id = xml2gl(llights[i],idLight);
							shadowMapping.setLight( &tabLight[ id ] );
						}
					}


					if(ptr = pElem->Attribute("color"))
					{
						vec4 color = str2vec4(ptr);
						shadowMapping.setShadowColor(color);
					}
			

			}
		}
		//////////////////////////////////////////////////////////////////
		/////////camera ////////////////////////////////////////////////
		else if(value.compare("camera") == 0)
		{
		}
	}//fin for
}





void RxScene::ActorApproxReader(RxActor * actor, string &approx)
{
	
	unsigned int found;

	found=approx.find("cube");
	if(found!=string::npos)
	{
		found = approx.find_first_of(':');
		if(found!=string::npos)
		{

			vec3 size = str2vec3(approx.substr(found+1));//on relève les dimensions
			actor->AddAsPhysXApproxCube(physicscene,size);
			return;
		}
	}

	found=approx.find("sphere");
	if(found!=string::npos)
	{
		found = approx.find_first_of(':');
		if(found!=string::npos)
		{
			float radius = 1.0f;
			sscanf(approx.substr(found+1).c_str(),"%f",&radius); //on relève le rayon
			actor->AddAsPhysXApproxSphere(physicscene,radius);
			return;
		}
	}


	found=approx.find("cylinder");
	if(found!=string::npos)
	{
		found = approx.find_first_of(':');
		if(found!=string::npos)
		{
			vec4 data = str2vec4(approx.substr(found+1));//on relève les paramétres
			vec3 size = data;
			actor->AddAsPhysXApproxCylinder(physicscene,data.w,size);
			return;
		}
	}

	found=approx.find("capsule");
	if(found!=string::npos)
	{
		found = approx.find_first_of(':');
		if(found!=string::npos)
		{
			vec2 data = str2vec2(approx.substr(found+1));//on relève les paramétres
			actor->AddAsPhysXApproxCapsule(physicscene,data.x,data.y);
			return;
		}
	}

	found=approx.find("convex");
	if(found!=string::npos)
	{
				found = approx.find_first_of(':');
		if(found!=string::npos)
		{

			return;
		}
	}


}

RxActor *RxScene::FindActorByName(const char *nameActor)
{
	if(!nameActor) return NULL;
	string name = nameActor;

	unsigned int i;
	for(i = 0; i < ptabActor.size(); i++)
	{
		if(ptabActor[i]->getName() == name)
			return ptabActor[i];
	}

	return NULL;
}

void RxScene::AddActor(RxActor *_actor)
{
	ptabActor.push_back(_actor);
	ptabActoritself.push_back(0);
	if(flagShadowMode == 1) //shadowvolume
	{

		RxItem3D * item = _actor->getItem3D();
		shadowVolume.AddItemRef( item );
		shadowVolume.Init();
	}
}