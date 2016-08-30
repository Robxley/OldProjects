//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


/////////////////////////////////////////////////////////////////////////////
//
// 
// 
//
// 
//
/////////////////////////////////////////////////////////////////////////////




#pragma once

#ifndef _RXPROCEDURALTEXTURE_H_
#define _RXPROCEDURALTEXTURE_H_

#include <noise/noise.h>
#include <noise/noiseutils.h>
#include <ctime>
#include <vector>

#include "RxTexManager.h"
#include "RxMath3D.h"

enum RX_COLOR_COMPONENT
{
	RX_NONE_COMPONENT,
	RX_RED_COMPONENT,
	RX_GREEN_COMPONENT,
	RX_BLUE_COMPONENT,
	RX_ALPHA_COMPONENT,
};

enum RX_COLOR_FORMAT
{
	RX_NONE_FORMAT,
	RX_R_FORMAT,
	RX_RG_FORMAT,
	RX_RGB_FORMAT,
	RX_RGBA_FORMAT,
};



class RxColorGradient
{
	public:
		std::vector<float> points;
		std::vector<glm::vec4> colors;

		RxColorGradient();
		void AddColorGradientPoint(float gradientPos, glm::vec4 color);

		void TerrainColorGradient();
		void GrayColorGradient();

		void Clear();

		unsigned int Size();

};


typedef struct _RxDataProceduralTexture
{
	unsigned int w,h;
	int color_components;
	std::vector<unsigned char> colors;

} RxDataProceduralTexture;


class RxProceduralTexture
{
	std::vector<RxDataProceduralTexture*> tab_texture;


	public:
	static RxTexture& createGLTexture( const char *name, int w = 64, int h = 64, Rx2DPerlinNoise perlinNoise = Rx2DPerlinNoise(), RxColorGradient *colorGradient = NULL);
	static void createTexture(RxDataProceduralTexture& data, const char *name, int w = 64, int h = 64, Rx2DPerlinNoise perlinNoise = Rx2DPerlinNoise(), RxColorGradient *colorGradient = NULL);
	static RxTexture& createGLArrayTexture(const char *name, int nb_texture, std::vector<RxDataProceduralTexture*> &tab_texture);
};

#endif