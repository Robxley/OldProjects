/*"
 * The MIT License
 *
 * Copyright 2014 Robxley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "SDLGLContext.hpp"





namespace BH3D
{
	SDLGLContext::SDLGLContext()
	{
		isCreated = InitSDLGL();
	}
	SDLGLContext::SDLGLContext(const char *name, int w, int h, int x, int y)
	{
		winWidth = w;
		winHeight = h;
		winCoordX = x;
		winCoordY = y;
		winName = name;
		isCreated = InitSDLGL();
	}
	SDLGLContext::~SDLGLContext()
	{
		// Destruction du contexte OpenGL, de la vue principale et fin de l'application
		SDL_GL_DeleteContext(openGLContext);
		SDL_DestroyWindow(pSDLWindow);
		SDL_Quit();
	}


	int SDLGLContext::InitSDLGL()
	{
		// SDL Initialisation
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{

			BH3D_LOGGERERROR("SDL_Init -> " << SDL_GetError());
			BH3D_ASSERT(0);
			return BH3D_ERROR;
		}
		else
			BH3D_LOGGER("INIT SDL -> OK !");

#ifdef BH3D_HAVE_OPENGLES
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, BH3D_GL_CONTEXT_MAJOR_VERSION);
#else
		// Opengl and GLSL Version&core

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, BH3D_GL_CONTEXT_MAJOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, BH3D_GL_CONTEXT_MINOR_VERSION);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

		// CENTERED or not ?
		if (winCoordX < 1 || winCoordY < 1)
		{
			winCoordX = SDL_WINDOWPOS_CENTERED;
			winCoordY = SDL_WINDOWPOS_CENTERED;
		}

		//My window, MY WINDOW DUDE !!!!!!!
#ifdef BH3D_HAVE_OPENGLES
		pSDLWindow = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
#else
		pSDLWindow = SDL_CreateWindow(winName.c_str(), winCoordX, winCoordY, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
#endif	
		if (pSDLWindow == 0)
		{
			//!*%&?#& window fail!
			BH3D_LOGGERERROR("SDL_CreateWindow -> " << SDL_GetError());		
			SDL_Quit();
			BH3D_ASSERT(0);
			return BH3D_ERROR;
		}

		//on n'a pas définie la résolution pr les environnements OPENGLES (android, ios)
#ifdef BH3D_HAVE_OPENGLES
		SDL_GetWindowSize(pSDLWindow, &winWidth, &winHeight);
#endif

		// Double Buffer
		if (doubleBuffer)
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//auxiliaire buffers
		if (dephtBufferSize)
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, dephtBufferSize);

		if (stencilBufferSize)
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilBufferSize);//flag_stencilSize8

		//we love opengl
		openGLContext = SDL_GL_CreateContext(pSDLWindow);
		if (openGLContext == 0)
		{
			//opengl don't like us

			BH3D_LOGGERERROR("SDL_GL_CreateContext -> " << SDL_GetError());
			SDL_DestroyWindow(pSDLWindow);
			SDL_Quit();
			BH3D_ASSERT(0);
			return BH3D_ERROR;
		}

		//Not so fast, you can not see everything (60fps is enough)
		if (bufferSynchronize)
			SDL_GL_SetSwapInterval(1);
		
		//We need help with opengl functions and extensions
		//Why make something already done.
#ifdef _WIN32

		// Si l'initialisation a échouée :
		glewExperimental = GL_TRUE;
		GLenum glewInitOK = glewInit();

		if (glewInitOK != GLEW_OK)
		{

			// On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)
			BH3D_LOGGERERROR("GLEW INIT FAIL -> " << glewGetErrorString(glewInitOK));

			// On quitte la SDL
			SDL_GL_DeleteContext(openGLContext);
			SDL_DestroyWindow(pSDLWindow);
			SDL_Quit();
			BH3D_ASSERT(0);
			return BH3D_ERROR;
		}

		//deal with glew BUG INVALID ENUM AFTER init
		if (glGetError() == 1280)
			BH3D_LOGGER("THE glewInit known bug -> invalid enum");

		glViewport(0, 0, winWidth, winHeight);

		BH3D_LOGGER("GLEW INIT OK");
#endif
		
		//Init des inputs de la SDL

		GLContext::input = (void*)&sdlInput;
		sdlInput.key.resize(SDL_NUM_SCANCODES, 0);
		
		//si écran tactile ou pas
		if (SDL_GetNumTouchDevices())
		{
			sdlInput.isTouchDevice = 1;

			for (int i = 0; i < 10; i++)
				sdlInput.fingerUsed[10] = 0;
		}
	
		//look what we get....
		BH3D_LOGGER("OpenGL Core :  " << glGetString(GL_VERSION) << " - (GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << ')');


#ifndef NDEBUG
		BH3D_CHECKGLERROR(); //is something wrong ?
#endif

		return BH3D_OK; //fine
	}


	int SDLGLContext::Events()
	{
		int returnValue = BH3D_OK;
		SDL_Event event;
		
		while (SDL_PollEvent(&event))  //get the next event
		{
			UpdateEvents(&event);
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
					ReSize();
					break;
				case SDL_WINDOWEVENT_RESTORED:
					ReSize();
					break;
				}
				break;

			case SDL_FINGERDOWN:  //3 doigts pour quitter
				if(event.tfinger.fingerId >= 2)
					return BH3D_EXIT;
				break;

				//keyboard event
			case SDL_KEYDOWN:   //ESCAPE pour quitté
				returnValue = (event.key.keysym.sym == SDLK_ESCAPE) ? BH3D_EXIT : returnValue;
				break;
			default:
				break;
			} // switch end
		} // while end

		return returnValue;
	}

	void SDLGLContext::ReSize()
	{
		if (!openGLContext)
			return;

		GetWindowSize(&winWidth, &winHeight);
		glViewport(0, 0, winWidth, winHeight);
		ReSizeCallBack(winWidth, winHeight);

	}

	void SDLGLContext::UpdateEvents(SDL_Event *event)
	{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				sdlInput.key[event->key.keysym.sym] = 1;
				break;
			case SDL_KEYUP:
				sdlInput.key[event->key.keysym.sym] = 0;
				break;
			case SDL_MOUSEMOTION:
				sdlInput.mousex = event->motion.x;
				sdlInput.mousey = event->motion.y;
				sdlInput.mousexrel = event->motion.xrel;
				sdlInput.mouseyrel = event->motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				sdlInput.mousebuttons[event->button.button] = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				sdlInput.mousebuttons[event->button.button] = 0;
				break;
			case SDL_QUIT:
				sdlInput.quit = 1;
				break;
			case SDL_FINGERDOWN:
				sdlInput.finger[event->tfinger.fingerId][0] = event->tfinger.x*winWidth;
				sdlInput.finger[event->tfinger.fingerId][1] = event->tfinger.y*winHeight;
				sdlInput.fingerUsed[event->tfinger.fingerId] = 1;
				break;
			case SDL_FINGERMOTION:
				sdlInput.finger[event->tfinger.fingerId][0] = event->tfinger.x*winWidth;
				sdlInput.finger[event->tfinger.fingerId][1] = event->tfinger.y*winHeight;
				sdlInput.fingerUsed[event->tfinger.fingerId] = 1;
				break;
			case SDL_FINGERUP:
				sdlInput.fingerUsed[event->tfinger.fingerId] = 0;
				break;
			default:
				break;
			}
		
	}

	

}

