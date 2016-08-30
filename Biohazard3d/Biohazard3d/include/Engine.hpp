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

/*!
\file Engine.hpp
\author Robxley (A.CAILLY)
\version 0.1
\date 09/12/2014
\~english
\brief Biohazard Engine - main class
\details The main class of the engine dealing the link with all other engine objects.
\~french
\brief Classe principale du moteur
\details La classe principale du moteur gérant la liaison entre tous les objets du moteur
*/


#ifndef _BH3D_ENGINE_H_
#define _BH3D_ENGINE_H_

#include "GLContext.hpp"


/*!Biohazard namespace*/
namespace BH3D
{

	/*!
	\author 	Robxley (A.CAILLY)
	\version	0.1
	\date		08/12/2014
	\~english
	\brief		Biohazard Engine - main class
	\remark		Dealing the link with all other engine objects. Includes the main loop of the program.
	\~french
	\brief		Classe principale du moteur biohazard3d
	\remark		Gérant la liaison entre tous les objets. Contient la boucle principale du programme.
	*/
	class Engine
	{

	public:
		/**
		*\~english
		*\brief		Constructor - Just writes "[Engine] Start" in logger file : logger.txt
		*\~french
		*\brief		Constructeur - Ne fait rien. Ecrit seulement "[Engine] Start" dans le fichier logger.txt
		*/
		Engine(); 

		/**
		*\~english
		*\brief		Destructor - Just writes "[Engine] End" in logger file : logger.txt
		*\~french
		*\brief		Destructeur - Ecrit seulement "[Engine] End" dans le fichier logger.txt
		*/
		~Engine();

		/**
		*\~english
		*\brief		Ininitialisation with a opengl context then call the function InitObjects.
		*\param[in]	pOpenGLContext		Pointer on GLContext object.
		*\return	Return the value of InitObjects function
		*\~french
		*\brief		Initialisation du moteur avec un context opengl puis appelle la fonction InitObjects
		*\param[in]	p_pObject		L'objet déplaçable
		*\return	Retourne la valeur de la fonction InitObjects
		*/
		int Init(GLContext *pOpenGLContext);    //initialise Engine avec le context opengl. return InitObjects.
		
		/**
		*\~english
		*\brief		Virtual function do nothing. Function called by Init function. Can be used with a derived function to initialise your objects
		*\return	Return BH3D_OK (true)
		*\~french
		*\brief		Fonction virtuelle appelée par Init. Peut être utilisée avec un fonction dérivée pour initialiser ses objets.
		*\return	Retourne BH3D_OK (true)
		*/
		virtual int InitObjects();		

		/**
		*\~english
		*\brief		Main engine loop. "while" loop on (pOpenGLContext->Events() != BH3D_EXIT). Calls Display function for each loop step.
		*\~french
		*\brief		Boucle principale du moteur. Boucle infinie sur la condition (pOpenGLContext->Events() != BH3D_EXIT) et fait un appel à la fonction Display à chaque boucle
		*/
		virtual void Loop(bool loopTest = 1);
		
		/**
		*\~english
		*\brief		Is Called by Loop function. You can use a derived function to display what you want for each loop.
		*\~french
		*\brief		Fonction appelée par la fonction Loop. Peut être dérivée pour afficher ce que l'on souhaite.
		*/
		virtual void Display() = 0; 
				

		
		virtual void ResizeCallBack(unsigned int w, unsigned int h);
	protected:
		GLContext *pGLContext;

	};
}
#endif //_BH3D_ENGINE_H_
