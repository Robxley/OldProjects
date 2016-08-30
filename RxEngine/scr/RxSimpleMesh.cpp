


#include "RxSimpleMesh.h"





 
 /*************************************************************************************************************************/
/*************************************************************************************************************************/
/**************RxPlane****************************************/

RxPlane::RxPlane(glm::vec4 _equation, float _textureloop) : equation(_equation), textureloop(_textureloop)
{
	
	equation /= glm::l2Norm(glm::vec3(equation)); //équation du plan avec le vecteur normalisé
	type = RX_PLANE_ITEM3D;
	Create();
	ComputeMesh();
}

glm::vec3 RxPlane::getNormal()
{
		return glm::vec3(equation);
}
glm::vec3 RxPlane::getOriginPoint()
{
		return origin;
}


glm::vec3 RxPlane::getPoint()
{
	//valide si et seuelement si l'équation du plan est écrit avec le vecteur normé
	return -glm::vec3(equation)*equation.w;
}

void RxPlane::ComputeOrigin()
{
	origin =  getPoint();
}
void RxPlane::set(glm::vec3 _normal, glm::vec3 _origin, float _textureloop)
{
	textureloop = _textureloop;
	origin = _origin;
	_normal= glm::normalize(_normal);
	equation.x = _normal.x;
	equation.y = _normal.y;
	equation.z = _normal.z;
	equation.w = -glm::dot(_normal,_origin);
	glequation[0] = equation[0];
	glequation[1] = equation[1];
	glequation[2] = equation[2];
	glequation[3] = equation[3];
	Create();
}
void RxPlane::set(glm::vec4 planeEqu, float _textureloop)
{
	textureloop = _textureloop;
	equation = planeEqu;
	equation/= glm::l2Norm(glm::vec3(equation));
	
	glequation[0] = equation[0];
	glequation[1] = equation[1];
	glequation[2] = equation[2];
	glequation[3] = equation[3];

	ComputeOrigin();
	Create();
}
void RxPlane::set(float a , float b , float c , float d, float _textureloop)
{
	textureloop = _textureloop;
	equation.x = a; equation.y = b; equation.z = c; equation.w = d;
	equation/= glm::l2Norm(glm::vec3(equation));
	
	glequation[0] = equation[0];
	glequation[1] = equation[1];
	glequation[2] = equation[2];
	glequation[3] = equation[3];

	ComputeOrigin();
	Create();
}
void RxPlane::Create()
{

	glm::vec3 normal( equation );
	glm::vec3 v,w;

	v = orthogonalTo(normal);

	w = glm::cross(v,normal);
	v = glm::normalize(v);
	w = glm::normalize(w);
	glm::vec3 A,B,C,D;
	glm::vec2 At,Bt,Ct,Dt;

	A = origin+v*RX_INFINITY;
	B = origin+w*RX_INFINITY;
	C = origin-v*RX_INFINITY;
	D = origin-w*RX_INFINITY;

	At = glm::vec2(0.0f,0.0f);						//0
	Bt = glm::vec2(textureloop,0.0f);				//1
	Ct = glm::vec2(textureloop,textureloop);		//2
	Dt = glm::vec2(0.0f,textureloop);				//3

	//methodes glDrawArrays
/*	//triangle 1
	vertex.push_back(A); normals.push_back(normal); texcoords.push_back(At);	//0
	vertex.push_back(C); normals.push_back(normal); texcoords.push_back(Ct);	//2
	vertex.push_back(B); normals.push_back(normal); texcoords.push_back(Bt);	//1
		
	//triangle 2
	vertex.push_back(A); normals.push_back(normal); texcoords.push_back(At);	//0
	vertex.push_back(D); normals.push_back(normal); texcoords.push_back(Dt);	//3
	vertex.push_back(C); normals.push_back(normal); texcoords.push_back(Ct);	//2*/

	//methodes glDrawElements
	tPosition.push_back(A); tNormal.push_back(normal); tTexCoord2.push_back(At);	//0
	tPosition.push_back(B); tNormal.push_back(normal); tTexCoord2.push_back(Bt);	//1
	tPosition.push_back(C); tNormal.push_back(normal); tTexCoord2.push_back(Ct);	//2
	tPosition.push_back(D); tNormal.push_back(normal); tTexCoord2.push_back(Dt);	//3

	tSubMeshes.resize(1);
	tSubMeshes[0].tFace.push_back(RxFace(0,2,1));
	tSubMeshes[0].tFace.push_back(RxFace(0,3,2));

	ComputeMesh();
	FreeArrayVertex();
}



