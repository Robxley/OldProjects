//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXSKYBOX_H_
#define _RXSKYBOX_H_

#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif
#include <vector>
#include <string>


#include "RxTexManager.h"
#include "RxConst.h"
#include "RxVBO.h"
#include "RxShader.h"
#include "RxTools.h"
#include "RxMath3D.h"

//position
#define RX_SKYBOX_X 0.0f
#define RX_SKYBOX_Y 0.0f
#define RX_SKYBOX_Z 0.0f

//size
#define RX_SKYBOX_LENGHT 1.0f 
#define RX_SKYBOX_HEIGHT 1.0f 
#define RX_SKYBOX_WIDTH  1.0f 

#define RX_SKYBOX_OFFSET 

#define RX_SKYBOX_VERTSHADER "Shaders/skybox.vert"
#define RX_SKYBOX_FRAGSHADER "Shaders/skybox.frag"


class RxSkyBox
{
private:
	RxIDTexture top,bottom,front,back,right,left;
	RxIDTexture cubeMapSkyBox;
	RxVBO skyBoxVBO;
	RxShader skyBoxShader;
	float width, height, length;
	std::string vertexShader, fragmentShader;

	glm::vec4 color;

public:
	RxSkyBox( RxIDTexture _top = 0,   RxIDTexture _bottom = 0,
			  RxIDTexture _front = 0, RxIDTexture _back = 0,
			  RxIDTexture _left = 0, RxIDTexture  _right= 0,
			  const char *_vertShader = RX_SKYBOX_VERTSHADER, const char *_fragShader = RX_SKYBOX_FRAGSHADER,
			  glm::vec4 _color = glm::vec4(1.0f,1.0f,1.0f,1.0f),
			  float _width = 10.0f, float _height = 10.0f, float _length = 10.0f);


	~RxSkyBox();

	void Init(); //initialise le shader et vbo

	void setSkyBoxTexture( RxIDTexture _top,   RxIDTexture _bottom,
					  RxIDTexture _front, RxIDTexture _back,
					  RxIDTexture _left, RxIDTexture _right);
	void setColor(glm::vec4& _color);

	//dessine l'objet
	void Draw(const glm::mat4 &projection, const glm::mat4 &modelview);


	void SetSize(float _width = 10.0f, float _height = 10.0f, float _length = 10.0f);//necessite de rappeler init
};

#endif




