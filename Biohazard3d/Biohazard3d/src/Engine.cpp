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








#include "Engine.hpp"
#include "MultiPlatformOpenGL.hpp"
#include "Logger.hpp"


namespace BH3D
{
	Engine::Engine()
	{
		BH3D_LOGGER("[Engine] Start");
	}
	Engine::~Engine()
	{
		BH3D_LOGGER("[Engine] End");
	}
	int Engine::Init(GLContext *pOpenGLContext)
	{
		if (pOpenGLContext == nullptr) return BH3D_ERROR;
		
		pGLContext = pOpenGLContext;

		pGLContext->ReSizeCallBack = [=](unsigned int w, unsigned int h) {
			this->ResizeCallBack(w, h); };

		return InitObjects();
	}
	int Engine::InitObjects()
	{
		
		return BH3D_OK; 
	}

	void Engine::Loop(bool loopTest)
	{
		BH3D_LOGGER("Start Loop");

		while ((pGLContext->Events() == BH3D_OK) && loopTest)
		{
			Display();
			pGLContext->SwapWindow();
		}

	}
	void Engine::ResizeCallBack(unsigned int w, unsigned int h)
	{

	}


}