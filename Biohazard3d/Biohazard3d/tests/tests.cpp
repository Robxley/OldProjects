

#include "SDLGLContext.hpp"
#include "Engine.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Font.hpp"
#include "GLErrorLogger.hpp"
#include "Fps.hpp"
#include "AssimpLoader.hpp"

#include <iostream>
#include <memory>
#include <random>
#include "SDLTextureManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyContext : public BH3D::SDLGLContext
{
	int Events()
	{
		int r = SDLGLContext::Events();
		

		return r;
	}
};


class MyEngine : public BH3D::Engine
{


	BH3D::VBO myvbo;
	BH3D::Cube myCube{ 2.0f };
	BH3D::Sphere mySphere{1.0f};
	BH3D::Mesh myMultiMesh;
	BH3D::Mesh myComplexMesh;
	BH3D::Shader defaultShader;
	glm::mat4 lookAt, projection, transform, translate;
	BH3D::Texture *myTexture, *myTexture2, *myFontTexture, *myFontTexture2;
	BH3D::Font myFont;
	BH3D::Fps myFps;
	BH3D::Mesh myMesh;
	BH3D::AnimMesh myAnimMesh;
	BH3D::Shader myAnimShader;



	unsigned int lengthNameGame;
	float rotation_angle = 0;

	public:

	~MyEngine()
	{
		BH3D_TextureManager().Clear();
	}