/*************************************************************************************************************************/
/*************************************************************************************************************************/
/*****RxCube**********************************************************/

RxCube::RxCube(glm::vec3 _dim)
{
	dim = _dim;
	type = RX_CUBE_ITEM3D;
	ComputeBoundingBox();
	Create();

}
RxCube::~RxCube()
{

}
void RxCube::Create()
{

	GLfloat vvertex[] =	{-0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,		//Front Face
							 -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,	  0.5f, 0.5f,-0.5f,	  0.5f,-0.5f,-0.5f,		// Back Face
							 -0.5f, 0.5f,-0.5f,	 -0.5f, 0.5f, 0.5f,	  0.5f, 0.5f, 0.5f,	  0.5f, 0.5f,-0.5f,		// Top Face	
							 -0.5f,-0.5f,-0.5f,	  0.5f,-0.5f,-0.5f,	  0.5f,-0.5f, 0.5f,	 -0.5f,-0.5f, 0.5f,		// Bottom Face
							  0.5f,-0.5f,-0.5f,	  0.5f, 0.5f,-0.5f,	  0.5f, 0.5f, 0.5f,	  0.5f,-0.5f, 0.5f,		// Right face	
							 -0.5f,-0.5f,-0.5f,	 -0.5f,-0.5f, 0.5f,	 -0.5f, 0.5f, 0.5f,	 -0.5f, 0.5f,-0.5f};	// Left Face

	GLfloat nvertex[] = {-1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
							 1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  
							 0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,  
							 0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,   0.0f,-1.0f, 0.0f,
							 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
							 0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f,-1.0f};

	GLfloat tvertex[] = {0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
							 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f,
							 0.0f, 1.0f,   0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,
							 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f,   1.0f, 0.0f,
							 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,   0.0f, 0.0f,
							 0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f}; 


	GLuint indices[] = { 0, 1, 2,  0, 2, 3,
						4, 5, 6,  4, 6, 7,
						8, 9,10,  8,10,11,
						12,13,14, 12,14,15,
						16,17,18, 16,18,19,
						20,21,22, 20,22,23};

	int i;
	for(i = 0; i < 24; i++)
	{
		vvertex[i*3]=vvertex[i*3]*dim.x;
		vvertex[i*3+1]=vvertex[i*3+1]*dim.y;
		vvertex[i*3+2]=vvertex[i*3+2]*dim.z;
	}


	AddSubMesh(12,indices,24,vvertex,nvertex,tvertex);
	ComputeMesh();
	FreeArrayVertex();
}
void RxCube::setSize(glm::vec3 _dim)
{
	Destroy();
	dim = _dim;
	Create();
}
glm::vec3 RxCube::getSize()
{
	return dim;
}
//returne les dimensions de la boite englobante centrée en 0
void RxCube::ComputeBoundingBox()
{
	boundingBox.size = dim;
	boundingBox.position = glm::vec3(0.0f,0.0f,0.0f);
}


/*************************************************************************************************************************/
/*************************************************************************************************************************/
/*********Sphere**************************/
RxSphere::RxSphere(float _radius, int _def, glm::vec3 _scaleAxis, glm::vec2 _latitudeBounds, glm::vec2 _longitudeBounds)
	: radius(_radius),definition(_def), scaleAxis(_scaleAxis), 
	latitudeBounds(_latitudeBounds), longitudeBounds(_longitudeBounds)
{
	type = RX_SPHERE_ITEM3D;
	Create();
	ComputeBoundingBox();
}
RxSphere::~RxSphere()
{

}
void RxSphere::set(float _radius, int _def, glm::vec3 _scaleAxis, glm::vec2 _latitudeBounds, glm::vec2 _longitudeBounds)
{
	Destroy();
	radius = _radius;
	definition = _def;
	scaleAxis = _scaleAxis; 
	latitudeBounds = _latitudeBounds; 
	longitudeBounds = _longitudeBounds;
	Create();
	ComputeBoundingBox();
}
void RxSphere::ComputeBoundingBox(){
	boundingBox.size = glm::vec3(radius,radius,radius);
	boundingBox.position = glm::vec3();
}
float RxSphere::getRadius(){return radius;}
int RxSphere::getDefinition(){return definition;}

