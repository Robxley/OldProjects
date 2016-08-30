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


#ifndef _BH3D_GLCONTEXT_H_
#define _BH3D_GLCONTEXT_H_


///Opengl Context options
#define BH3D_FULLSCREEN             false
#define BH3D_DOUBLEBUFFER           true
#define BH3D_BUFFERSYNCHRONIZE      true
#define BH3D_MULTISAMPLESIZE        4
#define BH3D_DEPHTBUFFERSIZE        24
#define BH3D_STENCILBUFFERSIZE      8


///Window default options
#define BH3D_WINDOW_WIDTH           1024
#define BH3D_WINDOW_HEIGHT          768
#define BH3D_WINDOW_COORD_X         -1
#define BH3D_WINDOW_COORD_Y         -1
#define BH3D_WINDOW_NAME			"Biohazard3D Window"

#include <string>
#include <vector>
#include <functional>
#include "Common.hpp"

namespace BH3D
{
	/*!
	\author 	Robxley (A.CAILLY)
	\version	0.1
	\date		08/12/2014
	\~english
	\brief		Virtual class for the creation of opengl context.
	\remark		Standard routines are defined to be used with the Engine (class Engine). Uses a derived class to create a Opengl Context with a dedicated LIB like SDL, GLUT, QT etc...
	\~french
	\brief		Classe virtel pour la gestion d'un context opengl.
	\remark		Plusieurs routines standards ont été définies pour être utilisées avec la class Engine. Une classe dérivée peut être utilisée pour créer un context opengl à partir d'une librairie dédiée tels que SDL, GLUT, QT etc...
	*/
    class GLContext
    {

		public:
   
			/**
			*\~english
			*\brief		Constructor - Do nothing
			*\~french
			*\brief		Constructeur - Ne fait rien
			*/
			//GLContext();

			/**
			*\~english
			*\brief		Virtual Destructor. Has to define with the derived class.
			*\~french
			*\brief		Destructeur virtuel. Doit être définie dans la classes dérivée.
			*/
			//virtual ~GLContext() = 0;

			/**
			*\~english
			*\brief		Checks if the context is created without error.
			*\return	return BH3D_OK (true) or BH3D_ERROR (false)
			*\~french
			*\brief		Verifie si le contexte opengl a été crée.
			*\return	retourne BH3D_OK (true) ou BH3D_ERROR (false)
			*/
			virtual int IsCreated();

			/**
			*\~english
			*\brief		Standard virtual fonction used by the Engine class to swap the window buffer (double buffer).
			*\~french
			*\brief		Fonction virtelle utilisée par la classe Engine qui échange les buffers de la fenêtre (double buffer).
			*/
            virtual void SwapWindow() = 0;

			/**
			*\~english
			*\brief		Resize windows routine. Calls resizeCallBack pointer function with param : w, h dimensions windows
			*\~french
			*\brief		Fonction appelée lors de redimensionnement de la fenetre. Appèle la fonction resizeCallBack 
			*/
			virtual void ReSize();

			/**
			*\~english
			*\brief		Standard virtual fonction used by the Engine class to leave the main loop when the function return BH3D_EXIT.
			*\return    Return BH3D_EXIT (false) to leave the main loop.
			*\~french
			*\brief		Fonction virtelle utilisée par la classe Engine pour quitter la boucle principale.
			*\return	Return BH3D_EXIT (false) to leave the main loop.
			*/
			virtual int Events() = 0;

			/**
			*\~english
			*\brief		Puts the window dimention (width, height).
			*\param[in]	Window width.
			*\param[in]	Window height.
			*\~french
			*\brief		Definit les dimensions de la fenêtre.
			*\param[in]	Largeur de la fenêtre.
			*\param[in]	Hauteur de la fenêtre.
			*/
			virtual void SetWindowSize(int w, int h) = 0;
			
			/**
			*\~english
			*\brief		Puts the window position on the screen.
			*\param[in]	Window x position (Horizontal)
			*\param[in]	Window y position (Vertical)
			*\~french
			*\brief		Definit la postion de la fenêtre sur l'écran.
			*\param[in]	Position par rapport à la coordonnée x (Horizontal)
			*\param[in]	Position par rapport à la coordonnée y (Vertical)
			*/
			virtual void SetWindowPosition(int x, int y) = 0;

			/**
			*\~english
			*\brief		Puts the window name.
			*\param[in]	Window name.
			*\~french
			*\brief		Definit le nom de la fenêtre.
			*\param[in]	Le nom de la fenêtre.
			*/
			virtual void SetWindowName(const char * name) = 0;

