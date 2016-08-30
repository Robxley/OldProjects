
#include "tests.h"



int MyEngine::InitGraphicObjects()
{
	#ifdef _DEBUG
	RXCHECKGLERROR("[MyEngine::InitGraphicObjects] start");
	#endif


	debugSkyBox.setSkyBoxTexture(
		RxTexManager::Instance().LoadTexture("data/skybox/1/YP.jpg"),
		RxTexManager::Instance().LoadTexture("data/skybox/1/YN.jpg"),
		RxTexManager::Instance().LoadTexture("data/skybox/1/ZP.jpg"),
		RxTexManager::Instance().LoadTexture("data/skybox/1/ZN.jpg"),
		RxTexManager::Instance().LoadTexture("data/skybox/1/XP.jpg"),
		RxTexManager::Instance().LoadTexture("data/skybox/1/XN.jpg")
	);


	debugSkyBox.SetSize(100.0f,100.0f,100.0f);
	debugSkyBox.Init();

    // Vertices et coodonnées
	simpleShader.Load("Shaders/texture.vert","Shaders/texture.frag");
	//animMeshShader.Load("Shaders/animmesh.vert","Shaders/animmesh.frag");
	//simpleShader.Load("Shaders/couleurs.vert","Shaders/couleurs.frag");

	
	simpleCubeMesh.setShader(&simpleShader);
	simpleCubeMesh.setTexture(RxTexManager::Instance().LoadTexture("data/texture.png"));
	

	debugSphereCulling.setProjection(&defaultPerspective.getProjection());
	debugSphereCulling.setModelView(&defaultCamera.LookAt());
	debugSphereCulling.setShader(&simpleShader);
	debugSphereCulling.setTransform(glm::translate(glm::mat4(),glm::vec3(20,10,20)));
	debugSphereCulling.setTexture(RxTexManager::Instance().LoadTexture("data/terre.jpg"));

	defaultFont.GenerateGlTexture("defaut.ttf",14,2);
	defaultFont.setTextColor(glm::vec4(1.0f,1.0f,0.0f,1.0f));

	debugItem2D.setFont(&defaultFont);
	debugItem2D.setPosition(200,400);
	debugItem2D.setSize(200,100);
	debugItem2D.setText("Test Frustrum Culling : Sphere draw");

	debugButton.setFont(&defaultFont);
	debugButton.setPosition(200,200);
	debugButton.setSize(200,100);
	debugButton.setTexts("out","on","click");
	debugButton.setColors(glm::vec4(1.0,1.0,1.0,1.0),glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0));
	

	RxTexManager::Instance().LoadTexture("data/texture3.jpg");
	debugFBO.setViewport(200,100);	
	debugFBO.Init();

	RxTexture tex = RxProceduralTexture::createGLTexture("proc_texture",1024,1024);
	debugButton.setIDsTex( tex.glID ,debugFBO.getColorMap(), RxTexManager::Instance().LoadTexture("data/Crackles0004_S.jpg"));
		
	#ifdef _DEBUG
	RXCHECKGLERROR("[MyEngine::InitGraphicObjects] start2");
	#endif

	//test heightmap
	std::vector<std::string> arrayfilename;
	arrayfilename.push_back("data/cailloux.jpg");
	arrayfilename.push_back("data/herbe.jpg");
	arrayfilename.push_back("data/sand.jpg");
	arrayfilename.push_back("data/sol.jpg");


	RxTexture ArrayTexture = RxTexManager::Instance().CreateArrayTexture(arrayfilename,"arrayTextureHeightMap");
	debugHeightMap.setTexture(ArrayTexture);
	debugHeightMap.EnableRandomTexturing();
	
	//debugHeightMap.Load("data/HeightMap.jpg",glm::vec3(0,-10,0),glm::vec3(100.0f,0.2f,100.0f));

	Rx2DPerlinNoise pNoise;

	int tS = 256;
	debugHeightMap.CreatePerlinMap(glm::vec3(-tS*0.5f ,-20,-tS*0.5f ),glm::vec3(1.0f,1.0f,1.0f),tS,tS,-20,20,pNoise);
	debugHeightMap.setMatrix(&defaultPerspective.getProjection(),&defaultCamera.LookAt());
	#ifdef _DEBUG
	RXCHECKGLERROR("[MyEngine::InitGraphicObjects] start3");
	#endif



	//RxAssimpLoader assimpLoader("data/models/static/Church/3ds file.3DS");
	//RxAssimpLoader assimpLoader("\data\models\collada\Creatures\momo2animed\md5");
	//RxAssimpLoader assimpLoader("data/models/animation/dwarf/dwarf2.b3d");
	//RxAssimpLoader assimpLoader("data/models/static/Church/3ds file.3DS");
	//RxAssimpLoader assimpLoader("data/models/animation/dwarf/dwarf2.b3d");
	RxAssimpLoader assimpLoader("data/models/animation/zombie/zombie.b3d");

	assimpLoader.ExportTo(&AssimpMesh);

	AssimpMesh.NormalizeData();
	AssimpMesh.CenterDataToOrigin(glm::vec3(0,1,0));
	AssimpMesh.ComputeMesh();




	animMeshShader.Load("Shaders/animmesh.vert","Shaders/animmesh.frag");
	//RxAssimpLoader assimpLoader2("data/models/animation/dwarf/dwarf2.b3d");
	RxAssimpLoader assimpLoader2("data/models/animation/zombie/zombie.b3d");
	//RxAssimpLoader assimpLoader2("data/models/collada/Creatures/momo2/momo2.dae");
	//RxAssimpLoader assimpLoader2("data/models/collada/Creatures/momo2animed/md5/momo3.md5mesh");
	assimpLoader2.ExportTo(&AnimMesh);
	AnimMesh.getSkeleton().BluidBones();
	AnimMesh.ComputeMesh();
	AnimMesh.setShader(&animMeshShader);
	tick_counter = 0;




	#ifdef _DEBUG
	RXCHECKGLERROR("[MyEngine::InitGraphicObjects] ");
	#endif

	return 0;
}

