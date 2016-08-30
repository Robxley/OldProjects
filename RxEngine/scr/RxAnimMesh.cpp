#include "RxAnimMesh.h"


void RxSkeleton::AddBone(RxBone &bone)
{
	tBones.push_back(bone);
}
void RxSkeleton::AddJoint(RxJoint &joint)
{
	tJoints.push_back(joint);
}

void RxSkeleton::UpdateBindJoints()
{ 
		if(tJoints.empty()) return;

		tJoints[0].globalPose = tJoints[0].localPose ; 
		for(unsigned int i = 1; i < tJoints.size(); i++)
		{
			tJoints[i].globalPose = tJoints[tJoints[i].parentID].globalPose *  tJoints[i].localPose;
		}
}


void RxSkeleton::BluidBones()
{
		sphere.set(0.2f);
		#ifdef _DEBUG
			if(tJoints.empty()) RXLOGGERERROR("[RxSkeleton][BluidBones] tJoints.empty()");
		#endif 

		if(tJoints.empty()) return;

		UpdateBindJoints();


	    std::vector<glm::vec3> tVertex, tColor;
		tVertex.reserve(tJoints.size()*2);
		tColor.reserve(tJoints.size()*2);

		glm::vec3  point;

		for(unsigned int i = 1; i < tJoints.size(); i++)
		{

			point = glm::vec3(glm::vec4(0.0f,0.0f,0.0f,1.0f));
			tVertex.push_back (point);
			point = glm::vec3(  tJoints[i].localPose  *glm::vec4(0.0f,0.0f,0.0f,1.0f));
			tVertex.push_back(point);

			tColor.push_back(glm::vec3(1.0f,0.0f,0.0f));
			tColor.push_back(glm::vec3(0.0f,1.0f,0.0f));

		}
		if(tVertex.empty()) return;
		skeletonVBO.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,tVertex);
		skeletonVBO.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX,tColor);
		skeletonVBO.Create();


		#ifdef _DEBUG
			RXCHECKGLERROR("[RxAnimMesh][BluidBones]");
		#endif		

}

void RxSkeleton::Draw(RxShader *shader)
{
#ifdef _DEBUG
	if(tJoints.empty()) {
		RXLOGGERERROR("[RxSkeleton][Draw] tJoints.empty()");
		return;
	}
#endif
		glBindTexture(GL_TEXTURE_2D,0);
		
		for(unsigned int i = 0; i < tJoints.size(); i++)
		{
			shader->SendTransform(tJoints[i].globalPose);
			sphere.Draw();
		}


		glLineWidth(3.0f);
		skeletonVBO.Enable();
		for(unsigned int i = 1; i < tJoints.size(); i++)
		{
			shader->SendTransform(tJoints[tJoints[i].parentID].globalPose);
			glDrawArrays(GL_LINES,(i-1)*2,2);
		}

		skeletonVBO.Disable();
		glLineWidth(1.0f);
}



/*********************************************************/
/*********RxAnimMesh**********************************/
/*********************************************************/
RxAnimMesh::RxAnimMesh()
{
	lastTimeInTick = 0;
}

