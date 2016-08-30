



#include "RxMath3D.h"




mat4stack::mat4stack(const glm::mat4& m)
{
	tabMat.reserve(PUSHPOP_DEFAULT_SIZE); 
	tabMat.push_back(m);
}
mat4stack::mat4stack(const mat4stack& m)
{
	tabMat.reserve(PUSHPOP_DEFAULT_SIZE); 
	tabMat.push_back(m.tabMat.back());
}
mat4stack::mat4stack()
{
	tabMat.reserve(PUSHPOP_DEFAULT_SIZE); 
	tabMat.push_back(glm::mat4());
}
	
//sauvegarde la matrice
void mat4stack::PushMatrix()
{
	tabMat.push_back(tabMat.back());
}

//restitution de la matrice
void mat4stack::PopMatrix()
{
	if(tabMat.size())
		tabMat.pop_back();
}
void mat4stack::LoadIdentity()
{
	tabMat.back() = glm::mat4();
}

void mat4stack::LoadMatrix(const glm::mat4 m)
{
	tabMat.back()= m;
}
void mat4stack::Reset(const glm::mat4 m)
{
	tabMat.clear();
	tabMat.push_back(m);
}
void mat4stack::Reset()
{
	tabMat.clear();
	tabMat.push_back(glm::mat4());
}

glm::mat4& mat4stack::Top()
{

	return tabMat.back();

}

glm::mat4& mat4stack::MultMatrix(const glm::mat4& m)
{
	return (tabMat.back()*= m);
}


mat4stack& mat4stack::operator = (const glm::mat4& m)
{
	tabMat.back() = m;
	return *this;
}

mat4stack& mat4stack::operator = (const mat4stack& m)
{
	tabMat.back() = m.tabMat.back();
	return *this;
}


int RxPowerOfTwo(int v)
{
    int value = 1;

    while ( value < v ) {
        value <<= 1;
    }
    return value;
}
glm::vec3 orthogonalTo(glm::vec3 v) //trouve un vecteur orthogonal à this
{
		
	//on cherche un verteur perpendiculaire à dir
	if(!v.x)
		return glm::vec3(1.0f,0.0f,0.0f);
	else if(!v.y)
		return glm::vec3(0.0f,1.0f,0.0f);
	else if(!v.z)
		return glm::vec3(0.0f,0.0f,1.0f);
	else
	{
		glm::vec3 tmp(-v.y,v.x,0.0f);
		tmp = glm::normalize(tmp);
		return tmp;
	}
}



void RxPerspective::set(float _fovy, float _aspect, float _zNear, float _zFar)
{
	fovy = _fovy;
	aspect = _aspect;
	zNear = _zNear;
	zFar = _zFar;
	mProjection = glm::perspective(fovy, aspect,zNear,zFar); 
}

void RxPerspective::setFovy(float f)
{
	fovy = f;
	mProjection = glm::perspective(fovy, aspect,zNear,zFar); 
}
void RxPerspective::setAspect(float a)
{
	aspect = a;
	mProjection = glm::perspective(fovy, aspect,zNear,zFar); 
}
void RxPerspective::setZNear(float n)
{
	zNear = n;
	mProjection = glm::perspective(fovy, aspect,zNear,zFar); 
}
void RxPerspective::setZFar(float f)
{
	zFar = f;
	mProjection = glm::perspective(fovy, aspect,zNear,zFar); 
}

float RxPerspective::getFovy(){return fovy;}
float RxPerspective::getAspect(){return aspect;}
float RxPerspective::getZNear(){return zNear;}
float RxPerspective::getZFar(){return zFar;}

glm::mat4& RxPerspective::getProjection()
{
	return mProjection; 
}
	//opérateur de transcriptage
RxPerspective::operator glm::mat4() 
{ 
	return mProjection; 
}
RxPerspective::operator const glm::mat4() const 
{ 
	return mProjection; 
}

RxPerspective::RxPerspective(float _fovy, float _aspect, float _zNear, float _zFar)
{
	set(_fovy,_aspect,_zNear,_zFar);
}



RxMathPlane::RxMathPlane( glm::vec3 &v1,  glm::vec3 &v2,  glm::vec3 &v3) {

	set3Points(v1,v2,v3);
}


RxMathPlane::RxMathPlane() {}

RxMathPlane::~RxMathPlane() {}


void RxMathPlane::set3Points( glm::vec3 &v1,  glm::vec3 &v2,  glm::vec3 &v3) {


	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::normalize(aux2 * aux1);

	point = v2;

	d = -(glm::dot(normal,point));
}

void RxMathPlane::setNormalAndPoint(glm::vec3 &normal, glm::vec3 &point) {

	this->normal = glm::normalize(normal);
	d = -(glm::dot(normal,point));
}

void RxMathPlane::setCoefficients(float a, float b, float c, float d) {

	// set the normal vector
	normal = glm::vec3(a,b,c);
	//compute the lenght of the vector
	float l = glm::length(normal);
	// normalize the vector
	normal *= l;
	// and divide d by th length as well
	this->d = d/l;
}


	

float RxMathPlane::distance(glm::vec3 &p) {

	return (d + glm::dot(normal,p));
}