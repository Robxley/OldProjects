//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

//#============================================================#
//#        Copyright © 2007, XT95, All rights reserved.        #
//#============================================================#


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif
#include <iostream>

#include "RxFBO.h"



//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
RxFBO::RxFBO():frameBuffer(0),depthBuffer(0),colorBuffer(0),
	colorTextureID(0),depthTextureID(0),
	useColorBuffer(1),useDepthBuffer(1)
{

}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void RxFBO::Init(int _colorBuffer, int _depthBuffer, int _width, int _height )
{
    
	useColorBuffer = _colorBuffer;
	useDepthBuffer = _depthBuffer;

	if( !_width || !_height)
	{
		GLint viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );
		//viewport[0]=x
		//viewport[1]=y
		//viewport[2]=width
		//viewport[3]=height 
		width = viewport[2];
		height = viewport[3];
	}

	
    glGenFramebuffers(1, &frameBuffer); //creation d'un frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	if(useColorBuffer)
	{
		if(useColorBuffer == 1 || useColorBuffer == 3)
		{
			//create a render buffer
			glGenRenderbuffers(1, &colorBuffer); 
			glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height); 
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);
		}
		if(useColorBuffer > 1 )
		{
			//Create a color texture buffer.
			glGenTextures(1, &colorTextureID);
			glBindTexture(GL_TEXTURE_2D, colorTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);
		}
	}

	if(useDepthBuffer)
	{
		if(useDepthBuffer == 1 || useDepthBuffer == 3)
		{
			glGenRenderbuffers(1, &depthBuffer);  //creation de rendu de profondeur lié au frame buffer
			glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); //allocation de la mémoire
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer); //attache  le renderbuffer au fbo
		}
		if(useDepthBuffer> 1)
		{
			//Create a depth texture.
			glGenTextures(1, &depthTextureID);
			glBindTexture(GL_TEXTURE_2D, depthTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);
		}
	}
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, size.x, size.y);

	#ifdef _DEBUG
		CheckError();
	#endif

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);;

}

//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void RxFBO::setViewport( glm::ivec2 size )
{
	width = size.x;
	height = size.y;

	if(colorBuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA, width, height);
	}
	if(depthBuffer)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT, width, height);
	}
	if(colorTextureID)
	{
		//Create a color texture.
		glBindTexture(GL_TEXTURE_2D, colorTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	
	if(depthTextureID)
	{
		//Create a depth texture.
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

}
 void RxFBO::setViewport( unsigned int w, unsigned int h)
 {
	setViewport(glm::ivec2(w,h));
 }
//-----------------------------------------------------------------------------
// Enable
//-----------------------------------------------------------------------------
void RxFBO::Enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glGetIntegerv( GL_VIEWPORT, viewportSaver ); //sauvegarde le wievport
	glViewport(0,0,width, height);
	

	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthAndStencilBuffer);
	//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthAndStencilBuffer);
	
	//Bind the textures.
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTextureID, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTextureID, 0);


	
	
}

//-----------------------------------------------------------------------------
// Disable
//-----------------------------------------------------------------------------
void RxFBO::Disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewportSaver[0],viewportSaver[1],viewportSaver[2],viewportSaver[3]);
}

//-----------------------------------------------------------------------------
// GetColor
//-----------------------------------------------------------------------------
GLuint RxFBO::getColorMap()
{
    return colorTextureID;
}

//-----------------------------------------------------------------------------
// GetDepth
//-----------------------------------------------------------------------------
GLuint RxFBO::getDepthMap()
{
    return depthTextureID;
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
RxFBO::~RxFBO()
{
	if(colorTextureID) glDeleteTextures(1, &colorTextureID);
	if(depthTextureID) glDeleteTextures(1, &depthTextureID);
	if(depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);
	if(colorBuffer) glDeleteRenderbuffers(1, &colorBuffer);
	if(frameBuffer) glDeleteFramebuffers(1, &frameBuffer);

}


glm::ivec2 RxFBO::getSize()
{
	return glm::ivec2(width, height);
}

GLuint RxFBO::getFrameBuffer()
{
	return frameBuffer;
}

#ifdef _DEBUG

int RxFBO::CheckError()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			RXLOGGER("[RXFBO][CheckError] None ");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT ");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			RXLOGGER("[RXFBO] Check : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ");
			break;
		case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE ");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_UNSUPPORTED ");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ");
			break;
		case GL_FRAMEBUFFER_UNDEFINED:
			RXLOGGER("[RXFBO][CheckError] error : GL_FRAMEBUFFER_UNDEFINED ");
			break;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return status;
}

#endif