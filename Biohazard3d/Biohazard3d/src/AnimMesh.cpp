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

#include "AnimMesh.hpp"
#include <algorithm>

namespace BH3D
{

	void Skeleton::UpdateBindJoints()
	{
		if (tJoints.empty()) return;
		tJoints[0].globalPose = tJoints[0].localPose;
		std::for_each(tJoints.begin() + 1, tJoints.end(), 
			[&](Joint &v) {v.globalPose = tJoints[v.parentID].globalPose *  v.localPose;
		});
	}


	void Skeleton::BluidBones()
	{
		BH3D_ASSERT(tJoints.size())
		if (tJoints.empty()) return;

		cube.setSize(glm::vec3(0.2f));
		UpdateBindJoints();

		std::vector<glm::vec3> tVertex, tColor;
		tVertex.reserve(tJoints.size() * 2);
		tColor.reserve(tJoints.size() * 2);

		const glm::vec3 point = glm::vec3(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		//start to 1. 0 is the root joint
		for (unsigned int i = 1; i < tJoints.size(); i++)
		{
			tVertex.push_back(point);
			tVertex.push_back(glm::vec3(tJoints[i].localPose *glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

			tColor.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			tColor.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		
		skeletonVBO.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, tVertex);
		skeletonVBO.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, tColor);
		skeletonVBO.Create();

	}

	void Skeleton::Draw(Shader *shader)
	{
		BH3D_ASSERT(tJoints.size())

		if (tJoints.empty()) return;

		glBindTexture(GL_TEXTURE_2D, 0);

		for (const auto & join : tJoints)
		{
			shader->SendTransform(join.globalPose);
			cube.Draw();
		}

		glLineWidth(3.0f);
		skeletonVBO.Enable();
		for (unsigned int i = 1; i < tJoints.size(); i++)
		{
			shader->SendTransform(tJoints[tJoints[i].parentID].globalPose);
			glDrawArrays(GL_LINES, (i - 1) * 2, 2);
		}
		skeletonVBO.Disable();
		glLineWidth(1.0f);
	}



	/*********************************************************/
	/*********AnimMesh**********************************/
	/*********************************************************/
	AnimMesh::AnimMesh() : lastTimeInTick(0)
	{

	}

	bool AnimMesh::ComputeMesh()
	{

		if (tSubMeshes.empty())
		{
			BH3D_LOGGERERROR("No vertices");
			return BH3D_ERROR;
		}

		vbo.Destroy();

		ComputeBoundingBox();

		//construction du vbo
		vbo.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, tPosition);

		if (tNormal.size())
			vbo.AddArrayBufferData(BH3D_NORMAL_ATTRIB_INDEX, tNormal);

		if (tTexCoord2.size())
			vbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tTexCoord2);
		else if (tTexCoord3.size())
			vbo.AddArrayBufferData(BH3D_COORD0_ATTRIB_INDEX, tTexCoord3);

		if (tColor3.size())
			vbo.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, tColor3);
		else if (tColor4.size())
			vbo.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, tColor4);

		if (tTangent.size())
			vbo.AddArrayBufferData(BH3D_DATA0_ATTRIB_INDEX, tTangent);

		if (tVertexBoneInfo.size())
		{
			unsigned int attrib[] = { BH3D_DATA1_ATTRIB_INDEX, BH3D_DATA2_ATTRIB_INDEX };
			unsigned int attribType[] = { GL_UNSIGNED_INT, GL_FLOAT };
			unsigned int attribSize[] = { BH3D_MAX_WEIGHTS_BY_VERTEX, BH3D_MAX_WEIGHTS_BY_VERTEX };
			unsigned int attribOffsetStart[] = { 0, BH3D_MAX_WEIGHTS_BY_VERTEX*sizeof(unsigned int) };
			unsigned int stride = BH3D_MAX_WEIGHTS_BY_VERTEX*(sizeof(float) + sizeof(unsigned int));
			std::size_t size = tVertexBoneInfo.size()*stride;

#ifndef BH3D_USE_OPENGLES2
			unsigned int attribInType[] =  { BH3D_ATTRIB_IVEC, BH3D_ATTRIB_VEC };
			vbo.AddStructArrayBufferData(2, attrib, attribType, attribSize, attribOffsetStart, stride, size, &(tVertexBoneInfo[0].tBoneIDs[0]), attribInType);
#else
			vbo.AddStructArrayBufferData(2, attrib, attribType, attribSize, attribOffsetStart, stride, size, &(tVertexBoneInfo[0].tBoneIDs[0]));
#endif

		}

		vbo.AddElementBufferData(tFace[0].id, tFace.size() * 3);

		skeleton.UpdateBindJoints();

		if (!vbo.Create(GL_STATIC_DRAW))
		{
			return BH3D_ERROR;
			BH3D_LOGGERERROR("Can't Create the VBO");
		}

		return (computed = BH3D_OK);
	}

	glm::mat4 AnimMesh::KeyTransform(glm::vec3 kscale, glm::quat kquat, glm::vec3 ktranslate)
	{
		glm::mat4 S = glm::scale(glm::mat4(1), kscale);
		glm::mat4 R = glm::mat4_cast(kquat);
		glm::mat4 T = glm::translate(glm::mat4(1), ktranslate);

		return (T*R*S);
	}

	void AnimMesh::PlayAnimationPerTick(unsigned int ID, unsigned int tick)
	{
		if (ID > skeleton.tAnimations.size() || skeleton.tAnimations.empty())
		{
			BH3D_LOGGERERROR("animation empty");
			return;
		}
		float timeInSeconds = float(tick);
		if (skeleton.tAnimations[ID].ticksPerSecond > 0)
			timeInSeconds /= skeleton.tAnimations[ID].ticksPerSecond;

		PlayAnimation(ID, timeInSeconds);
	}

	void AnimMesh::PlayAnimation(unsigned int ID, float timeInSeconds)
	{

		if (ID > skeleton.tAnimations.size() || skeleton.tAnimations.empty())
		{
			BH3D_LOGGERERROR("animation empty");
			return;
		}

		if (skeleton.tJoints.empty())
		{
			BH3D_LOGGERERROR("skeleton empty");
			return;
		}

		Animation & animation = skeleton.tAnimations[ID];

		timeInTicks = timeInSeconds * animation.ticksPerSecond;
		if (animation.duration > 0.0)
			timeInTicks = fmod(timeInTicks, animation.duration);

		unsigned int sKey, pKey, rKey;
		glm::vec3 posKey, scaleKey;
		glm::quat rotKey;

		//reset des matrices d'animation
		tAnimJointTransforms.resize(skeleton.tJoints.size());

		//permet de savoir quel joint est animé
		std::vector<bool> JointIsAnimated(skeleton.tJoints.size(), 0);

		//calcul des matrices d'animation
		for (auto & animkey : animation.tAnimKey)
		{
			FindTransformKey(animkey, &sKey, &pKey, &rKey);  //recherche la frameKey
																		   //calcul la matrice d'interpolation
			scaleKey = InterpolationKey(animkey.tScaleKeys, sKey);
			posKey = InterpolationKey(animkey.tPosKeys, pKey);
			rotKey = InterpolationKey(animkey.tRotKeys, rKey);

			unsigned int jointID = animkey.joint;
			tAnimJointTransforms[jointID] = KeyTransform(scaleKey, rotKey, posKey);//sauvegarde la matrice d'animation
			JointIsAnimated[jointID] = 1; //joint modifié

			animkey.lastPosKey = pKey;
			animkey.lastScaleKey = sKey;
			animkey.lastRotKey = rKey;

		}

		//si le joint n'est pas animé alors la matrice d'animation correspond à la matrice locale du joint
		for (unsigned int i = 0; i < skeleton.tJoints.size(); i++)
		{
			if (!JointIsAnimated[i])
				tAnimJointTransforms[i] = skeleton.tJoints[i].localPose;
		}

		lastTimeInTick = timeInTicks;

		UpdateAnimJoints();

	}

	void AnimMesh::FindTransformKey(const AnimJointKey & animKey, unsigned int * scaleID, unsigned int * posID, unsigned int * rotID)
	{

		unsigned int i;
		//cherche le plus proche
		(*scaleID) = 0;
		(*posID) = 0;
		(*rotID) = 0;

		//évite de commencer à 0 à chaque fois
		i = (timeInTicks >= lastTimeInTick) ? animKey.lastScaleKey : 0;
		const auto & tScaleKey = animKey.tScaleKeys;
		for (; i < tScaleKey.size(); i++)
		{
			if (tScaleKey[i].times >= timeInTicks)
			{
				(*scaleID) = i;
				break;
			}
		}

		i = (timeInTicks >= lastTimeInTick) ? animKey.lastRotKey : 0;
		const auto & tRotKeys = animKey.tRotKeys;
		for (; i < tRotKeys.size(); i++)
		{
			if (tRotKeys[i].times >= timeInTicks)
			{
				(*rotID) = i;
				break;
			}
		}

		i = (timeInTicks >= lastTimeInTick) ? animKey.lastPosKey : 0;
		const auto & tPosKeys = animKey.tPosKeys;
		for (; i < tPosKeys.size(); i++)
		{
			if (tPosKeys[i].times >= timeInTicks)
			{
				(*posID) = i;
				break;
			}
		}

	}

	glm::vec3 AnimMesh::InterpolationKey(const std::vector<AnimJointKey::Vec3Key> & tabKey, unsigned int keyID)
	{
		if (tabKey.empty())
			return glm::vec3(0.0f);
		if (timeInTicks == tabKey[keyID].times) return tabKey[keyID].value;
		if (tabKey.size() == 1 || keyID == 0) return tabKey[0].value;

		glm::vec3 a, b;
		float timea, timeb, deltaTime;
		a = tabKey[keyID - 1].value;
		b = tabKey[keyID].value;
		timea = tabKey[keyID - 1].times;
		timeb = tabKey[keyID].times;
		deltaTime = (timeInTicks - timea) / (timeb - timea);
		return glm::mix(a, b, deltaTime);
	}


	glm::quat AnimMesh::InterpolationKey(const std::vector<AnimJointKey::QuatKey> & tabKey, unsigned int keyID)
	{
		if (tabKey.empty()) return glm::quat();
		if (timeInTicks == tabKey[keyID].times) return tabKey[keyID].value;
		if (tabKey.size() == 1 || keyID == 0) return tabKey[0].value;

		glm::quat a, b;
		float timea, timeb, deltaTime;
		a = tabKey[keyID - 1].value;
		b = tabKey[keyID].value;
		timea = tabKey[keyID - 1].times;
		timeb = tabKey[keyID].times;
		deltaTime = (timeInTicks - timea) / (timeb - timea);
		return glm::slerp(a, b, deltaTime);
	}

	void AnimMesh::BluidAnimedBones()
	{

		if (skeleton.tJoints.empty())
		{
			BH3D_LOGGERERROR("skeleton.tJoints.empty()");
			return;
		}

		std::vector<Joint> & tJoints = skeleton.tJoints;

		animSkeletonVBO.Destroy();

		std::vector<glm::vec3> tVertex, tColor;
		tVertex.reserve(tJoints.size() * 2);
		tColor.reserve(tJoints.size() * 2);

		glm::vec3  point;

		//LocalToGlobalPose
		//tJoints[0].localPose = tJoints[0].globalPose ; //root
		//parentPoint = glm::vec3(tJoints[0].globalPose*glm::vec4(0.0f,0.0f,0.0f,1.0f));
		for (unsigned int i = 1; i < tJoints.size(); i++)
		{

			point = glm::vec3(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			tVertex.push_back(point);
			point = glm::vec3(tAnimJointTransforms[i] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			tVertex.push_back(point);

			tColor.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			tColor.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

		}
		if (tVertex.empty()) return;
		animSkeletonVBO.AddArrayBufferData(BH3D_VERTEX_ATTRIB_INDEX, tVertex);
		animSkeletonVBO.AddArrayBufferData(BH3D_COLOR_ATTRIB_INDEX, tColor);
		animSkeletonVBO.Create();

		BH3D_CHECKGLERROR();
	}


	void AnimMesh::Draw()
	{

		BH3D_ASSERT_MSG( (IsValid() && tSubMeshes.size()), "Missing Call to Compute()", )

		vbo.Enable();
		Shader * shader = Shader::CurrentBindShader();

		for (unsigned int j = 0; j < skeleton.tJoints.size(); j++)
		{
			shader->SendTransform(tAnimJointGlobalTransforms[j]); //envoie la matrice de transformation des joints
			const auto & meshIDs = skeleton.tJoints[j].tMeshIDs;
			for (unsigned int m = 0; m < meshIDs.size(); m++)
			{
				unsigned int id = meshIDs[m];
				tSubMeshes[id].nMaterial.Bind();
				//UpdateAnimJoints();
				GetBoneMatrices(j, 0);

				shader->SendMat4f(BH3D_BONE_MATRIX_UNIFORM_NAME, 0, tBonesGlobalTransforms);
				glDrawElements(GL_TRIANGLES, (GLsizei)tSubMeshes[id].nFaces * 3, GL_UNSIGNED_INT, BH3D_BUFFER_OFFSET(tSubMeshes[id].faceOffset * 3 * sizeof(unsigned int)));
			}
		}
		vbo.Disable();

	}

	void AnimMesh::DrawAnimSkeleton(Shader *shader)
	{
		if (skeleton.tJoints.empty())
		{
			BH3D_LOGGERERROR("skeleton.tJoints.empty()");
			return;
		}

		BluidAnimedBones();

		glBindTexture(GL_TEXTURE_2D, 0);
		//tJoints[0].localPose = tJoints[0].globalPose ; //root
		for (const auto & m : tAnimJointGlobalTransforms)
		{
			shader->SendTransform(m);
			skeleton.cube.Draw();
		}
		
		glLineWidth(3.0f);
		animSkeletonVBO.Enable();
		for (unsigned int i = 1; i < skeleton.tJoints.size(); i++)
		{
			shader->SendTransform(tAnimJointGlobalTransforms[skeleton.tJoints[i].parentID]);
			glDrawArrays(GL_LINES, (i - 1) * 2, 2);
		}

		animSkeletonVBO.Disable();
		glLineWidth(1.0f);
	}


}