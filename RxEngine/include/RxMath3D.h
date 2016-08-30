
//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#pragma once

#ifndef _RXMATH3D_H_
#define _RXMATH3D_H_


#include <vector>
#include <glm/glm.hpp>
//#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>


#define RX_EPSILON			0.000001f
#define RX_PI				3.141592653589793238462643383279f
#define RX_PIDIV2			1.570796326794896619231321691639f		// PI / 2
#define RX_2PI				6.283185307179586476925286766559f		// 2 * PI
#define RX_PI2				9.869604401089358618834490999876f		// PI au carre
#define RX_PIDIV180			0.01745329251994329576923690768488f		// PI / 180
#define RX_180DIVPI			57.295779513082320876798154814105f		// 180 / PI

#define DegToRad(a)	(a)*=RX_PIDIV180
#define RadToDeg(a)	(a)*=RX_180DIVPI
#define RADIANS(a)	((a)*RX_PIDIV180)
#define DEGRES(a)	((a)*RX_180DIVPI)


const float RX_INV_RAND_MAX = 1.0 / (RAND_MAX + 1);
inline float random(float max=1.0) { return max * rand() * RX_INV_RAND_MAX; }
inline float random(float min, float max) { return min + (max - min) * RX_INV_RAND_MAX * rand(); }
inline int random(int max=RAND_MAX) { return rand()%(max+1); }

//calcule le sinus angle d'ouverture /2 selon x à partir de l'angle d'ouverture selon y et du ratio w/h.
inline float sinflovx2(float fovy, float ratio)
{
	return ratio*sin(DegToRad(fovy)/2); //sin(fovx/2) = r * sin(fovy/2)
}
inline float getfovx(float fovy, float ratio)
{
	return asin(ratio*sin(DegToRad(fovy)/2))*2; //sin(fovx/2) = r * sin(fovy/2)
}


#define PUSHPOP_DEFAULT_SIZE 50

//surcharge de fonction glm


int RxPowerOfTwo(int v);
glm::vec3 orthogonalTo(glm::vec3 v);

class mat4stack
{

public:
	mat4stack(const glm::mat4& m);
	mat4stack(const mat4stack& m);
	mat4stack();
	
	//sauvegarde la matrice
	void PushMatrix(); //glPushMatrix

	//restitution de la matrice
	void PopMatrix(); //glPopMatrix

	//charge la matrice identitée sur la matrice active
	void LoadIdentity(); //glLoadIdentity

	//charge une matrice
	void LoadMatrix(const glm::mat4 m); //glLoadMatrix

	//multiplication
	glm::mat4& MultMatrix(const glm::mat4& m); //glMultMatrix

	//retour la matrice sauvegardée en haut de pile (matrice active)
	glm::mat4& Top(); 

	mat4stack& operator = (const glm::mat4& m);
	mat4stack& operator = (const mat4stack& m);

	void Reset(const glm::mat4 m);
	void Reset();

	std::vector<glm::mat4> tabMat;
	std::vector<glm::mat4>::iterator it;

};



/************** Conversion ************************/

typedef struct _Rx2DPerlinNoise
{
	int octaveCount;
	double frequency;
	double persistence;
	int seed;

	glm::vec2 leftLowerCorner, rightUpperCorner;

	_Rx2DPerlinNoise():
		octaveCount(6),
		frequency(1.0),
		persistence(0.5),
		seed(0),
		leftLowerCorner(2.0f,1.0f),
		rightUpperCorner(6.0f,5.0f)
	{}

} Rx2DPerlinNoise;



class RxMathPlane
{
	public:

	glm::vec3 normal,point;
	float d;


	RxMathPlane( glm::vec3 &v1,  glm::vec3 &v2,  glm::vec3 &v3);
	RxMathPlane(void);
	~RxMathPlane();

	void set3Points( glm::vec3 &v1,  glm::vec3 &v2,  glm::vec3 &v3);
	void setNormalAndPoint(glm::vec3 &normal, glm::vec3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(glm::vec3 &p);
};

class RxPerspective
{
	private:
		float fovy;
		float aspect;
		float zNear,zFar;
		glm::mat4 mProjection;

	public:

    RxPerspective(float _fovy = 70.0, float _aspect = 0.75, float _zNear = 1.0, float _zFar = 10.0);

	void set(float _fovy, float _aspect, float _zNear, float _zFar);
	void setFovy(float f);
	void setAspect(float a);
	void setZNear(float n);
	void setZFar(float f);

	float getFovy();
	float getAspect();
	float getZNear();
	float getZFar();


	glm::mat4 &getProjection();

		//opérateur de transcriptage
	operator glm::mat4(); 
	operator const glm::mat4() const;

} ;



#endif 

