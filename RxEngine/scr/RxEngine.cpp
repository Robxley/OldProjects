//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#include "RxEngine.h"




/**************************************************************************/
/*
/*    RxGraphicEngine
/*
/*
/***************************************************************************/


RxSDLOpenglContext::RxSDLOpenglContext():
winWidth(RX_WINDOW_WIDTH), winHeight(RX_WINDOW_HEIGHT),
winCoordX(RX_WINDOW_COORD_X), winCoordY(RX_WINDOW_COORD_Y),
winName(RX_WINDOW_NAME),
flagFullScreen(RX_FULLSCREEN), flagDoubleBuffer(RX_DOUBLE_BUFFER), flagMultiSamples(RX_MULTISAMPLES), flagBufferSynchronize(RX_BUFFER_SYNCHRONIZE), flagDephtSize(RX_DEPTH_SIZE),flagStencilSize(RX_STENCILE_SIZE)
{

}


RxSDLOpenglContext::RxSDLOpenglContext(const char *name, int w, int h, int x, int y):
winWidth(w), winHeight(h),
winCoordX(x), winCoordY(y),
winName(name),
flagFullScreen(RX_FULLSCREEN), flagDoubleBuffer(RX_DOUBLE_BUFFER), flagMultiSamples(RX_MULTISAMPLES), flagBufferSynchronize(RX_BUFFER_SYNCHRONIZE), flagDephtSize(RX_DEPTH_SIZE),flagStencilSize(RX_STENCILE_SIZE)
{
}


void RxSDLOpenglContext::InitGlOptions()
{
	//default			
	glClearColor(RX_CLEAR_COLOR);			
	glClearDepth(1.0f);								
	glEnable(GL_DEPTH_TEST);						
	glDepthFunc(GL_LEQUAL);							
    glEnable(GL_LINE_SMOOTH);
	glCullFace(GL_BACK); 
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	
	#ifdef _DEBUG
		RXCHECKGLERROR("[RxSDLOpenglContext][InitGlOptions]");
	#endif
}

int RxSDLOpenglContext::InitSDLGL()
{

    // SDL Initialisation 
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        RXLOGGERERROR("[RxSDLOpenglContext][InitSDLGL] SDL_Init ->: "<< SDL_GetError());
        SDL_Quit();
        return RX_ERROR;
    }
	else
		RXLOGGER("[RxSDLOpenglContext][InitSDLGL] INIT SDL : OK !");


    // Opengl and GLSL Version&core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, RX_GL_CONTEXT_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, RX_GL_CONTEXT_MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Double Buffer
	if(flagDoubleBuffer)
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//auxiliaire buffers
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, flagDephtSize);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, flagStencilSize);//flag_stencilSize8

    // CENTERED or not ?
	if(winCoordX < 1 || winCoordY < 1)
	{
		winCoordX = SDL_WINDOWPOS_CENTERED;
		winCoordY = SDL_WINDOWPOS_CENTERED;
	}

	//My window, MY WINDOW DUDE !!!!!!!
	pSDLWindow = SDL_CreateWindow(winName.c_str(), winCoordX, winCoordY, winWidth, winHeight,  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(pSDLWindow == 0)
    {
		//!*%&?#& window !
        RXLOGGERERROR("[RxSDLOpenglContext][InitSDLGL] SDL_CreateWindow -> "<< SDL_GetError());
        SDL_Quit();
        return RX_ERROR;
    }

	//we love opengl
    GLContext = SDL_GL_CreateContext(pSDLWindow);
    if(GLContext == 0)
    {
		//opengl don't like us
        RXLOGGERERROR("[RxSDLOpenglContext][InitSDLGL] SDL_GL_CreateContext -> "<< SDL_GetError());
        SDL_DestroyWindow(pSDLWindow);
		SDL_Quit();
        return RX_ERROR;
    }

    //Not so fast, you can not see everything (60fps is enough)
    if (flagBufferSynchronize)
	   SDL_GL_SetSwapInterval(1);
	

	//We need help with opengl functions and extensions
	//Why make something already done.
    #ifdef WIN32
	if (gl3wInit()) 
	{
                RXLOGGERERROR("[RxSDLOpenglContext][InitSDLGL] gl3wInit");
                return RX_ERROR;
    }
    if (!gl3wIsSupported(RX_GL_CONTEXT_MAJOR_VERSION, RX_GL_CONTEXT_MINOR_VERSION))
	{
                RXLOGGERERROR("[RxSDLOpenglContext][InitSDLGL] INIT GL3W : ERROR OPENGL 3.3 not supported");
                return RX_ERROR;
    }
    #endif


	//look what we get....
	RXLOGGER("[RxSDLOpenglContext] OpenGL Core :  "<<glGetString(GL_VERSION)<<" - (GLSL "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<')');

	
	#ifdef _DEBUG
		RXCHECKGLERROR("[RxSDLOpenglContext][InitSDLGL]"); //is something wrong ?
	#endif

    return RX_OK; //fine


}
int RxSDLOpenglContext::Init()
{

	//we need a lot of things like a window and a opengl context
	int valide = InitSDLGL();


	if(valide > 0)
	{
		InitGlOptions(); //some gl default parameters
		#ifdef _DEBUG
		RXCHECKGLERROR("[RxSDLOpenglContext][InitGlOptions]"); //is something wrong ?
		#endif
	}

	return valide;
}


