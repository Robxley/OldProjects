

#include "RxProceduralTexture.h"





RxColorGradient::RxColorGradient()
{

}


void RxColorGradient::AddColorGradientPoint(float gradientPos, glm::vec4 color)
{
	unsigned int i;
	for(i = 0; i < points.size(); i++)
	{
		if(points[i] == gradientPos)
		{
			colors[i] = color;
			break;
		}
	}


	points.push_back(gradientPos);
	colors.push_back(color);
}

void RxColorGradient::TerrainColorGradient()
{
	Clear();
	AddColorGradientPoint (-1.0000, glm::vec4 (  0,   0, 128, 255)/255.0f ); // deeps
	AddColorGradientPoint (-0.2500, glm::vec4 (  0,   0, 255, 255)/255.0f ); // shallow
	AddColorGradientPoint ( 0.0000, glm::vec4 (  0, 128, 255, 255)/255.0f ); // shore
	AddColorGradientPoint ( 0.0625, glm::vec4 (240, 240,  64, 255)/255.0f ); // sand
	AddColorGradientPoint ( 0.1250, glm::vec4 ( 32, 160,   0, 255)/255.0f ); // grass
	AddColorGradientPoint ( 0.3750, glm::vec4 (224, 224,   0, 255)/255.0f ); // dirt
	AddColorGradientPoint ( 0.7500, glm::vec4 (128, 128, 128, 255)/255.0f ); // rock
	AddColorGradientPoint ( 1.0000, glm::vec4 (255, 255, 255, 255)/255.0f ); // snow
}

void RxColorGradient::GrayColorGradient()
{
		Clear();
		AddColorGradientPoint (-1.0000, glm::vec4 (  0,   0,   0,  1)); // deeps
		AddColorGradientPoint (1.0000, glm::vec4 (  1,   1,   1,  1)); // deeps
}
void RxColorGradient::Clear()
{
	points.clear();
	colors.clear(); 
}

unsigned int RxColorGradient::Size()
{
	return points.size();
}

void RxProceduralTexture::createTexture(RxDataProceduralTexture& texture, const char *name, int w, int h, Rx2DPerlinNoise perlinNoise, RxColorGradient *colorGradient)
{

	RxColorGradient grayGradient;
	grayGradient.TerrainColorGradient();
	if(colorGradient == NULL)
		colorGradient = &grayGradient;


	noise::module::Perlin perlinModule;
	perlinModule.SetOctaveCount (perlinNoise.octaveCount);
    perlinModule.SetFrequency (perlinNoise.frequency);
    perlinModule.SetPersistence (perlinNoise.persistence);
	perlinModule.SetSeed(perlinNoise.seed);

    noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane textureBuilder;
	textureBuilder.SetSourceModule (perlinModule);
	textureBuilder.SetDestNoiseMap (heightMap);
	textureBuilder.SetDestSize (w, h);
	textureBuilder.SetBounds (perlinNoise.leftLowerCorner.x,perlinNoise.rightUpperCorner.x,perlinNoise.leftLowerCorner.y,perlinNoise.rightUpperCorner.y);
	textureBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	for(unsigned int i = 0; i < colorGradient->Size(); i++)
	{
		 noise::utils::Color color (colorGradient->colors[i].r*255,
									colorGradient->colors[i].g*255,
									colorGradient->colors[i].b*255,
									colorGradient->colors[i].a*255);

		renderer.AddGradientPoint(colorGradient->points[i], color);
	}

	renderer.EnableLight ();
	renderer.SetLightContrast (3.0); // Triple the contrast
	renderer.SetLightBrightness (2.0); // Double the brightness
	renderer.Render();


	unsigned char* ptr = (unsigned char*) image.GetSlabPtr(); 	//Addresse de l'image

    texture.w = w;
	texture.h = h;
	texture.color_components = 4;
	texture.colors.resize(w*h*4,0);
	unsigned int k;
	for(k=0; k < w*h*4; k++)
		texture.colors[k] = ptr[k];

}
RxTexture& RxProceduralTexture::createGLTexture(const char *name, int w, int h, Rx2DPerlinNoise perlinNoise, RxColorGradient *colorGradient )
{

	RxColorGradient grayGradient;
	grayGradient.TerrainColorGradient();
	if(colorGradient == NULL)
		colorGradient = &grayGradient;


	noise::module::Perlin perlinModule;
	perlinModule.SetOctaveCount (perlinNoise.octaveCount);
    perlinModule.SetFrequency (perlinNoise.frequency);
    perlinModule.SetPersistence (perlinNoise.persistence);
	perlinModule.SetSeed(perlinNoise.seed);

    noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane textureBuilder;
	textureBuilder.SetSourceModule (perlinModule);
	textureBuilder.SetDestNoiseMap (heightMap);
	textureBuilder.SetDestSize (w, h);
	textureBuilder.SetBounds (perlinNoise.leftLowerCorner.x,perlinNoise.rightUpperCorner.x,perlinNoise.leftLowerCorner.y,perlinNoise.rightUpperCorner.y);
	textureBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	for(unsigned int i = 0; i < colorGradient->Size(); i++)
	{
		 noise::utils::Color color (colorGradient->colors[i].r*255,
									colorGradient->colors[i].g*255,
									colorGradient->colors[i].b*255,
									colorGradient->colors[i].a*255);

		renderer.AddGradientPoint(colorGradient->points[i], color);
	}

	renderer.EnableLight ();
	renderer.SetLightContrast (3.0); // Triple the contrast
	renderer.SetLightBrightness (2.0); // Double the brightness
	renderer.Render();

	GLuint ID = 0;
	glGenTextures(1,&ID);
	glBindTexture(GL_TEXTURE_2D,ID); 	
	glTexImage2D (
	GL_TEXTURE_2D, 	//Type : texture 2D
	0, 	//Mipmap : aucun
	GL_RGBA, 	//Couleurs : 4
	w, 	//Largeur : 2
	h, 	//Hauteur : 2
	0, 	//Largeur du bord : 0
	GL_RGBA, 	//Format : RGBA
	GL_UNSIGNED_BYTE, 	//Type des couleurs
	image.GetConstSlabPtr() 	//Addresse de l'image
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);
	RxTexManager::Instance().AddTexture(RxTexture(ID,name));

	return RxTexManager::Instance().getTextureTab().back();
}