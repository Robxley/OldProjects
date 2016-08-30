//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXAPP_H_
#define _RXAPP_H_



#define NOMINMAX


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <SDL2/SDL.h>
#include "RxFont.h"
#include "RxGUI.h"
#include "RxTexManager.h"
#include "RxWindow.h"
#include "RxHeightMap.h"
#include "RxProceduralTexture.h"
#include "RxLogger.h"
#include "RxCamera.h"
#include "RxShader.h"
#include "RxVBO.h"
#include "RxTools.h"
#include "RxMesh.h"
#include "RxAssimpLoader.h"
#include "RxSimpleMesh.h"
#include "RxTreeStruct.h"
#include "RxFPS.h"
#include "RxFBO.h"
#include "RxSkyBox.h"
#include "RxCulling.h"
#include "RxMath3D.h"
#include "RxAnimMesh.h"


#define RX_ENGINE_EXIT 0   
#define RX_ENGINE_NEXTSTAP 1   


//default values
#define RX_WINDOW_NAME "RxEngine Default Window"
#define RX_FULLSCREEN 0
#define RX_DOUBLE_BUFFER 1
#define RX_BUFFER_SYNCHRONIZE 1
#define RX_MULTISAMPLES 4
#define RX_DEPTH_SIZE 24
#define RX_STENCILE_SIZE 8   //if 0, stencil not use
#define RX_WINDOW_HEIGHT 768 	 
#define RX_WINDOW_WIDTH 1024
#define RX_WINDOW_COORD_X -1 // RX_WINDOW_COORD_X < 0 --> centred windows	 
#define RX_WINDOW_COORD_Y -1 // RX_WINDOW_COORD_Y < 0 --> centred windows	 

#define RX_GL_CONTEXT_MAJOR_VERSION 3
#define RX_GL_CONTEXT_MINOR_VERSION 3

//default opengl
#define RX_CLEAR_COLOR 0.5f, 0.5f, 0.5f, 1.0f  //glClearColor(RX_CLEAR_COLOR);	
#define RX_CLEAR_DEPTH 1.0f  //	glClearDepth(RX_CLEAR_DEPTH);	



class RxSDLOpenglContext
{

	public:
	


		bool flagFullScreen;			//Default : false
		bool flagDoubleBuffer;			//Default : true
		bool flagBufferSynchronize;		//Default : true
		int flagMultiSamples;			//Default : 4
		int flagDephtSize;				//Default : 24
		int flagStencilSize;			//Default : 8
		

		RxSDLOpenglContext();
		RxSDLOpenglContext(const char *name, int w, int h, int x = RX_WINDOW_COORD_X, int y = RX_WINDOW_COORD_Y);
		~RxSDLOpenglContext();	


		int Init();
		virtual void InitGlOptions();

		inline void SwapWindow();

		virtual void setWindowSize(int w, int h);
		virtual void setWindowPosition(int x, int y);
		virtual void setWindowName(const char * name);
		virtual int setWindowFullScreen(bool f);

		virtual void getWindowPosition(int *x, int *y);
		virtual void getWindowSize(int *w, int *h);



		SDL_Window* getSDLWindow();




	private:
		 
		//création de la fenetre et contexte opengl core 3.3
		int InitSDLGL();


		int winWidth, winHeight;		//Default : 1024x768
		int winCoordX, winCoordY;		//Middle Position

		std::string winName;					//Default : "RxEngine Default Window"

		//initialise les routines SDL et OPENGL (initialisation, varible par défaut ...)
		
		SDL_Window *pSDLWindow;
        SDL_GLContext GLContext;

};



/*************************************************************************************************************************/
/*************************************************************************************************************************/
//RxEngine
/*************************************************************************************************************************/
/*************************************************************************************************************************/


class RxEngine
{
/*************************************************************************************************************************/
/*************************************************************************************************************************/
public:
	RxPerspective defaultPerspective;
	RxCamera defaultCamera;
	RxMaterial defaultMaterial;
	RxShader defaultShader;
	RxLight defaultLight;
	RxFrustrumCulling defaultFrustrumCulling;


public:
    RxEngine();  //corps de l'application
    ~RxEngine();

	int Init(RxSDLOpenglContext *_pOpenglContext);    //initialise Engine avec le context opengl. return InitGraphicObjects.
	virtual int InitGraphicObjects(); //Fonction appelée par Init. 

    virtual void Loop();						//boucle
	virtual void DisplayFrames();  //dessine l'ensemble de la scène (objet 2d & 3d & Picking si besoin)

	//frames
    virtual void Display3D();			//dessine les objets 3d
    virtual void Display2D();			//dessine les objets 2d
	virtual void Frame();				//
	virtual void DisplayPicking();		//picking

	virtual int KeyboardEvent(SDL_Keycode key);  //appelé par Events()

protected:

	void ReSize();				//procédure de redimentionnement de la fenetre
    int Events();  //gère les eventements
	RxSDLOpenglContext *pSDLOpenglContext;
     
};
#endif

