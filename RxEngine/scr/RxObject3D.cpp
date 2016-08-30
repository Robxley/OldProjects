//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxObject3D.h"

/*************************************************************************************************************************/
/*************************************************************************************************************************/
/**************RxLight****************************************/
RxLight::RxLight():
enable(RX_ON),
pos(10.0f,-10.0f,45.0f,RX_PUNCTUAL_LIGHT),
dir(-1.0f,-1.0f,-1.0f),
color(1.0f,1.0f,1.0f,1.0f),
diffuse(0.5f,0.5f,0.5f,1.0f),
ambient(0.5f,0.5f,0.5f,1.0f),
specular(0.8f,0.6f,0.5f,1.0f),
attenuation(0.5f,0.0f,0.0002f)
{



}
RxLight::RxLight(glm::vec4 position, glm::vec4 direction, bool _enable):
enable(_enable),
pos(position),dir(direction),
color(1.0f,1.0f,1.0f,1.0f),
diffuse(0.5f,0.5f,0.5f,1.0f),
ambient(0.5f,0.5f,0.5f,1.0f),
specular(0.8f,0.6f,0.5f,1.0f),
attenuation(0.5f,0.0f,0.0002f)
{

}

bool RxLight::IsPunctual()
{ 
	if(pos.w == RX_PUNCTUAL_LIGHT)
		return true;
	return false;
}
bool RxLight::IsDirectional()
{ 
	if(pos.w == RX_DIRECTIONAL_LIGHT)
		return true;
	return false;
}

float RxLight::getTypeLight() {return pos.w;}

/*************************************************************************************************************************/
/*************************************************************************************************************************/
/**************RxMaterial****************************************/

RxMaterial::RxMaterial(): 
color( 1.0f,1.0f,1.0f,1.0f), ambiant(1.0f,1.0f,1.0f,1.0f),diffuse(0.6f,0.6f,0.6f,1.0f),
specular(0.4f,0.4f,0.4f,1.0f), shininess(30.0f), colorMap(0),normalMap(0),heightMap(0),
idFragmentColorMap(GL_TEXTURE0), idFragmentNormalMap(GL_TEXTURE1), idFragmentHeightMap(GL_TEXTURE2)
{
	
}

void RxMaterial::Bind()
{
 	if(normalMap.glID)
	{
		glActiveTexture(idFragmentNormalMap); 
		if( normalMap.type == RX_ARRAY_TEX )
			glBindTexture(GL_TEXTURE_2D_ARRAY, normalMap);
		else
			glBindTexture(GL_TEXTURE_2D, normalMap);
	}

	if(colorMap.glID)
	{
		glActiveTexture(idFragmentColorMap);
		if( colorMap.type == RX_ARRAY_TEX )
			glBindTexture(GL_TEXTURE_2D_ARRAY, colorMap);
		else
			glBindTexture(GL_TEXTURE_2D, colorMap);
	}

	if(heightMap.glID)
	{
		glActiveTexture(idFragmentHeightMap);
		if( heightMap.type == RX_ARRAY_TEX )
			glBindTexture(GL_TEXTURE_2D_ARRAY, heightMap);
		else
			glBindTexture(GL_TEXTURE_2D, heightMap);
	}

}


void RxMaterial::setColorMap(const char *filename)
{
	colorMap = RxTexManager::Instance().LoadTexture(filename);
}
void RxMaterial::setNormalMap(const char *filename)
{
	normalMap = RxTexManager::Instance().LoadTexture(filename);
}
void RxMaterial::setHeighMap(const char *filename)
{
	heightMap = RxTexManager::Instance().LoadTexture(filename);
}

void RxMaterial::setColorMap(GLuint id)
{
	colorMap = RxTexManager::Instance().getTexture(id);
}
void RxMaterial::setNormalMap(GLuint id)
{
	normalMap = RxTexManager::Instance().getTexture(id);
}
void RxMaterial::setHeighMap(GLuint id)
{
	heightMap = RxTexManager::Instance().getTexture(id);
}

void RxMaterial::setColorMap(RxTexture &texture)
{
	colorMap = texture;
}
void RxMaterial::setNormalMap(RxTexture &texture)
{
	normalMap = texture;
}
void RxMaterial::setHeighMap(RxTexture &texture)
{
	heightMap = texture;
}


/*************************************************************************************************************************/
/*************************************************************************************************************************/
/**************RxMaterialManager****************************************/


/*************************************************************************************************************************/
/*************************************************************************************************************************/
/**************RxBoundingBox****************************************/

