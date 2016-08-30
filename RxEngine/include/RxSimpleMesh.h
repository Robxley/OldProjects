

/******   Objets 3D   ***************************************/
/*															*/
/*		class RxItem3D;										*/
/*		class RxCube	: RxMesh;							*/
/*		class RxSphere	: RxMesh;							*/
/*		class RxCapsule : RxMesh;							*/
/*		class RxPlane	: RxMesh;							*/
/*		class RxSurface : RxMesh:							*/
/*															*/
/*															*/
/************************************************************/



#pragma once
#ifndef _RXSIMPLEMESH_H_
#define _RXSIMPLEMESH_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <iostream>
#include <vector>

#include "RxMesh.h"


class RxCube:public RxMesh
{
	private:
        glm::vec3 dim;
		void Create();
		
	public:
		
		RxCube(glm::vec3 _dim = glm::vec3(1.0f));
		~RxCube();
		void setSize(glm::vec3 _dim = glm::vec3(1.0f));
		glm::vec3 getSize();
	
		//retourne les dimensions de la boite englobante centrée en 0
		void ComputeBoundingBox();

};


class RxSphere:public RxMesh
{
	private:

		float radius;		//rayon de la sphère
		unsigned int definition;  //definition de la sphère
		glm::vec3 scaleAxis;		//étirement des axes pour faire des ellipses

		glm::vec2 latitudeBounds, longitudeBounds;

		void Create();


	public:
		RxSphere(float _radius = 0.5f, int _def = 12, glm::vec3 _scaleAxis = glm::vec3(1.0f), glm::vec2 _latitudeBounds = glm::vec2(0.0f,180.0f), glm::vec2 _longitudeBounds = glm::vec2(0.0f,360.0f));
		~RxSphere();
		void set(float _radius = 0.5f, int _def = 12, glm::vec3 _scaleAxis = glm::vec3(1.0f), glm::vec2 _latitudeBounds = glm::vec2(0.0f,180.0f), glm::vec2 _longitudeBounds = glm::vec2(0.0f,360.0f));
		float getRadius();
		int getDefinition();
		
		void ComputeBoundingBox();
		
};

class RxCapsule:public RxMesh
{
	private:
		
		float radius;
		float height;
		unsigned int definition;
		void Create();
		void ComputeHalfSphere(	std::vector<RxFace> &tFace, std::vector<glm::vec3> &svertex, std::vector<glm::vec3> &snormal, std::vector<glm::vec2> &stexcoord);

	public:
		RxCapsule( float _radius = 0.5f, float _height = 1.0f, int _definition = 12 );
		~RxCapsule();
		
		void set(float _radius = 0.5f, float _height = 1.0f, int _definition = 12);
		void setRadius(float _radius);
		void setHeight(float _height);
		void setDefinition(int _definition);

		float getRadius();
		float getHeight();
		int getDefinition();

		void ComputeBoundingBox();
};


class RxPlane:public RxMesh
{
	GLdouble glequation[4];
	void Create();
	glm::vec4 equation; 
	glm::vec3 origin;
	float textureloop; //nombre de répétion de la texture sur le plan par défaut RX_INVINFINITY, soit une texture par unité
public:


	RxPlane(glm::vec4 _equation = glm::vec4(0.0f,1.0f,0.0f,0.0f) , float _textureloop = RX_INFINITY);//vec4(a, b ,c ,d)  => (ax + by + cz + d = 0)
	glm::vec3 getNormal();
	glm::vec3 getOriginPoint();
	glm::vec3 getPoint();
	glm::vec4 getEquation(){return equation;}
	GLdouble* getGlEquation(){ return glequation;}

	void ComputeOrigin();

	void set(glm::vec3 _normal, glm::vec3 _origin, float _textureloop = RX_INFINITY);
	void set(glm::vec4 planeEqu, float _textureloop = RX_INFINITY);
	void set(float a = 0, float b = 1, float c = 0, float d = 0, float _textureloop = RX_INFINITY);
	

};



class RxCylinder : public RxMesh
{
		private:
		glm::vec3 size; //radius x, height, radius z
		unsigned int definition;
		void Create();

		std::vector<unsigned int> indices;

	public:
		RxCylinder( glm::vec3 _size = glm::vec3(1.0f), int _definition = 12 );
		~RxCylinder();
		
		void set( glm::vec3 _size = glm::vec3(1.0f) , int _definition = 12);
		void setSize(glm::vec3 _size);
		void setDefinition(int _definition);

		glm::vec3 getSize();
		int getDefinition();

		void ComputeBoundingBox();
};
#endif