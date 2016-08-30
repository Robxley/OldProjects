/*
 * The MIT License
 *
 * Copyright 2014 Robxley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


 
#ifndef _BH3D_ANIMMESH_H_
#define _BH3D_ANIMMESH_H_

#include <map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLErrorLogger.hpp"
#include "Common.hpp"
#include "VBO.hpp"
#include "Mesh.hpp"
#include "Cube.hpp"

#define BH3D_MAX_WEIGHTS_BY_VERTEX 4
#define BH3D_MAX_BONES 32

#define BH3D_BONE_MATRIX_UNIFORM_NAME "bonesMatrix"


namespace BH3D
{

	//structure de stockage des bones
	typedef struct Joint
	{
		glm::mat4 globalPose;
		glm::mat4 localPose;
		glm::mat4 offsetPose;
		unsigned int parentID = 0;
		bool isBone = 0;
		std::vector<unsigned int> tMeshIDs;
	} Joint;



	typedef struct Bone
	{
		glm::mat4 offsetPose; //bind pose
		unsigned int jointID = 0; //le joint auquel le bone appartient
	} Bone;


	//structure de stokage d'une Frame de l'animation pour un joint
	typedef struct AnimJointKey
	{

		typedef struct Vec3Key
		{
			glm::vec3 value{ 0,0,0 };
			float times = 0;
		} Vec3Key;

		typedef struct QuatKey
		{
			glm::quat value{ 0,0,0,0 };
			float times = 0;
		} QuatKey;

		std::vector<Vec3Key> tScaleKeys, tPosKeys;
		std::vector<QuatKey> tRotKeys;

		unsigned int lastScaleKey = 0, lastPosKey = 0, lastRotKey = 0;
		unsigned int joint = 0;  //le joint affecté par l'animation

	}AnimJointKey;

	//un suite d'animJoinKey donne par magie une animation ;D
	typedef struct Animation
	{
		std::string name;
		float duration = 0, ticksPerSecond = 0;
		std::vector<AnimJointKey> tAnimKey;

	}Animation;

	class Skeleton
	{

	public:
		Skeleton() {}

		std::vector<Bone> tBones;
		std::vector<Joint> tJoints;
		std::vector<Animation> tAnimations;

		void AddBone(Bone & bone);
		void AddJoint(Joint & bone);

		void UpdateBindJoints();

		//debug functions/objects
		void Draw(Shader *shader); //Draw the skeleton with a given shader. BluidBones must be called in first
		void BluidBones();	//Fill the VBO with the bone positions
		VBO skeletonVBO;	//VBO used to draw the bones
		Cube cube;			//Used to draw the joints

	};


	class AnimMesh : public Mesh
	{
	public:
		AnimMesh();

		//vertex bone info send to VBO/Shader to animate a mesh on the GPU
		typedef struct VertexBoneInfo
		{
			unsigned int tBoneIDs[BH3D_MAX_WEIGHTS_BY_VERTEX] = {};
			float tWeights[BH3D_MAX_WEIGHTS_BY_VERTEX] = {};

			void Add(unsigned int id, float w){
				for (int i = 0; i < BH3D_MAX_WEIGHTS_BY_VERTEX; i++){
					if (tWeights[i] == 0){
						tWeights[i] = w; tBoneIDs[i] = id; return;
					}
				}
			}

		} VertexBoneInfo;

		bool ComputeMesh();
		void DrawAnimSkeleton(Shader * s);
		void Draw();

	public:

		Skeleton &getSkeleton(){ return skeleton; }
		std::vector<VertexBoneInfo>&  getTabVertexBoneInfo();
		void PlayAnimation(unsigned int ID, float elapseTime);
		void PlayAnimationPerTick(unsigned int ID, unsigned int tick);

		std::vector<glm::mat4> tAnimJointTransforms;
		std::vector<glm::mat4> tAnimJointGlobalTransforms;
		std::vector<glm::mat4> tBonesGlobalTransforms;

	private:

		glm::mat4 KeyTransform(glm::vec3 kscale, glm::quat kquat, glm::vec3 ktranslate);

		void FindTransformKey(const AnimJointKey & animKey, unsigned int * scaleID, unsigned int * posID, unsigned int * rotID);
		glm::vec3 InterpolationKey(const std::vector<AnimJointKey::Vec3Key> & tabKey, unsigned int keyID);
		glm::quat InterpolationKey(const std::vector<AnimJointKey::QuatKey> & tabKey, unsigned int keyID);

		void UpdateAnimJoints();
		void GetBoneMatrices(unsigned int idJoint, unsigned int idMesh);

		void BluidAnimedBones();

		Skeleton skeleton;
		std::vector<VertexBoneInfo> tVertexBoneInfo;

		float timeInTicks = 0;

		float lastTimeInTick = 0;

		VBO animSkeletonVBO;
	};

	//inline function

	inline void Skeleton::AddBone(Bone &bone)
	{
		tBones.push_back(bone);
	}

	inline void Skeleton::AddJoint(Joint &joint)
	{
		tJoints.push_back(joint);
	}

	inline std::vector<AnimMesh::VertexBoneInfo>&  AnimMesh::getTabVertexBoneInfo()
	{
		return tVertexBoneInfo;
	}

	inline void AnimMesh::GetBoneMatrices(unsigned int idJoint, unsigned int idMesh)
	{
		tBonesGlobalTransforms.resize(skeleton.tJoints.size());

		//glm::mat4 invGlobalJointTransform = glm::inverse(tAnimJointGlobalTransforms[idJoint]);

		for (unsigned int i = 0; i < skeleton.tJoints.size(); i++)
		{
			//tBonesGlobalTransforms[i] = invGlobalJointTransform * tAnimJointGlobalTransforms[i] *skeleton.tJoints[i].offsetPose;
			tBonesGlobalTransforms[i] = tAnimJointGlobalTransforms[i] * skeleton.tJoints[i].offsetPose;
		}
	}
	inline void AnimMesh::UpdateAnimJoints()
	{
		tAnimJointGlobalTransforms.resize(skeleton.tJoints.size());
		tAnimJointGlobalTransforms[0] = tAnimJointTransforms[0];
		for (unsigned int i = 1; i < skeleton.tJoints.size(); i++)
		{
			tAnimJointGlobalTransforms[i] = tAnimJointGlobalTransforms[skeleton.tJoints[i].parentID] * tAnimJointTransforms[i];
		}
	}
}
#endif //