//some usefull window tool
//SDL is the best for this, so lets him do this. 
//No more comment. It's easy really !!
void RxSDLOpenglContext::setWindowSize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	SDL_SetWindowSize(pSDLWindow, w, h);
}
void RxSDLOpenglContext::setWindowPosition(int x, int y)
{
	winCoordX = x;
	winCoordY = y;
	SDL_SetWindowPosition(pSDLWindow, x, y);
}
void RxSDLOpenglContext::setWindowName(const char * name)
{
	winName = name;
	SDL_SetWindowTitle(pSDLWindow, name);
}
int RxSDLOpenglContext::setWindowFullScreen(bool f)
{
	flagFullScreen = f;
	if(f)
		return SDL_SetWindowFullscreen(pSDLWindow, SDL_TRUE);
	return SDL_SetWindowFullscreen(pSDLWindow, SDL_FALSE);
}

void RxSDLOpenglContext::getWindowPosition(int *x, int *y)
{
	SDL_GetWindowPosition(pSDLWindow,x,y);
	winCoordX = *x;
	winCoordY = *y;
}

void RxSDLOpenglContext::getWindowSize( int *w, int *h)
{
	SDL_GetWindowSize(pSDLWindow,w,h);
	winWidth  = *w;
	winHeight = *h;
}
void RxSDLOpenglContext::SwapWindow()
{
	SDL_GL_SwapWindow(pSDLWindow);
}
RxSDLOpenglContext::~RxSDLOpenglContext()
{
		SDL_GL_DeleteContext(GLContext);
		SDL_DestroyWindow(pSDLWindow);
		SDL_Quit();
}
SDL_Window* RxSDLOpenglContext::getSDLWindow()
{
	return pSDLWindow;
}


/**************************************************************************/
/*
/*    RxEngine
/*
/*
/***************************************************************************/
RxEngine::RxEngine()
{
    RXLOGGER("[RxEngine] START"); 
	#ifdef _DEBUG
		RXCHECKGLERROR("[RxEngine][RxEngine]");
	#endif
} 


RxEngine::~RxEngine()
{
	RxTexManager::Instance().Destroy();
    RXLOGGER("[RxEngine] END");    
}

int RxEngine::Init(RxSDLOpenglContext *_pSDLOpenglContext)
{


	RXLOGGER("[RxEngine][Init] start");
	
	if (_pSDLOpenglContext == 0)
	{
		RXLOGGER("[RxEngine][Init] SDLOpenglContext ptr NULL");
		return RX_ERROR;
	}
	pSDLOpenglContext = _pSDLOpenglContext;
	defaultCamera.Init(pSDLOpenglContext->getSDLWindow());
	int w,h;
	pSDLOpenglContext->getWindowSize(&w,&h);
	defaultPerspective.set(70.0f, (float)w/h, 0.01f, 1000.0f);
	defaultCamera.setPosition(1,1,1);
	defaultCamera.setDirectionPoint(0,0,0);



	RXLOGGER("[RxEngine][InitGraphicObjects] start");
	int value = InitGraphicObjects();
	RXLOGGER("[RxEngine][InitGraphicObjects] end");

	#ifdef _DEBUG
	RXCHECKGLERROR("[RxEngine][Init]");
	#endif

	return value;
}

int RxEngine::InitGraphicObjects()
{
	return RX_OK;
}
void RxEngine::Loop()
{
	RXLOGGER("[RxEngine][Loop] Start Loop");
    
    while( Events() == RX_ENGINE_NEXTSTAP  )
    {
		DisplayFrames();

		#ifdef _DEBUG
		RXCHECKGLERROR("[RxEngine][Loop]");
		#endif
    } 

}

void RxEngine::ReSize()
{
	if(!pSDLOpenglContext)
		return;

	int w,h;
	pSDLOpenglContext->getWindowSize(&w,&h);
	glViewport(0,0,w,h);
	defaultPerspective.setAspect((float)w/h);

}

void RxEngine::DisplayFrames()
{
         glClear( GL_DEPTH_BUFFER_BIT );

  		 Frame();     

		 Display3D();

         Display2D();

         pSDLOpenglContext->SwapWindow();
}

int RxEngine::Events()
{
	    int returnValue = RX_ENGINE_NEXTSTAP;
	    
        SDL_Event event; // un evenement SDL
		//for all events
        while(SDL_PollEvent(&event))  //get the next event
        {
            
            switch(event.type)
            {
                //it's the end, so sad
                case SDL_QUIT:
                    return RX_ENGINE_EXIT;
                    break;

				//window event
				case SDL_WINDOWEVENT:
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE: //it's the end, so sad
							return RX_ENGINE_EXIT;
							break;
				        case SDL_WINDOWEVENT_RESIZED: //window resize
							ReSize();
							break;
					}
					break;

                //keyboard event
                case SDL_KEYDOWN:
					returnValue = KeyboardEvent(event.key.keysym.sym);
					break;
            } // switch end
        } // while end

        return returnValue;
}


int RxEngine::KeyboardEvent(SDL_Keycode key)
{
	 switch(key)
	 {
		case SDLK_ESCAPE:
			return RX_ENGINE_EXIT;
			break;
		case SDLK_SPACE:
			defaultCamera.setDirectionPoint(0,0,0);
			defaultCamera.EnableDisable();
			break;
	 }

	return RX_ENGINE_NEXTSTAP;
}
void RxEngine::Frame()
{

}


void RxEngine::Display3D()
{

}



void RxEngine::Display2D()
{

}



void RxEngine::DisplayPicking()
{

  
}