bool RxAnimMesh::ComputeMesh()
{
	#ifdef _DEBUG
	if(tPosition.empty())
	{
		RXLOGGERERROR("[RxMesh][ComputeMesh] No vertices");
		return RX_ERROR;
	}
	#endif

	if(tPosition.empty()) return RX_ERROR;

	vbo.Destroy();


	ComputeBoundingBox();

	//construction du vbo
	vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX, tPosition);

	if(tNormal.size())
		vbo.AddArrayBufferData(RX_NORMAL_ATTRIB_INDEX, tNormal);

	if(tTexCoord2.size())
		vbo.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX, tTexCoord2);
	else if(tTexCoord3.size())
		vbo.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX, tTexCoord3);

	if(tColor3.size())
		vbo.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX, tColor3);
	else if(tColor4.size())
		vbo.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX, tColor4);

	if(tTangent.size())
		vbo.AddArrayBufferData(RX_DATA0_ATTRIB_INDEX, tTangent);



	if(tVertexBoneInfo.size())
	{
		unsigned int attrib[] = {RX_DATA1_ATTRIB_INDEX,RX_DATA2_ATTRIB_INDEX};
		unsigned int attribType[] = {GL_UNSIGNED_INT,GL_FLOAT};
		unsigned int attribSize[] = {RX_MAX_WEIGHTS_BY_VERTEX,RX_MAX_WEIGHTS_BY_VERTEX};
		unsigned int attribOffsetStart[] = {0,RX_MAX_WEIGHTS_BY_VERTEX*sizeof(unsigned int)};
		unsigned int stride = RX_MAX_WEIGHTS_BY_VERTEX*(sizeof(float)+sizeof(unsigned int));
		unsigned int size = tVertexBoneInfo.size()*stride;
		unsigned int attribInType[] = {RX_ATTRIBUT_IVEC,RX_ATTRIBUT_VEC};
		vbo.AddStructArrayBufferData(2,attrib,attribType,attribSize,attribOffsetStart,stride,size,&(tVertexBoneInfo[0].tBoneIDs[0]),attribInType);
	}



	nbFaces = 0;
	for(unsigned int i = 0; i < tSubMeshes.size(); i++)
	{
		    tSubMeshes[i].faceOffset = nbFaces;
			nbFaces += tSubMeshes[i].tFace.size();
			vbo.AddElementBufferData(tSubMeshes[i].tFace[0].id, tSubMeshes[i].tFace.size()*3);

	}
	
	skeleton.UpdateBindJoints();


	if(!vbo.Create(GL_STATIC_DRAW))
	{
		return RX_ERROR;
		RXLOGGERERROR("[RxAnimMesh][ComputeMesh] VBO Fail");
	}



	#ifdef _DEBUG
	RXCHECKGLERROR("[RxAnimMesh][ComputeMesh]");
	#endif
	return computed=RX_OK;

}



std::vector<RxAnimMesh::RxVertexBoneInfo>&  RxAnimMesh::getTabVertexBoneInfo()
{
	return tVertexBoneInfo;
}


glm::mat4 RxAnimMesh::KeyTransform(glm::vec3 kscale, glm::quat kquat, glm::vec3 ktranslate)
{

	glm::mat4 S = glm::scale(glm::mat4(1), kscale);
	glm::mat4 R = glm::mat4_cast(kquat);
	glm::mat4 T = glm::translate(glm::mat4(1), ktranslate);

	return (T*R*S);

}

void RxAnimMesh::PlayAnimationPerTick(unsigned int ID, unsigned int tick)
{
	if(ID > skeleton.tAnimations.size() || skeleton.tAnimations.empty())
	{
	#ifdef _DEBUG
			RXLOGGERERROR("[RxAnimMesh][PlayAnimation] None animation");
	#endif
		return;
	}
	float timeInSeconds = float(tick);
	if(skeleton.tAnimations[ID].ticksPerSecond>0)
		timeInSeconds /= skeleton.tAnimations[ID].ticksPerSecond;

	PlayAnimation(ID,timeInSeconds);
}



void RxAnimMesh::PlayAnimation(unsigned int ID, float timeInSeconds)
{

	if(ID > skeleton.tAnimations.size() || skeleton.tAnimations.empty())
	{
	#ifdef _DEBUG
			RXLOGGERERROR("[RxAnimMesh][PlayAnimation] None animation");
	#endif
		return;
	}


	if(skeleton.tJoints.empty())
	{
	#ifdef _DEBUG
		RXLOGGERERROR("[RxAnimMesh][PlayAnimation] skeleton.tJoints.empty()");
	#endif
		return;
	}


	RxAnimation & animation = skeleton.tAnimations[ID];

	timeInTicks = timeInSeconds * animation.ticksPerSecond;
	if(animation.duration > 0.0)
		timeInTicks = fmod(timeInTicks,animation.duration);


	unsigned int sKey,pKey,rKey;
	glm::vec3 posKey, scaleKey;
	glm::quat rotKey;

	//reset des matrices d'animation
	tAnimJointTransforms.resize(skeleton.tJoints.size());

	//permet de savoir quel joint est animé
	std::vector<bool> JointIsAnimated;
	JointIsAnimated.resize(skeleton.tJoints.size(),0);

	//calcul des matrices d'animation
	for(unsigned int i = 0; i < animation.tAnimKey.size(); i++)
	{


		findTransformKey(animation.tAnimKey[i], &sKey, &pKey, &rKey);  //recherche la frameKey
		//calcul la matrice d'interpolation
		scaleKey = InterpolationKey(animation.tAnimKey[i].tScaleKeys,sKey);
		posKey = InterpolationKey(animation.tAnimKey[i].tPosKeys,pKey);
		rotKey = InterpolationKey(animation.tAnimKey[i].tRotKeys,rKey);

		unsigned int jointID = animation.tAnimKey[i].joint;
		tAnimJointTransforms[jointID] = KeyTransform(scaleKey,rotKey,posKey);//sauvegarde la matrice d'animation
		JointIsAnimated[jointID] = 1; //joint modifié

		animation.tAnimKey[i].lastPosKey = pKey;
		animation.tAnimKey[i].lastScaleKey = sKey;
		animation.tAnimKey[i].lastRotKey = rKey;

	}

	//si le joint n'est pas animé alors la matrice d'animation correspond à la matrice locale du joint
	for(unsigned int i = 0; i < skeleton.tJoints.size(); i++)
	{
		if(!JointIsAnimated[i])
			tAnimJointTransforms[i] = skeleton.tJoints[i].localPose;
	}
	
	lastTimeInTick = timeInTicks;

	UpdateAnimJoints();

}