void RxSphere::Create()
{

	float a;		    
	float b;

	glm::vec3 tmp3;
	glm::vec2 tmp2;


	for( b = latitudeBounds[0]; b <= latitudeBounds[1] - definition; b+=definition)
	{
		for( a = longitudeBounds[0]; a <= longitudeBounds[1] - definition; a+=definition)
		{   
			
			tmp3.x = radius * sin((a) / 180 * RX_PI) * sin((b) / 180 * RX_PI) * scaleAxis.x;
			tmp3.z = radius * cos((a) / 180 * RX_PI) * sin((b) / 180 * RX_PI) * scaleAxis.y;     
			tmp3.y = radius * cos((b) / 180 * RX_PI) * scaleAxis.z;         
			tmp2.y = (2 * b) / 360;           
			tmp2.x =(a) / 360;
			tPosition.push_back(tmp3);
			tTexCoord2.push_back(tmp2);
			tmp3=glm::normalize(tmp3);
			tNormal.push_back(tmp3);
			
		
			tmp3.x = radius * sin((a) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI)* scaleAxis.x;    
			tmp3.z = radius * cos((a) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI)* scaleAxis.y;        
			tmp3.y = radius * cos((b + definition) / 180 * RX_PI)* scaleAxis.z;        
			tmp2 .y = (2 * (b + definition)) / 360;         
			tmp2.x = (a) / 360;  
			tPosition.push_back(tmp3);
			tTexCoord2.push_back(tmp2);
			tmp3=glm::normalize(tmp3);
			tNormal.push_back(tmp3);
			       
			tmp3.x = radius * sin((a + definition) / 180 * RX_PI) * sin((b) / 180 * RX_PI)* scaleAxis.x;          
			tmp3.z = radius * cos((a + definition) / 180 * RX_PI) * sin((b) / 180 * RX_PI)* scaleAxis.y;
			tmp3.y = radius * cos((b) / 180 * RX_PI)* scaleAxis.z;
			tmp2.y = (2 * b) / 360;
			tmp2.x = (a + definition) / 360;
			tPosition.push_back(tmp3);
			tTexCoord2.push_back(tmp2);	
			tmp3=glm::normalize(tmp3);
			tNormal.push_back(tmp3);
		   

			tmp3.x = radius * sin((a + definition) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI)* scaleAxis.x;          
			tmp3.z = radius * cos((a + definition) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI)* scaleAxis.y;       
			tmp3.y = radius * cos((b + definition) / 180 * RX_PI) * scaleAxis.z;  
			tmp2.y = (2 * (b + definition)) / 360;        
			tmp2.x = (a + definition) / 360;         
			tPosition.push_back(tmp3);
			tTexCoord2.push_back(tmp2);
			tmp3=glm::normalize(tmp3);
			tNormal.push_back(tmp3);
		}
	}
     
	tSubMeshes.resize(1);

	unsigned int i;
	RxFace f;
	for( i = 0 ; i < tPosition.size()-2; i++)
	{
		if( !(i%2) ) //paire
			f.set(i,i+1,i+2);
		else		//impaire
			f.set(i,i+2,i+1);

		tSubMeshes[0].tFace.push_back(f);
	}

	ComputeMesh();
	FreeArrayVertex();
}
/*
void RxSphere::Draw()
{
	vbo.Enable();
	glDrawArrays(GL_TRIANGLE_STRIP,0,nb_triangles);
	vbo.Disable();
}*/
/*************************************************************************************************************************/
/*************************************************************************************************************************/
/*********** Capsule ***************************/
RxCapsule::RxCapsule(float _radius, float _height, int _definition ) 
	: height(_height),//correspondance à PhysX
	radius(_radius), definition (_definition)
{

	type = RX_CAPSULE_ITEM3D;
	Create();
	ComputeBoundingBox();
	ComputeMesh();
}
RxCapsule::~RxCapsule()
{
	
}

