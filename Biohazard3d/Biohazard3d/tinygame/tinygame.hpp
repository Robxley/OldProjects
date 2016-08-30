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




 
#ifndef _BH3D_TINYGAME_H_
#define _BH3D_TINYGAME_H_

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


class TinyContext : public BH3D::SDLGLContext
{

	int Events(){ return BH3D_OK; }
};



class TinyEngine : public BH3D::Engine
{
public:

	TinyEngine();
	~TinyEngine();
	void Loop(bool loopTest = 1);
private:
	glm::mat4 projection, lookAt, transform;
	BH3D::Shader tinyShader;
	BH3D::Font tinyFont;
	BH3D::Fps tinyFps;

	//ship ddata
	BH3D::Mesh tinyShipMesh;
	glm::mat4 shipTransform;
	glm::vec3 shipTranslate;
	float rotationShip;

	float topPosition = 25.0f;

	//2d data
	std::string	nameGame = "Tiny game engine !!!";
	unsigned int lengthNameGame;

	//mouse/touch event
	int mousex = 0, mousey = 0;
	
	void ResizeCallBack(unsigned int w, unsigned h);


	int InitObjects();



	void Display();

	int Events();

	void TinyGame();

	void Display3D();
	void Display2D();	
};






#endif //