void RxAnimMesh::findTransformKey(const RxAnimJointKey & animKey, unsigned int * scaleID, unsigned int * posID, unsigned int * rotID)
{

	unsigned int i;
	//cherche le plus proche
	(*scaleID) = 0;
	(*posID) = 0;
	(*rotID) = 0;

	i = (timeInTicks >= lastTimeInTick) ? animKey.lastScaleKey : 0; //évite de commencer à 0 à chaque fois
	for(; i < animKey.tScaleKeys.size(); i++)
	{
		if(animKey.tScaleKeys[i].times >= timeInTicks)
		{
				(*scaleID) = i;
				break;
		}
	}
	i = (timeInTicks >= lastTimeInTick) ? animKey.lastRotKey : 0;
	for(; i < animKey.tRotKeys.size(); i++)
	{
		if(animKey.tRotKeys[i].times >= timeInTicks)
		{
				(*rotID) = i;
				break;
		}
	}
	i = (timeInTicks >= lastTimeInTick) ? animKey.lastPosKey : 0;
	for(; i < animKey.tPosKeys.size(); i++)
	{
		if(animKey.tPosKeys[i].times >= timeInTicks)
		{
				(*posID) = i;
				break;
		}
	}


}

glm::vec3 RxAnimMesh::InterpolationKey(const std::vector<RxAnimJointKey::RxVec3Key> & tabKey, unsigned int keyID)
{
	if(tabKey.empty()) 
		return glm::vec3(0.0f);
	if( timeInTicks == tabKey[keyID].times) return tabKey[keyID].value;
	if(tabKey.size() == 1 || keyID == 0) return tabKey[0].value;


	glm::vec3 a,b;
	float timea, timeb, deltaTime;
	a = tabKey[keyID-1].value;
	b = tabKey[keyID].value;
	timea = tabKey[keyID-1].times;
	timeb = tabKey[keyID].times;
	deltaTime = (timeInTicks-timea)/(timeb-timea);
	return glm::mix(a,b,deltaTime);
}


glm::quat RxAnimMesh::InterpolationKey(const std::vector<RxAnimJointKey::RxQuatKey> & tabKey, unsigned int keyID)
{
	if(tabKey.empty()) return glm::quat();
	if( timeInTicks == tabKey[keyID].times) return tabKey[keyID].value;
	if(tabKey.size() == 1 || keyID == 0 ) return tabKey[0].value;

	
	glm::quat a,b;
	float timea, timeb, deltaTime;
	a = tabKey[keyID-1].value;
	b = tabKey[keyID].value;
	timea = tabKey[keyID-1].times;
	timeb = tabKey[keyID].times;
	deltaTime = (timeInTicks-timea)/(timeb-timea);
	return glm::slerp(a,b,deltaTime);
}