void RxCapsule::ComputeHalfSphere(	std::vector<RxFace> &tFace, std::vector<glm::vec3> &vertex, std::vector<glm::vec3> &normal, std::vector<glm::vec2> &texCoord)
{
	float a;		    
	float b;

	glm::vec3 tmp3;
	glm::vec2 tmp2;

	for( b = 0; b <= 180 - definition; b+=definition)
	{
		for( a = 0; a <= 360 - definition; a+=definition)
		{   
			
			tmp3.x = radius * sin((a) / 180 * RX_PI) * sin((b) / 180 * RX_PI);
			tmp3.z = radius * cos((a) / 180 * RX_PI) * sin((b) / 180 * RX_PI);     
			tmp3.y = radius * cos((b) / 180 * RX_PI);         
			tmp2.y = (2 * b) / 360;           
			tmp2.x =(a) / 360;
			vertex.push_back(tmp3);
			texCoord.push_back(tmp2);
			tmp3=glm::normalize(tmp3);
			normal.push_back(tmp3);
			
		
			tmp3.x = radius * sin((a) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI);    
			tmp3.z = radius * cos((a) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI);        
			tmp3.y = radius * cos((b + definition) / 180 * RX_PI);        
			tmp2 .y = (2 * (b + definition)) / 360;         
			tmp2.x = (a) / 360;  
			vertex.push_back(tmp3);
			texCoord.push_back(tmp2);
			tmp3=glm::normalize(tmp3);
			normal.push_back(tmp3);
			       
			tmp3.x = radius * sin((a + definition) / 180 * RX_PI) * sin((b) / 180 * RX_PI);          
			tmp3.z = radius * cos((a + definition) / 180 * RX_PI) * sin((b) / 180 * RX_PI);
			tmp3.y = radius * cos((b) / 180 * RX_PI);
			tmp2.y = (2 * b) / 360;
			tmp2.x = (a + definition) / 360;
			vertex.push_back(tmp3);
			texCoord.push_back(tmp2);	
			tmp3=glm::normalize(tmp3);
			normal.push_back(tmp3);
		   

			tmp3.x = radius * sin((a + definition) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI);          
			tmp3.z = radius * cos((a + definition) / 180 * RX_PI) * sin((b + definition) / 180 * RX_PI);       
			tmp3.y = radius * cos((b + definition) / 180 * RX_PI);  
			tmp2.y = (2 * (b + definition)) / 360;        
			tmp2.x = (a + definition) / 360;         
			vertex.push_back(tmp3);
			texCoord.push_back(tmp2);
			tmp3=glm::normalize(tmp3);
			normal.push_back(tmp3);
		}
	}
          
	unsigned int i;
	RxFace f;
	for( i = 0 ; i < vertex.size()-2; i++)
	{
		if( !(i%2) ) //paire
			f.set(i,i+1,i+2);
		else		//impaire
			f.set(i,i+2,i+1);

		tFace.push_back(f);
	}
}
void RxCapsule::Create()
{

	//création de la demi sphere nécessaire à faire le haut et le bas du la capsule
	std::vector<glm::vec3> svertex;
	std::vector<glm::vec3> snormal;
	std::vector<glm::vec2> stexcoord;
	std::vector<RxFace> sface;

	ComputeHalfSphere(sface,svertex,snormal,stexcoord);


	//demi sphere supérieur
	glm::vec3 poshalfSphere(0.0f,height*0.25f,0.0f);
	unsigned int i;
	for(i = 0; i < svertex.size(); i++) //position supérieur
		svertex[i]+=poshalfSphere;

	AddSubMesh(sface.size(), sface[0].id,
			 svertex.size(), &svertex[0].x,
			 &snormal[0].x,
			 &stexcoord[0].x);


	//demi sphere inférieur
	for(i = 0; i < svertex.size(); i++)
	{
		//on la reposition au centre
		//svertex[i]-=poshalfSphere; 

		//on retourne la demi sphère, on retourne aussi l'axe des z ou y pr le CULL FACE
		svertex[i].y *= -1;		
		svertex[i].z *= -1;
		snormal[i].y *= -1;		
		snormal[i].z *= -1;

		//on la position en bas
		//svertex[i]-=poshalfSphere; 
	}


	AddSubMesh(sface.size(), sface[0].id,
			 svertex.size(), &svertex[0].x,
			 &snormal[0].x,
			 &stexcoord[0].x);
	

	//creation du tube
	float a;		    
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec2> texcoord;
	std::vector<glm::vec3> normal;
	glm::vec3 tmp3;
	glm::vec2 tmp2;

	
	
	for( a = 0; a <= 360 - definition; a+=definition)
	{   
			
		tmp3.x = radius * sin((a) / 180 * RX_PI);
		tmp3.z = radius * cos((a) / 180 * RX_PI);     
		tmp3.y = height*0.25f;         
		tmp2.y = 0;           
		tmp2.x =(a) / 360;
		vertex.push_back(tmp3);
		texcoord.push_back(tmp2);
		tmp3 = glm::normalize(tmp3);
		normal.push_back(tmp3);
			
		
		tmp3.x = radius * sin((a) / 180 * RX_PI);    
		tmp3.z = radius * cos((a) / 180 * RX_PI);        
		tmp3.y = -height*0.25f;        
		tmp2 .y = 1;         
		tmp2.x = (a) / 360;  
		vertex.push_back(tmp3);
		texcoord.push_back(tmp2);
		tmp3 = glm::normalize(tmp3);
		normal.push_back(tmp3);
			       
		tmp3.x = radius * sin((a + definition) / 180 * RX_PI);          
		tmp3.z = radius * cos((a + definition) / 180 * RX_PI);
		tmp3.y = height*0.25f;
		tmp2.y = 0;
		tmp2.x = (a + definition) / 360;
		vertex.push_back(tmp3);
		texcoord.push_back(tmp2);	
		tmp3 = glm::normalize(tmp3);
		normal.push_back(tmp3);
		   

		tmp3.x = radius * sin((a + definition) / 180 * RX_PI);          
		tmp3.z = radius * cos((a + definition) / 180 * RX_PI);       
		tmp3.y = -height*0.25f;  
		tmp2.y = 1;        
		tmp2.x = (a + definition) / 360;         
		vertex.push_back(tmp3);
		texcoord.push_back(tmp2);
		tmp3 = glm::normalize(tmp3);
		normal.push_back(tmp3);
	}
	 
	std::vector<RxFace> tFace;
	RxFace f;
	for( i = 0 ; i < vertex.size()-2; i++)
	{
		if( !(i%2) ) //paire
			f.set(i,i+1,i+2);
		else		//impaire
			f.set(i,i+2,i+1);

		tFace.push_back(f);
	}
	
	AddSubMesh(tFace.size(), tFace[0].id,
			vertex.size(), &vertex[0].x,
			&normal[0].x,
			&texcoord[0].x);

	ComputeMesh();
	FreeArrayVertex();
}
void RxCapsule::ComputeBoundingBox()
{
	boundingBox.position = glm::vec3(0.0f,0.0f,0.0f);
	boundingBox.size = glm::vec3(radius,height+2*radius,radius);
}