void MyEngine::Display3D()
{
	

	defaultCamera.Update();
	debugSkyBox.Draw(defaultPerspective.getProjection(), defaultCamera.LookAt());


	// Nettoyage de l'écran
	static float angles = 0;
	angles +=0.1f;
	// On affiche le triangle
	simpleShader.Enable();
	simpleShader.Send1i("textureSampler", 0);
	//simpleShader.SendMat4f("modelview",GL_FALSE,defaultCamera.LookAt());
	//simpleShader.SendMat4f("projection",GL_FALSE,defaultPerspective);
	simpleShader.SendProjection(defaultPerspective);
	simpleShader.SendModelView(defaultCamera.LookAt());
	//simpleShader.SendMat4f("transform",GL_FALSE,glm::mat4());
	simpleShader.SendTransform(glm::mat4());	
	Rx_Repere3D();


	//simpleCubeMesh.setTransform(glm::translate(glm::mat4(),));
	//simpleShader.SendMat4f("transform",GL_FALSE,glm::translate(glm::mat4(),glm::vec3(1.0,1.0,0.0)));


	AssimpMesh.Draw();

	//AnimMesh.getSkeleton().Draw(&simpleShader);

	//simpleShader.SendTransform(glm::rotate(glm::mat4(),angles,glm::vec3(1.0,1.0,0.0)));
	simpleCubeMesh.Draw();

	//skeleton.Draw(&simpleShader);


	AnimMesh.getSkeleton().Draw(&simpleShader);
	//tick_counter++;
	//float dt = 24.0f*(tick_counter)/60.0f;
	//AnimMesh.PlayAnimationPerTick(0, dt);
	AnimMesh.PlayAnimation(0,(float)SDL_GetTicks()/5000.0f);
	AnimMesh.DrawAnimSkeleton(&simpleShader);
	simpleShader.Disable();

	animMeshShader.Enable();
	animMeshShader.SendProjection(defaultPerspective);
	animMeshShader.SendModelView(defaultCamera.LookAt());
//	animMeshShader.SendTransform(glm::mat4());	
//	animMeshShader.SendMat4f("bonesMatrix[]",0,AnimMesh.tBonesGlobalTransforms);

	AnimMesh.Draw();
	animMeshShader.Disable();

	#ifdef _DEBUG
	RXCHECKGLERROR("[MyEngine::Display3D] animMeshShader");
	#endif


	debugHeightMap.Draw(defaultCamera.pos, defaultCamera.dir);
	//debugHeightMap.Draw();


	//test culling
	//defautFrustrumCulling.UpdateFrustrum(defaultCamera, defaultPerspective);


	
	
	defaultFrustrumCulling.Update(defaultPerspective.getProjection(), defaultCamera.LookAt(),debugSphereCulling.getTransform());
	
	
	glBindTexture(GL_TEXTURE_2D, RxTexManager::Instance().getTexture("proc_texture"));
	simpleShader.Enable();
	simpleShader.Send1i("textureSampler", 0);
	simpleShader.SendMat4f("modelview",GL_FALSE,defaultCamera.LookAt());
	simpleShader.SendMat4f("projection",GL_FALSE,defaultPerspective);
	simpleShader.SendMat4f("transform",GL_FALSE,debugSphereCulling.getTransform());
	

	

	debugSphereCulling.Draw();
	simpleShader.Disable();


	int test_frustrum = defaultFrustrumCulling.SphereInFrustum(glm::vec3(0.0f),0.5);
	if(test_frustrum == RX_INSIDE_FRUSTRUM)
	{
		debugItem2D.setText("Test Frustrum Culling : Sphere draw");
	}
	else if(test_frustrum == RX_OUTSIDE_FRUSTRUM)
	{
		debugItem2D.setText("Test Frustrum Culling : Sphere no draw");
	}
	else
	{
		debugItem2D.setText("Test Frustrum Culling : Sphere draw intersect");
	}

	#ifdef _DEBUG
	RXCHECKGLERROR("[RxEngine::DebugInit] ");
	#endif


	//test du FBO
	debugFBO.Enable();
	glClearColor(0.5f,0.5f,1.0f,1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	glm::mat4 rotation = defaultCamera.LookAt() *glm::rotate(glm::mat4(),angles*4,glm::vec3(1.0,1.0,0.0));

	simpleShader.Enable();
	simpleShader.Send1i("textureSampler", 0);
	simpleShader.SendMat4f("modelview",GL_FALSE,rotation);
	simpleShader.SendMat4f("projection",GL_FALSE,defaultPerspective);

	glBindTexture(GL_TEXTURE_2D, RxTexManager::Instance().LoadTexture("data/texture3.jpg"));
	simpleCubeMesh.Draw();


	simpleShader.Disable();




	debugFBO.Disable();


}
void MyEngine::Display2D()
{
	//glEnable(GL_BLEND);
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	defaultFont.PrintText(100,100, "test");
	int w,h;
	pSDLOpenglContext->getWindowSize(&w,&h);
	defaultFont.setTextColor(glm::vec4(1.0f));
	defaultFont.PrintText(20 ,h-20, "fps : %d \ntps entre 2 frames  : %d",(int)defaultFps.getFPS(),(int)defaultFps.getElapseTime());
	
	debugItem2D.Draw();
	debugButton.Draw();
	//glDisable(GL_BLEND);
	
}
void MyEngine::Frame()
{
		defaultFps.Compute();
		debugButton.CallBackCoord();
}
