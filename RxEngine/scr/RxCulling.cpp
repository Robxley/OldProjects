
#include "RxCulling.h"

RxFrustrumCulling::RxFrustrumCulling():testType(4)
{

}


void RxFrustrumCulling::Update(glm::mat4 &proj, glm::mat4 &modelview, glm::mat4 &model)
{
    glm::mat4 mvp = glm::transpose(proj * modelview * model);
 
    planes[PLANE_LEFT]   = (mvp[3] + mvp[0]);
    planes[PLANE_RIGHT]  = (mvp[3] - mvp[0]);
    planes[PLANE_BOTTOM] = (mvp[3] + mvp[1]);
    planes[PLANE_TOP]    = (mvp[3] - mvp[1]);
    planes[PLANE_NEAR]   = (mvp[3] + mvp[2]);
    planes[PLANE_FAR]    = (mvp[3] - mvp[2]);

	planes[PLANE_LEFT]		/= glm::length2(glm::vec3(planes[PLANE_LEFT]));
	planes[PLANE_RIGHT]		/= glm::length2(glm::vec3(planes[PLANE_RIGHT]));
	planes[PLANE_BOTTOM]	/= glm::length2(glm::vec3(planes[PLANE_BOTTOM]));
	planes[PLANE_TOP]		/= glm::length2(glm::vec3(planes[PLANE_TOP]));
	planes[PLANE_NEAR]		/= glm::length2(glm::vec3(planes[PLANE_NEAR]));
	planes[PLANE_FAR]		/= glm::length2(glm::vec3(planes[PLANE_FAR]));

    //planes[PLANE_LEFT]   = glm::normalize(mvp[3] + mvp[0]);
    //planes[PLANE_RIGHT]  = glm::normalize(mvp[3] - mvp[0]);
    //planes[PLANE_BOTTOM] = glm::normalize(mvp[3] + mvp[1]);
    //planes[PLANE_TOP]    = glm::normalize(mvp[3] - mvp[1]);
    //planes[PLANE_NEAR]   = glm::normalize(mvp[3] + mvp[2]);
    //planes[PLANE_FAR]    = glm::normalize(mvp[3] - mvp[2]);
}

float RxFrustrumCulling::SignedDistance(glm::vec4 plane, glm::vec3 point)
{
	return glm::dot((glm::vec3(plane)),point)+plane.w;
}

int RxFrustrumCulling::PointInFrustum(glm::vec3 &point)
{
	int i;
	for ( i = 0; i < testType; i++)
	{
	    float d;
		d = (planes[i].x*point.x + planes[i].y*point.y + planes[i].z*point.z + planes[i].w);
	    if (d < 0) return RX_OUTSIDE_FRUSTRUM;
		else if (d = 0) return RX_INTERSECT_FRUSTRUM;
	}

	return RX_INSIDE_FRUSTRUM;
}

int RxFrustrumCulling::SphereInFrustum(glm::vec3 &point, float radius)
{
	int i;
	for ( i = 0; i < testType; i++)
	{
	    float d;
		d = (planes[i].x*point.x + planes[i].y*point.y + planes[i].z*point.z + planes[i].w);
	    if (d < -radius) return RX_OUTSIDE_FRUSTRUM;
		else if (d <= radius) return RX_INTERSECT_FRUSTRUM;
	}

	return RX_INSIDE_FRUSTRUM;
}

int RxFrustrumCulling::BoxInFrustum(RxBoundingBox &box)
{
	glm::vec3 cube_points[8];
	box.Compute8Points(cube_points);
	int result = PointInFrustum(cube_points[0]);

	int i;
	for(i=1;i<8;i++)
	{
		if( result != PointInFrustum(cube_points[1]))
			return RX_INTERSECT_FRUSTRUM;
	}
	
	return result;
}