		void ResizeCallBack(unsigned int w, unsigned h)
		{
			projection = glm::perspectiveFov(70.0f, (float)w, (float)h, 0.1f, 50.0f);
			if (w > h)
				lookAt = glm::lookAt(glm::vec3(0, 0.0f, 10), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			else
				lookAt = glm::lookAt(glm::vec3(0, 0.0f, 25), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		int InitObjects()
		{
		
#ifndef NDEBUG
			BH3D_LOGGER("We are in DEBUG MODE !!!!");
			glClearColor(0.4f, 0.3f, 1.0f, 0.0f);
#else
			BH3D_LOGGER("We are in RELEASE MODE !!!!");
			glClearColor(0.3f, 1.0f, 0.4f, 0.0f);
#endif

			//param opengl
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);

			projection = glm::perspectiveFov(70.0f, (float)pGLContext->GetWinWidth(), (float)pGLContext->GetWinHeight(), 0.1f, 50.0f);

			if(pGLContext->GetWinWidth() > pGLContext->GetWinHeight())
				lookAt = glm::lookAt(glm::vec3(0, 0.0f,10), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			else
				lookAt = glm::lookAt(glm::vec3(0,0.0f,25), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			
			translate = glm::translate(glm::mat4(), glm::vec3(-4.0f, 0.0f, 0.0f));

#ifndef BH3D_HAVE_OPENGLES
			std::vector<float> vvertex = {	-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,		//Front Face
											-0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,		// Back Face
											-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,		// Top Face	
											-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,		// Bottom Face
											 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f,		// Right face	
											-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f };	// Left Face


			std::vector<float>  tvertex = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
											1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
											1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };


			std::vector<GLuint> indices = { 0, 1, 2, 0, 2, 3,
											4, 5, 6, 4, 6, 7,
											8, 9, 10, 8, 10, 11,
											12, 13, 14, 12, 14, 15,
											16, 17, 18, 16, 18, 19,
											20, 21, 22, 20, 22, 23 };

			std::default_random_engine generator;
			generator.seed(SDL_GetTicks());
			std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

			std::vector<float> cvertex;


			for (int i = 0; i < vvertex.size(); i++)
				cvertex.push_back(distribution(generator));


			float dice_roll = distribution(generator);  // generates number in the range 1..6 


			//VBO Test
			myvbo.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, vvertex);
			myvbo.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, cvertex,3);
			myvbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tvertex,2);
			myvbo.AddElementBufferData(indices);
			myvbo.Create();


			//Test SDLTextureManager
			myTexture = BH3D_LoadTexture("data/textures/texture1.jpg");
			BH3D::Texture  * test = BH3D_LoadTexture("data/textures/texture1.jpg");
			test = BH3D_LoadTexture("data/textures/texture2.png");
			myTexture2 = BH3D_LoadTexture("data/textures/texture3.jpg");
			test = BH3D_LoadTexture("data/textures/texture4.jpg");
		    BH3D::Texture * eraseTex = BH3D_LoadTexture("data/textures/texture5.tga");

			std::string reslist = "Les textures chargée sont : ";
			BH3D_TextureManager().GetResourceList(reslist,"\n ----------  -->");
			BH3D_LOGGER(reslist);

			BH3D_TextureManager().Erase(eraseTex);
			BH3D_TextureManager().Erase("data/textures/texture2.png");
			reslist = "\n\nLes textures chargée sont : ";
			BH3D_TextureManager().GetResourceList(reslist, "\n ----------  -->");
			BH3D_LOGGER(reslist);

			//Shader Test
			defaultShader.Load("data/shaders/default.vert", "data/shaders/default.frag");

#else
			//Test SDLTextureManager
			myTexture= BH3D_LoadTexture("data/textures/texture1.jpg");
			
			//Shader Test
			defaultShader.Load("data/shaders/default_es.vert","data/shaders/default_es.frag");
#endif
			myFontTexture2 = BH3D_LoadTexture("data/textures/font.png");
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			myCube.SetTexture(myTexture);
			mySphere.SetTexture(myTexture);
			//multimesh test

			BH3D::Sphere::AddSubMesh(&myMultiMesh);
			BH3D::Sphere::AddSubMesh(&myMultiMesh);
			BH3D::Cube::AddSubMesh(&myMultiMesh);
			glm::mat4 transf = glm::translate(glm::mat4(), glm::vec3(0.0f, 2.5f, 0.0f));
			myMultiMesh.TransformMesh(transf, 0);
			transf = glm::translate(glm::mat4(), glm::vec3(0.0f, -2.5f, 0.0f));
			myMultiMesh.TransformMesh(transf, 1);

			myMultiMesh.NormalizeData();
			myMultiMesh.ScaleMesh(glm::vec3(4.0f));
			myMultiMesh.SetTexture(myTexture);
			myMultiMesh.ComputeMesh();



			//Font test
			myFont.UseTexture(myFontTexture2->GetGLTexture(),16);

			if (!myFont.IsValid())
				BH3D_LOGGERERROR("Invalid Font");


			myFont.AddStaticText("TinY Game Engine !!!");
			myFont.AddStaticText("Textures : 4");
			myFont.AddStaticText("Shaders : 3");
			myFont.AddStaticText("Meshes : 5");
	
			myFont.ComputeStaticText();
			lengthNameGame = myFont.LengthText("TinY Game Engine !!!");
			//lengthNameGame = myFont.LengthText("TinY Game Engine !!!");

			//test assimp loader
			//BH3D::AssimpLoader myAssimpLoader("data/F-14A_Tomcat/F-14A_Tomcat.obj");
			BH3D::AssimpLoader myAssimpLoader("data/space_frigate_6/space_frigate_6.3DS");

			myAssimpLoader.ExportTo(&myComplexMesh);
			myComplexMesh.SetTexture("data/space_frigate_6/space_frigate_6_color.png");
			//myCube.setTexture("data/space_frigate_6/space_frigate_6_color.png");
			myComplexMesh.NormalizeData();
			myComplexMesh.ScaleMesh(4.0f);
			myComplexMesh.ComputeMesh();
			myComplexMesh.FreeArrays();

			myAssimpLoader.LoadFile("data/models/zombie.b3d");
			
		

			myAssimpLoader.ExportTo(&myMesh);
			myMesh.NormalizeData();
			myMesh.ScaleMesh(glm::vec3(4.0f));
			myMesh.CenterDataToOrigin(glm::vec3(0, 0, 0));
			myMesh.ComputeMesh();


			myAssimpLoader.ExportTo(&myAnimMesh);
			myAnimMesh.getSkeleton().BluidBones();
			myAnimMesh.ComputeMesh();
#ifndef BH3D_HAVE_OPENGLES
			myAnimShader.Load("data/shaders/animmesh.vert", "data/shaders/animmesh.frag");
#else
			myAnimShader.Load("data/shaders/animmesh_es.vert", "data/shaders/animmesh_es.frag");

#endif

			BH3D_CHECKGLERROR();
			BH3D_LOGGER("End Init Objects");
			return BH3D_OK;
		}