void RxCapsule::set(float _radius, float _height, int _definition)
{
	radius = _radius;
	height = 2*_height;
	definition = _definition;
	Create();
	ComputeBoundingBox();
}
void RxCapsule::setRadius(float _radius)
{
	radius = _radius;
	Create();
	ComputeBoundingBox();
}
void RxCapsule::setHeight(float _height)
{
	height = 2*_height;
	Create();
	ComputeBoundingBox();
}
void RxCapsule::setDefinition(int _definition)
{
	definition = _definition;
	Create();
	ComputeBoundingBox();
}

float RxCapsule::getRadius()
{
	return radius;
}
float RxCapsule::getHeight()
{
	return height/2;
}
int RxCapsule::getDefinition()
{
	return definition;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RxCylinder::RxCylinder( glm::vec3 _size, int _definition ):
	size(_size), definition(_definition)
{
	type = RX_CYLINDER_ITEM3D;
	Create();
	ComputeBoundingBox();
	ComputeMesh();
}
RxCylinder::~RxCylinder()
{
	vbo.Destroy();
}
void RxCylinder::Create()
{

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;


	vertices.reserve(definition*4);
	normals.reserve(definition*4);
	texcoords.reserve(definition*4);

	float x,z;

    float step = 0;
	unsigned int n = 0;

	//formule de Ramanujan approximation du contour d'une ellipse
	float length = RX_PI*( 3*(size.x+size.z)-sqrt((3*size.x+size.z)*(size.x+3*size.z)));
	length/=definition;
	//le tube
	for(n = 0; n <= definition; n++)
	{
		x = cos(step)* size.x;
		z = sin(step)* size.z;
		glm::vec3 tn = glm::vec3(x,0,z);
		tn = glm::normalize(tn);

		//tube
		vertices.push_back(glm::vec3(x,-0.5f*size.y,z));   //0
		normals.push_back(tn);
		texcoords.push_back(glm::vec2(length*n,0));

		vertices.push_back(glm::vec3(x,0.5f*size.y,z) );  //1
		normals.push_back(tn);
		texcoords.push_back(glm::vec2(length*n,1.0f));

		//disque suppérieur
		vertices.push_back(glm::vec3(x,0.5f*size.y,z));   //2
		normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
		texcoords.push_back(glm::vec2(x,z));

		//disque inférieur
		vertices.push_back(glm::vec3(x,-0.5f*size.y,z));   //3
		normals.push_back(glm::vec3(0.0f,-1.0f,0.0f));
		texcoords.push_back(glm::vec2(x,z));

		step+=(RX_2PI/definition);


		
	}



	//points centraux des disques inférieurs et supérieur
	vertices.push_back(glm::vec3(0.0f, 0.5f*size.y, 0.0f)); //sup
	normals.push_back(glm::vec3(0.0f,1.0f,0.0f));
	texcoords.push_back(glm::vec2(0.0f,0.0f));
	
	vertices.push_back(glm::vec3(0.0f, -0.5f*size.y, 0.0f)); //inf
	normals.push_back(glm::vec3(0.0f,-1.0f,0.0f));
	texcoords.push_back(glm::vec2(0.0f,0.0f));

	indices.clear();
	indices.reserve(definition*4);

	unsigned int last, blast;
	last  = vertices.size()-1;
	blast = vertices.size()-2;

	for(n = 0; n < definition; n++)
	{
			//faces du tube
			indices.push_back(n*4);
			indices.push_back(n*4+1);
			indices.push_back((n+1)*4);

			indices.push_back(n*4+1);
			indices.push_back((n+1)*4+1);
			indices.push_back((n+1)*4);

			//faces du disque sup
			indices.push_back(n*4+2);
			indices.push_back(blast);
			indices.push_back((n+1)*4+2);


			//faces du disque inf
			indices.push_back((n+1)*4+3);
			indices.push_back(last);
			indices.push_back(n*4+3);

	}
	
	AddSubMesh(indices.size()/3,&indices[0], vertices.size(),&vertices[0].x, &normals[0].x,&texcoords[0].x);
	ComputeMesh();
	FreeArrayVertex();
}

void RxCylinder::set(glm::vec3 _size, int _definition)
{
	Destroy();
	size = _size;
	definition=_definition;
	Create();
}

void RxCylinder::setDefinition(int _definition)
{
	Destroy();
	definition = _definition;
	Create();
}

glm::vec3 RxCylinder::getSize()
{
	return size;
}

int RxCylinder::getDefinition()
{
	return definition;
}

void RxCylinder::ComputeBoundingBox()
{
	boundingBox.size = size;
	boundingBox.position= glm::vec3(0.0f,0.0f,0.0f);
}