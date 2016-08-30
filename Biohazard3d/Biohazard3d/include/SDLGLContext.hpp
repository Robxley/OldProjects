/*
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


#ifndef _BH3D_SDLGLCONTEXT_H_
#define _BH3D_SDLGLCONTEXT_H_


#include "MultiPlatformOpenGL.hpp"

#ifdef BH3D_HAVE_OPENGLES	//opengles 2.0
	#include <SDL_opengles2.h>
	#define BH3D_GL_CONTEXT_MAJOR_VERSION 2
#endif



#include <SDL.h>
#include "Common.hpp"
#include "Assertion.hpp"
#include "Logger.hpp"
#include "GLErrorLogger.hpp"
#include "GLContext.hpp"

namespace BH3D
{
	class SDLGLContext : public GLContext
    {
		private:
			SDL_Window *pSDLWindow;
			SDL_GLContext openGLContext;

			//structure des données inputs mis à jour par la fonction UpdateEvent
			typedef struct
			{
				std::vector<bool> key;		// 1 : keydown, keyup
				int mousex = 0, mousey = 0;			// mouse position
				int mousexrel = 0, mouseyrel = 0;	//
				char mousebuttons[8];		//bouton sourie
				int quit = 0;					//quit event 
				bool isTouchDevice = 0;
				int finger[10][2]; //position x,y des 10 doigts eventuels (qui a plus ?)
				bool fingerUsed[10]; //les doigts qui touche l'écran.
			} SDLInput;

			SDLInput sdlInput; // GLContext::input = (void*) &sdlInput;

        public:

			SDLGLContext();
			SDLGLContext(const char *name, int w, int h, int x = BH3D_WINDOW_COORD_X, int y = BH3D_WINDOW_COORD_Y);
			~SDLGLContext();

			//gère les evenements (fait appel au fonction ReSize et KeyboardEvent)
			virtual int Events();

			virtual void SwapWindow();

			virtual void SetWindowSize(int w, int h);
			virtual void SetWindowPosition(int x, int y);
			virtual void SetWindowName(const char * name);
			virtual void SetWindowFullScreen(bool f);

			virtual void GetWindowPosition(int *x, int *y) const;
			virtual void GetWindowSize(int *w, int *h) const;

		protected:

			virtual void ReSize();		
			virtual void UpdateEvents(SDL_Event *event);

		private:
			int InitSDLGL();

    };

	//inline

	inline void SDLGLContext::SwapWindow()
	{
		SDL_GL_SwapWindow(pSDLWindow);
	}

	//some usefull window tool
	//SDL is the best for this, so lets him do this.
	//No more comment. It's easy really !!
	inline void SDLGLContext::SetWindowSize(int w, int h)
	{
		winWidth = w;
		winHeight = h;
		SDL_SetWindowSize(pSDLWindow, w, h);
		ReSize();
	}
	inline void SDLGLContext::SetWindowPosition(int x, int y)
	{
		winCoordX = x;
		winCoordY = y;
		SDL_SetWindowPosition(pSDLWindow, x, y);
	}
	inline void SDLGLContext::SetWindowName(const char * name)
	{
		winName = name;
		SDL_SetWindowTitle(pSDLWindow, name);
	}
	inline void SDLGLContext::SetWindowFullScreen(bool f)
	{
		SDL_SetWindowFullscreen(pSDLWindow, f);
		ReSize();
	}

	inline void SDLGLContext::GetWindowPosition(int *x, int *y) const
	{
		SDL_GetWindowPosition(pSDLWindow, x, y);
	}

	inline void SDLGLContext::GetWindowSize(int *w, int *h) const
	{
		SDL_GetWindowSize(pSDLWindow, w, h);
	}

}
#endif 