RxBoundingBox::RxBoundingBox():
size(0.0f, 0.0f, 0.0f),
position(0.0f, 0.0f, 0.0f)
{


}
bool RxBoundingBox::IsValide()
{
	if( size.x || size.y || size.z)
		return RX_OK;

	return RX_ERROR;
}
void RxBoundingBox::Compute8Points(glm::vec3 *tab)
{
	tab[0] = position + glm::vec3(-size.x*0.5, -size.y*0.5, -size.z*0.5);
	tab[1] = position + glm::vec3(-size.x*0.5, -size.y*0.5,  size.z*0.5);

	tab[2] = position + glm::vec3(-size.x*0.5,  size.y*0.5,  -size.z*0.5);
	tab[3] = position + glm::vec3(-size.x*0.5,  size.y*0.5,   size.z*0.5);

	tab[4] = position + glm::vec3(size.x*0.5,  -size.y*0.5,  -size.z*0.5);
	tab[5] = position + glm::vec3(size.x*0.5,  -size.y*0.5,   size.z*0.5);

	tab[6] = position + glm::vec3(size.x*0.5,  size.y*0.5,  -size.z*0.5);
	tab[7] = position + glm::vec3(size.x*0.5,  size.y*0.5,   size.z*0.5);
}
/*void RxBoundingBox::Draw()
{
	static const unsigned int indices[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		1,5,
		3,7,
		2,6
	};
	glPushMatrix();
		glTranslatef(position.x,position.y,position.z);
		glScalef(size.x,size.y,size.z);
		vbo.Enable();
		glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,indices);
		vbo.Disable();
	glPopMatrix();
}*/

/*************************************************************************************************************************/
/*************************************************************************************************************************/
/**************RxItem3D****************************************/

RxMaterial RxItem3D::defautMaterial;
RxShader *RxItem3D::defautShader = NULL;

RxItem3D::RxItem3D()
{
	type = RX_DEFAULT_ITEM3D; 
	material = defautMaterial;
	shader = defautShader; 
	transform_uniform_name = RX_TRANSFORM_MODEL_UNIFORM_NAME;
}

RxItem3D::~RxItem3D()
{
}

void RxItem3D::Draw(glm::vec3 pos, glm::vec3 dir)
{
	Draw();
}

unsigned int RxItem3D::getType()
{
	return type;
}
bool RxItem3D::IsValide()
{ 
	return true;
}
void RxItem3D::setShader(RxShader *s)
{
	shader = s;
}

RxShader* RxItem3D::getShader()
{
	return shader;
}
void RxItem3D::setUniformNameTransform(const char*name)
{
	if(name)
		transform_uniform_name = name;
}
std::string& RxItem3D::getUniformNameTransform()
{
	
		return transform_uniform_name;
}
void RxItem3D::setMaterial(RxMaterial &m)
{
	material = m;
}

void RxItem3D::setTexture(RxTexture &t)
{
	material.colorMap = t;
}

void RxItem3D::setColor(glm::vec4 &color)
{
	material.color = color;
}
RxMaterial &RxItem3D::getMaterial()
{
	return material;
}

 void RxItem3D::setDefaultMaterial(RxMaterial& defaultm)
{ 
	defautMaterial = defaultm;
}

void RxItem3D::setBoundingBox(RxBoundingBox  box)
{
	boundingBox = box;
}
void RxItem3D::setBoundingBox(glm::vec3 size,glm::vec3 position)
{
	boundingBox.size = size;
	boundingBox.position = position;
}
void RxItem3D::ComputeBoundingBox()
{
	boundingBox.size = glm::vec3(1.0f,1.0f,1.0f);
	boundingBox.position = glm::vec3(0.0f,0.0f,0.0f);
}

void RxItem3D::setMatrix(glm::mat4* projection, glm::mat4* modelview)
{
	this->modelview = modelview;
	this->projection = projection;
}

void RxItem3D::setProjection(glm::mat4* projection)
{
	this->projection = projection;
}
void RxItem3D::setModelView(glm::mat4* modelview)
{
	this->modelview = modelview;
}
void RxItem3D::setTransform(glm::mat4 &transform)
{
	this->transform = transform;
}
glm::mat4& RxItem3D::getTransform()
{
	return transform;
}
glm::mat4 *RxItem3D::getProjection()
{
	return projection;
}
glm::mat4 *RxItem3D::getModelView()
{
	return modelview;
}

RxBoundingBox &RxItem3D::getBoundingBox()
{	
	return boundingBox;
}




