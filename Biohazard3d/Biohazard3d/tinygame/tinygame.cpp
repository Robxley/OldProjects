

#include "tinygame.hpp"



TinyEngine::TinyEngine()
{
	BH3D_TextureManager();
}
TinyEngine::~TinyEngine()
{
	BH3D_TextureManager().Clear();
}

void TinyEngine::ResizeCallBack(unsigned int w, unsigned h)
{
	//projection = glm::perspectiveFov(70.0f, (float)w, (float)h, 0.1f, 100.0f);
	projection = glm::perspective(70.0f, (float)w / h, 0.1f, 100.0f);
}

int TinyEngine::InitObjects()
{
	//param opengl
	glClearColor(0.4f, 0.3f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	projection = glm::perspective(70.0f, (float)pGLContext->GetWinWidth() / pGLContext->GetWinHeight(), 0.1f, 100.0f);
	lookAt = glm::lookAt<double, glm::highp>(glm::vec3(0.0f, topPosition, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//chargement des ressources



	//chargement du shader de base
#ifndef BH3D_HAVE_OPENGLES
	tinyShader.Load("data/shaders/default.vert", "data/shaders/default.frag");
#else
	tinyShader.Load("data/shaders/default_es.vert", "data/shaders/default_es.frag");
#endif

	//paramétrage du font

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BH3D::Texture * fontTexture = BH3D_LoadTexture("data/textures/font.png");
	if (fontTexture)
		tinyFont.UseTexture(fontTexture->GetGLTexture(), 16);
	else
		tinyFont.UseTexture(0, 16);

	if (!tinyFont.IsValid())
		BH3D_LOGGERERROR("Invalid Font");

	tinyFont.AddStaticText(nameGame);
	lengthNameGame = tinyFont.LengthText(nameGame);
	
	tinyFont.ComputeStaticText();


	//chargement vaisseau
	BH3D::AssimpLoader myAssimpLoader("data/space_frigate_6/space_frigate_6.3DS");

	myAssimpLoader.ExportTo(&tinyShipMesh);
	tinyShipMesh.SetTexture("data/space_frigate_6/space_frigate_6_color.png");
	
	tinyShipMesh.NormalizeData();
	//tinyShipMesh.CenterDataToOrigin(glm::vec3(0, 0, 0));
	tinyShipMesh.ScaleMesh(4.0f);
	tinyShipMesh.ComputeMesh();
	tinyShipMesh.FreeArrays();


	//game init
	mousex = pGLContext->GetWinWidth()/2;
	mousey = pGLContext->GetWinHeight()/2;


	BH3D_CHECKGLERROR();
	BH3D_LOGGER("End Init Objects");
	return BH3D_OK;
}


void TinyEngine::Loop(bool loopTest)
{
	while (Events() && loopTest)
	{
		TinyGame();
		Display();
		pGLContext->SwapWindow();

	}
}

void TinyEngine::Display3D()
{
	tinyShader.Enable();
	tinyShader.SendProjection(projection);
	tinyShader.SendModelView(lookAt);
	tinyShader.SendTransform(shipTransform);

	tinyShipMesh.Draw();

	BH3D_CHECKGLERROR();
}
void TinyEngine::Display2D()
{

	unsigned int h = pGLContext->GetWinHeight();
	unsigned int w = (pGLContext->GetWinWidth() - lengthNameGame)/2;

	tinyFont.GetDynamicTextStream() << "Fps : " << tinyFps.GetFps();
	tinyFont.PrintTextStream(5, h-20);
	tinyFont.GetDynamicTextStream().str("");
	tinyFont.GetDynamicTextStream() << "mx : " << shipTranslate.x << " \nmy : " << shipTranslate.z;
	tinyFont.PrintTextStream(5, h - 40);
	tinyFont.GetDynamicTextStream().str("");

	tinyFont.PrintStaticTexts(w, h - 20, 0);

	BH3D_CHECKGLERROR();

}

int TinyEngine::Events()
{
	SDL_Event event;
	void SDL_PumpEvents(void);


	//gestion des autres events

	int returnValue = BH3D_OK;

	while (SDL_PollEvent(&event))  //get the next event
	{

		switch (event.type)
		{
			//it's the end, so sad
		case SDL_QUIT:
			return BH3D_EXIT;
			break;

			//window event
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				case SDL_WINDOWEVENT_CLOSE: //it's the end, so sad
					return BH3D_EXIT;
					break;
				case SDL_WINDOWEVENT_RESIZED: //window resize
					pGLContext->ReSize();
					break;
				case SDL_WINDOWEVENT_RESTORED:
					pGLContext->ReSize();
					break;
			}
			break;
			
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					return BH3D_EXIT;
					break;
			}
			break;


		/*case SDL_FINGERMOTION:
			mousex = event.tfinger.x*pGLContext->getWinWidth();
			mousey = event.tfinger.y*pGLContext->getWinHeight();			
			break;
		case SDL_FINGERDOWN:
			mousex = event.tfinger.x*pGLContext->getWinWidth();
			mousey = event.tfinger.y*pGLContext->getWinHeight();
			BH3D_LOGGER("finger id : " << event.tfinger.fingerId);
			BH3D_LOGGER("touch id : " << event.tfinger.touchId);
			if (event.tfinger.fingerId >= 2) return BH3D_EXIT;
			break;
		case SDL_FINGERUP:
			mousex = pGLContext->getWinWidth()*0.5;
			mousey = pGLContext->getWinHeight()*0.5;*/

		} // switch end
	} // while end




	SDL_GetMouseState(&mousex, &mousey);
	/*if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		topPosition += 0.01f*tinyFps.getElapseTime();
	}

	if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		topPosition -= 0.01f*tinyFps.getElapseTime();
	}*/

	shipTranslate.x -= (pGLContext->GetWinWidth() - mousex * 2)*0.0001f;
	shipTranslate.z -= (pGLContext->GetWinHeight() - mousey * 2)*0.0001f;


	return returnValue;

}


void TinyEngine::TinyGame()
{
	tinyFps.Compute();

	rotationShip += 0.002f*tinyFps.GetElapseTime();
	shipTransform = glm::rotate(glm::mat4(), rotationShip, glm::vec3(0, 1.0, 0));

    float d = 1 / (1 + topPosition*0.01f);
	d = d *d;
	lookAt = glm::lookAt<double, glm::highp>(glm::vec3(0.0f, topPosition, 5.0f* d), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mousey = 0;
	mousex = 0;

	shipTransform = glm::translate(glm::mat4(), shipTranslate);

}



void TinyEngine::Display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Display3D();
	Display2D();
}


int main(int argc, char **argv)
{


	TinyContext * pGLContext = new TinyContext();

		if (!pGLContext->IsCreated())
		{
			delete pGLContext;
			return 0;
		}


		TinyEngine  myEngine;


		myEngine.Init(pGLContext);
		myEngine.Loop();

			delete pGLContext;



    return 0;
}