			/**
			*\~english
			*\brief		Puts the window in fullscreen or not.
			*\param[in]	true or false. So Easy.
			*\~french
			*\brief		Met la fenêtre en plein écran ou non.
			*\param[in]	true or false. Tellement simple
			*/
			virtual void SetWindowFullScreen(bool f) = 0;

			/**
			*\~english
			*\brief		Gets the window position.
			*\param[in]	Pointer to save the x position.
			*\param[in]	Pointer to save the y position.
			*\~french
			*\brief		Pour obtenir la dimension de la fenêtre.
			*\param[in]	Pointeur pour sauvegarder la position horizontal (x)
			*\param[in]	Pointeur pour sauvegarder la position vertical (y)
			*/
			inline void GetWindowPosition(int *x, int *y) const;

			/**
			*\~english
			*\brief		Gets the window dimension.
			*\param[in]	Pointer to save the width.
			*\param[in]	Pointer to save the height.
			*\~french
			*\brief		Pour obtenir la dimension de la fenêtre.
			*\param[in]	Pointeur pour sauvegarder la largeur
			*\param[in]	Pointeur pour sauvegarder la hauteur
			*/
			inline void GetWindowSize(int *w, int *h) const;

			/**
			*\~english
			*\brief		Gets the window width.
			*\return	The width.
			*\~french
			*\brief		Pour obtenir la dimension de la fenêtre.
			*\return	 La largeur
			*/
			inline int GetWinWidth() const;
			
			/**
			*\~english
			*\brief		Gets the window height.
			*\return	The height.
			*\~french
			*\brief		Pour obtenir la hauteur de la fenêtre.
			*\return	La hauteur
			*/
			inline int GetWinHeight() const;

			/**
			*\~english
			*\brief		Gets the X window position.
			*\return	X window position.
			*\~french
			*\brief		Pour obtenir la position X de la fenêtre.
			*\param[in]	Position X de la fenêtre
			*/
			inline int GetWinCoordX() const;

			/**
			*\~english
			*\brief		Gets the Y window position.
			*\return	Y window position.
			*\~french
			*\brief		Pour obtenir la position Y de la fenêtre.
			*\param[in]	Position Y de la fenêtre
			*/
			inline int GetWinCoordY() const;

			//Call Back functions will be called by ReSize with w,h windows dimension. Do nothing by default.
			std::function<void(unsigned int, unsigned int)> ReSizeCallBack = [=] (unsigned int w, unsigned int h){};

	protected:
			int winWidth = BH3D_WINDOW_WIDTH, winHeight = BH3D_WINDOW_HEIGHT;
			int winCoordX = BH3D_WINDOW_COORD_X, winCoordY = BH3D_WINDOW_COORD_Y;
			std::string winName = BH3D_WINDOW_NAME;

			bool fullScreen = BH3D_FULLSCREEN;			//Default : BH3D_FULLSCREEN
			bool doubleBuffer = BH3D_DOUBLEBUFFER;			//Default : BH3D_DOUBLEBUFFER
			bool bufferSynchronize = BH3D_BUFFERSYNCHRONIZE;		//Default : BH3D_BUFFERSYNCHRONIZE
			int multiSampleSize = BH3D_MULTISAMPLESIZE;		//Default : BH3D_MULTISAMPLESIZE
			int dephtBufferSize = BH3D_DEPHTBUFFERSIZE;		//Default : BH3D_DEPHTBUFFERSIZE
			int stencilBufferSize = BH3D_STENCILBUFFERSIZE;		//Default : BH3D_STENCILBUFFERSIZE

			int isCreated = BH3D_ERROR;

			void * input = nullptr;		//utiliser par la fonction uptdateEvents pour gérer les inputs

    };



	inline int GLContext::GetWinWidth() const
	{
		return winWidth;
	}
	inline int GLContext::GetWinHeight() const
	{
		return winHeight;
	}
	inline int GLContext::GetWinCoordX() const
	{
		return winCoordX;
	}

	inline int GLContext::GetWinCoordY() const
	{
		return winCoordY;
	}

	inline void GLContext::GetWindowPosition(int *x, int *y) const
	{
		(*x) = winCoordX;
		(*y) = winCoordY;
	}

	inline void GLContext::GetWindowSize(int *w, int *h) const
	{
		(*w) = winWidth;
		(*h) = winHeight;
	}

	inline int GLContext::IsCreated() { return isCreated; };

	inline void GLContext::ReSize()
	{
		ReSizeCallBack(winWidth, winHeight);
	}

}
#endif // __BH_TEMPLATE__
