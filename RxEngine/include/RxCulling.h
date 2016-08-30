//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXCULLING_H_
#define _RXCULLING_H_


#include "RxCamera.h"
#include "RxObject3D.h"
#include "RxSimpleMesh.h"
#include "RxMath3D.h"

//http://www.lighthouse3d.com/tutorials/view-frustum-culling/view-frustums-shape/

#define RX_OUTSIDE_FRUSTRUM 0
#define RX_INTERSECT_FRUSTRUM 1
#define RX_INSIDE_FRUSTRUM 2



//forme en cone du frustrum  (sommet du cone la camera, les near plan et far plan ne sont pas pris en compte)
#define RX_CONE_TEST 0

//prise en compte du near plan et far plan dans le test
#define RX_TRAPEZOID_TEST 1


//CPU calcul inversion matrice modelview et projection des Bounding Box
class RxFrustrumCulling 
{
private:

	enum
	{
		PLANE_LEFT,
		PLANE_RIGHT,
		PLANE_BOTTOM,
		PLANE_TOP,
		PLANE_NEAR,
		PLANE_FAR,
	};


	glm::vec4 planes[6];

	float SignedDistance(glm::vec4 plane, glm::vec3 point);

	int testType;

public:

	RxFrustrumCulling();


	void Update(glm::mat4 &proj, glm::mat4 &modelview, glm::mat4 &model);

	int PointInFrustum(glm::vec3 &point);
	int SphereInFrustum(glm::vec3 &point, float radius);
	int BoxInFrustum(RxBoundingBox &box);

};

//GPU calcul, utilisation de l'extension OCCLUSION QUERY 
class RxOcclusionCulling
{
	RxOcclusionCulling();
	~RxOcclusionCulling();

	int AddRxItem3D(RxItem3D* item);
	int getQueryPixelDrawForItem(RxItem3D* item);
	int getQueryPixelDrawForItem(unsigned int id);
};


#endif