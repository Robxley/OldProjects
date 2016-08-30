//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#pragma once

#ifndef _RXANIMMESH_H_
#define _RXANIMMESH_H_


#include <map>
#include <glm/gtc/quaternion.hpp>


#include "RxMesh.h"
#include "RxLogger.h"
#include "RxSimpleMesh.h"



#define RX_MAX_WEIGHTS_BY_VERTEX 4
#define RX_MAX_BONES 64

#define RX_BONE_MATRIX_UNIFORM_NAME "bonesMatrix"

//structure de stockage des bones
typedef struct _Joint
{

	glm::mat4 globalPose;

	glm::mat4 localPose;

	glm::mat4 offsetPose;

	unsigned int parentID;

	bool isBone;
	
	std::vector<unsigned int> tMeshIDs;

	_Joint():parentID(0),isBone(0){}

} RxJoint;



typedef struct _Bone
{
	//unsigned int ID;
	//glm::vec3 position;

	//glm::mat4 globalPose;
	//glm::mat4 localPose;

	//for the animation
	glm::mat4 offsetPose; //bind pose

	unsigned int jointID; //le joint auquel le bone appartient

	//std::vector<unsigned int> children;


	_Bone():jointID(0){}

} RxBone;


//structure de stokage d'une Frame de l'animation pour un joint
typedef struct _AnimJointKey 
{

	//glm::mat4 mTransformKey;

	typedef struct _Vec3Key
	{
		glm::vec3 value;
		float times;
		_Vec3Key():value(0.0f,0.0f,0.0f),times(0){}
	} RxVec3Key;

	typedef struct _QuatKey
	{
		glm::quat value;
		float times;
		_QuatKey():value(0.0f,0.0f,0.0f,0.0f),times(0){}
	} RxQuatKey;

	std::vector<RxVec3Key> tScaleKeys,tPosKeys;
	std::vector<RxQuatKey> tRotKeys;

	

	unsigned int lastScaleKey, lastPosKey, lastRotKey;
	unsigned int joint;  //le joints affecté par l'animation

	_AnimJointKey():joint(0),lastScaleKey(0),lastPosKey(0),lastRotKey(0){}

}RxAnimJointKey;




typedef struct _Animation
{
	std::string name;
	float duration,ticksPerSecond;
	std::vector<RxAnimJointKey> tAnimKey;

}RxAnimation;




class RxSkeleton
{
	
	public:
		
		std::vector<RxBone> tBones;
		std::vector<RxJoint> tJoints;
		std::vector<RxAnimation> tAnimations;

	
		/**********************************************************************/
		//fonctions d'animation  
		void PlayAnimation(unsigned int ID, float elapseTime);
		
		void AddBone(RxBone & bone);
		void AddJoint(RxJoint & bone);


        void UpdateBindJoints(); 


		//fonction de débugage
		void BluidBones();
		void Draw(RxShader *shader); //Compute doit etre appeler
//
		RxVBO skeletonVBO;
		RxSphere sphere; //utiliser par




};


class RxAnimMesh : public RxMesh
{
	public:
		RxAnimMesh();

		typedef struct _VertexBoneInfo
		{
			//std::vector<unsigned int> tJointIDs;
			unsigned int tBoneIDs[RX_MAX_WEIGHTS_BY_VERTEX];
			float tWeights[RX_MAX_WEIGHTS_BY_VERTEX];
			_VertexBoneInfo()
			{
				for(unsigned int i = 0; i < RX_MAX_WEIGHTS_BY_VERTEX; i++)
				{
					tBoneIDs[i] = 0;
					tWeights[i] = 0;
				}
			}

			void Add(unsigned int id, float w){
				for(unsigned int i = 0; i < RX_MAX_WEIGHTS_BY_VERTEX; i++){
					if(tWeights[i] == 0){
						tWeights[i] = w; tBoneIDs[i] = id; return;
					}
				}
			}

		} RxVertexBoneInfo;


		bool ComputeMesh();
		void DrawAnimSkeleton(RxShader * s);

		void Draw();

	public:

		RxSkeleton &getSkeleton(){return skeleton;}
		std::vector<RxVertexBoneInfo>&  getTabVertexBoneInfo();
		void PlayAnimation(unsigned int ID, float elapseTime);
		void PlayAnimationPerTick(unsigned int ID, unsigned int tick);


		std::vector<glm::mat4> tAnimJointTransforms;
		std::vector<glm::mat4> tAnimJointGlobalTransforms;
		std::vector<glm::mat4> tBonesGlobalTransforms;



	private:


		glm::mat4 KeyTransform(glm::vec3 kscale, glm::quat kquat, glm::vec3 ktranslate);

		void findTransformKey(const RxAnimJointKey & animKey, unsigned int * scaleID, unsigned int * posID, unsigned int * rotID);
		glm::vec3 InterpolationKey(const std::vector<RxAnimJointKey::RxVec3Key> & tabKey, unsigned int keyID);
		glm::quat InterpolationKey(const std::vector<RxAnimJointKey::RxQuatKey> & tabKey, unsigned int keyID);

		void UpdateAnimJoints();
		void GetBoneMatrices(unsigned int idJoint, unsigned int idMesh);
		
		void BluidAnimedBones();

		RxSkeleton skeleton;	
		std::vector<RxVertexBoneInfo> tVertexBoneInfo;
		//std::vector<RxAnimation> tAnimations;


		
		float timeInTicks;



		
		float lastTimeInTick;


		RxVBO animSkeletonVBO;
};










#endif