void RxAnimMesh::BluidAnimedBones()
{

		if(skeleton.tJoints.empty())
		{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxAnimMesh][DrawAnimSkeleton] skeleton.tJoints.empty()");
			#endif
				return;
		}

	    std::vector<RxJoint> & tJoints = skeleton.tJoints;


		animSkeletonVBO.Destroy();

	    std::vector<glm::vec3> tVertex, tColor;
		tVertex.reserve(tJoints.size()*2);
		tColor.reserve(tJoints.size()*2);

		glm::vec3  point;

	    //LocalToGlobalPose
		//tJoints[0].localPose = tJoints[0].globalPose ; //root
		//parentPoint = glm::vec3(tJoints[0].globalPose*glm::vec4(0.0f,0.0f,0.0f,1.0f));
		for(unsigned int i = 1; i < tJoints.size(); i++)
		{

			point = glm::vec3(glm::vec4(0.0f,0.0f,0.0f,1.0f));
			tVertex.push_back (point);
			point = glm::vec3(tAnimJointTransforms[i] *glm::vec4(0.0f,0.0f,0.0f,1.0f));
			tVertex.push_back(point);

			tColor.push_back(glm::vec3(1.0f,0.0f,0.0f));
			tColor.push_back(glm::vec3(0.0f,1.0f,0.0f));

		}
		if(tVertex.empty()) return;
		animSkeletonVBO.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,tVertex);
		animSkeletonVBO.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX,tColor);
		animSkeletonVBO.Create();


		#ifdef _DEBUG
			RXCHECKGLERROR("[RxAnimMesh][BluidBones]");
		#endif	
}


void RxAnimMesh::Draw()
{

	#ifdef _DEBUG
	if(!IsValide() || tSubMeshes.empty())
	{
		  RXLOGGERERROR("[RxMesh][Draw] Missing Call to Compute()");
		  return;
	}
	#endif
	vbo.Enable();


	for(unsigned int j = 0; j < skeleton.tJoints.size(); j++)
	{

		shader->SendTransform(tAnimJointGlobalTransforms[j]); //envoi la matrice de transformation
		for(unsigned int m = 0; m < skeleton.tJoints[j].tMeshIDs.size(); m++)
		{
			unsigned int id = skeleton.tJoints[j].tMeshIDs[m];
			tSubMeshes[id].nMaterial.Bind();
			//UpdateAnimJoints();
			GetBoneMatrices(j,0);
			
			shader->SendMat4f(RX_BONE_MATRIX_UNIFORM_NAME,0,tBonesGlobalTransforms);
			glDrawElements(GL_TRIANGLES, (GLsizei)tSubMeshes[id].tFace.size()*3, GL_UNSIGNED_INT, ((GLchar*)NULL)+(tSubMeshes[id].faceOffset*3*sizeof(unsigned int)));
		}
	}
	vbo.Disable();

}
void RxAnimMesh::GetBoneMatrices(unsigned int idJoint, unsigned int idMesh)
{
	tBonesGlobalTransforms.resize(skeleton.tJoints.size());

	//glm::mat4 invGlobalJointTransform = glm::inverse(tAnimJointGlobalTransforms[idJoint]);

	for(unsigned int i = 0; i < skeleton.tJoints.size(); i++)
	{
		//tBonesGlobalTransforms[i] = invGlobalJointTransform * tAnimJointGlobalTransforms[i] *skeleton.tJoints[i].offsetPose;
		tBonesGlobalTransforms[i] =   tAnimJointGlobalTransforms[i]*skeleton.tJoints[i].offsetPose;
	}
}
void RxAnimMesh::UpdateAnimJoints()
{
		tAnimJointGlobalTransforms.resize(skeleton.tJoints.size());
		tAnimJointGlobalTransforms[0] =  tAnimJointTransforms[0] ; 
		for(unsigned int i = 1; i < skeleton.tJoints.size(); i++)
		{
			tAnimJointGlobalTransforms[i] = tAnimJointGlobalTransforms[skeleton.tJoints[i].parentID] *  tAnimJointTransforms[i];
		}
}

void RxAnimMesh::DrawAnimSkeleton(RxShader *shader)
{
		if(skeleton.tJoints.empty())
		{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxAnimMesh][DrawAnimSkeleton] skeleton.tJoints.empty()");
			#endif
				return;
		}

		BluidAnimedBones();

		glBindTexture(GL_TEXTURE_2D,0);
		//tJoints[0].localPose = tJoints[0].globalPose ; //root
		for(unsigned int i = 0; i < skeleton.tJoints.size(); i++)
		{
			shader->SendTransform(tAnimJointGlobalTransforms[i]);
			skeleton.sphere.Draw();
		}


		glLineWidth(3.0f);
		animSkeletonVBO.Enable();
		for(unsigned int i = 1; i < skeleton.tJoints.size(); i++)
		{

				shader->SendTransform(tAnimJointGlobalTransforms[skeleton.tJoints[i].parentID]);
				glDrawArrays(GL_LINES,(i-1)*2,2);
			
		}

		animSkeletonVBO.Disable();
		glLineWidth(1.0f);
}