		void Display()
		{
			myFps.Compute();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			rotation_angle += 0.002f*myFps.GetElapseTime();
			transform = glm::rotate(glm::mat4(), rotation_angle, glm::vec3(rotation_angle, 1.0 - rotation_angle, 1.0 + rotation_angle/2.0f));
			glm::mat4 transformMesh;

			
			defaultShader.Enable();
			defaultShader.SendProjection(projection);
			defaultShader.SendModelView(lookAt);


			//myTexture.Bind();
			//myvbo.Enable();
			//defaultShader.SendTransform(transform);
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


			const float offsetX = 5.0f;

			const float offsetY = 2.50f;

			//cube
			transformMesh = glm::translate(glm::mat4(), glm::vec3(0.0f, offsetY, 0.0f));
			defaultShader.SendTransform(transformMesh*transform);
			myCube.DrawSubMesh();

			//sphere
			transformMesh = glm::translate(glm::mat4(), glm::vec3(-offsetX, offsetY, 0.0f));
			defaultShader.SendTransform(transformMesh*transform);
			mySphere.DrawSubMesh();

			//cubes & sphere
			transformMesh = glm::translate(glm::mat4(), glm::vec3(offsetX, offsetY, 0.0f));
			defaultShader.SendTransform(transformMesh);
			myMultiMesh.Draw();

			//vaisseau
			transformMesh = glm::translate(glm::mat4(), glm::vec3(-offsetX, -offsetY, 0.0f));
			defaultShader.SendTransform(transformMesh*transform);
			myComplexMesh.Draw();

			//skelette
			transformMesh = glm::translate(glm::mat4(), glm::vec3(offsetX, -offsetY-2.0f, 0.0f))* glm::scale(glm::mat4(), glm::vec3(0.25f));
			defaultShader.SendModelView(lookAt*transformMesh);
			myAnimMesh.PlayAnimation(0, (float)0.001f*SDL_GetTicks());
			myAnimMesh.DrawAnimSkeleton(&defaultShader);
		
			//zombie
			myAnimShader.Enable();
			myAnimShader.SendProjection(projection);
			transformMesh =  glm::translate(glm::mat4(), glm::vec3(0, -offsetY-2.0f, 0.0f))* glm::scale(glm::mat4(), glm::vec3(0.25f));
			myAnimShader.SendModelView(lookAt*transformMesh);
			myAnimMesh.Draw();
			myAnimShader.Disable();		



			//display2d (texte)

			unsigned int h = this->pGLContext->GetWinHeight();
			unsigned int w = this->pGLContext->GetWinWidth();
			int d = myFont.GetLineSkip()+2;



			//myFont.PrintStaticTexts(10, heightw-200, 1);
			myFont.GetDynamicTextStream().str("");
			myFont.GetDynamicTextStream() << "Fps : " << myFps.GetFps();
			myFont.PrintTextStream(10, h-d);
			myFont.GetDynamicTextStream().str("");


			myFont.PrintStaticTexts( (w- lengthNameGame)/2, h - d, 0); //titre
			myFont.PrintStaticTexts( 10, h - d*2, 1); //nb textures
			myFont.PrintStaticTexts( 10, h - d*3, 2); //nb shaders
			myFont.PrintStaticTexts( 10, h - d*4, 3);  //nb meshes
	



			BH3D_CHECKGLERROR();
		}


};







int main(int argc, char **argv)
{


	MyContext pGLContext;// = new MyContext();

		if (!pGLContext.IsCreated())
		{
			//delete pGLContext;
			return 0;
		}
		pGLContext.SetWindowSize(1794,1080);

		MyEngine  myEngine;


		if(myEngine.Init(&pGLContext))
			myEngine.Loop();

			//delete pGLContext;

;

    return 0;
}
