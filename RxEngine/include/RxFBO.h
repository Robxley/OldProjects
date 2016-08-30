//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXFBO_H_
#define _RXFBO_H_

#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif
#include "RxLogger.h"
#include "RxMath3D.h"

//type de buffer
#define RX_NONE_RENDER_BUFFER 0
#define RX_RENDER_BUFFER 1
#define RX_TEXTURE2D_BUFFER 2
#define RX_RENDER_AND_TEXTURE2D_BUFFER 3


//-----------------------------------------------------------------------------
// FBO Class
//-----------------------------------------------------------------------------
class RxFBO
{
public:
	RxFBO();
	~RxFBO();

	void Init(int colorBuffer = RX_TEXTURE2D_BUFFER, int depthBuffer = RX_TEXTURE2D_BUFFER, int _width = 0, int _height =0 );
	void Enable();
	void Disable();

	void setViewport( glm::ivec2 size );
	void setViewport( unsigned int w, unsigned int h);
	GLuint getColorMap();
	GLuint getDepthMap();
	GLuint getFrameBuffer();

	glm::ivec2 getSize();

#ifdef _DEBUG
	int CheckError();
#endif
       
private:
	int useColorBuffer;
	int useDepthBuffer;

	GLuint frameBuffer;
	GLuint depthBuffer;
	GLuint colorBuffer;
	GLuint colorTextureID;
	GLuint depthTextureID;

	unsigned int width;
	unsigned int height;

	GLint viewportSaver[4];
   

};

#endif

