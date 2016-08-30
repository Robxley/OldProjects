//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxMesh.h"
#include "RxVBO.h"
#include <algorithm>

RxMesh::RxMesh():
nbFaces(0),computed(0),
formatTexCoord3(false),formatColor4(false)
{
	type = RX_MESH_ITEM3D;
}
RxMesh::~RxMesh() 
{
	Destroy();
}

bool RxMesh::IsValide()
{ 
	return (vbo.IsValide() && computed);
}


bool RxMesh::AddSubMesh(GLuint nbFaces , const GLuint *ptFaces, GLuint nbVertices, const GLfloat * ptPositions, const GLfloat * ptNormals, const GLfloat * ptTexCoord, bool _formatTexCoord3, const GLfloat *ptColors, bool _formatColor4, const RxMaterial *pMaterial )
{
	#ifdef _DEBUG
	if(!nbFaces || !ptFaces)
	{
		RXLOGGERERROR("[RxMesh][AddSubMesh] invalid param");
		return RX_ERROR;
	}
	#endif

	computed = 0;
	boundingBox.size = glm::vec3(0.0,0.0,0.0); //réinitialise le bounding box

	if(!LoadSubMesh(nbFaces , ptFaces, nbVertices, ptPositions, ptNormals, ptTexCoord, _formatTexCoord3, ptColors, _formatColor4,  pMaterial))
	{
		RXLOGGERERROR("[RxMesh][AddSubMesh] Fail");
		return RX_ERROR;
	}

	return RX_OK;
}


void RxMesh::FreeArrayVertex()
{
		tPosition.clear();
		tNormal.clear();
		tTexCoord2.clear();
		tTexCoord3.clear();
		tColor3.clear();
		tColor4.clear();
		tTangent.clear();
}


bool RxMesh::ComputeMesh()
{
	#ifdef _DEBUG
	if(tPosition.empty())
	{
		RXLOGGERERROR("[RxMesh][ComputeMesh] No vertices");
		return RX_ERROR;
	}
	#endif

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

	nbFaces = 0;
	for(unsigned int i = 0; i < tSubMeshes.size(); i++)
	{
		    tSubMeshes[i].faceOffset = nbFaces;
			nbFaces += tSubMeshes[i].tFace.size();
			vbo.AddElementBufferData(tSubMeshes[i].tFace[0].id, tSubMeshes[i].tFace.size()*3);

	}
	



	if(vbo.Create(GL_STATIC_DRAW))
		return computed=RX_OK;
	else
			RXLOGGERERROR("[RxMesh][ComputeMesh] VBO Fail");

	return RX_ERROR;
}




void RxMesh::Destroy()
{

	vbo.Destroy();

	nbFaces = 0;
	tSubMeshes.clear();
	computed = 0;

	tPosition.clear();
	tNormal.clear();
	tTexCoord2.clear();
	tTexCoord3.clear();
	tColor3.clear();
	tColor4.clear();
	tTangent.clear();
	formatTexCoord3 = formatColor4 = false;
}


bool RxMesh::LoadSubMesh(GLuint nbFaces, const GLuint *ptFaces, GLuint nbVertices, const GLfloat * ptPositions, const GLfloat * ptNormals , const GLfloat * ptTexCoords, bool _formatTexCoord3, const GLfloat *ptColors, bool _formatColor4, const RxMaterial *pMaterial)
{
	
	//les données de chaque groupe doivent avoir le même format de vertex (4,3,2...)
	if(tSubMeshes.size())
	{
		if( formatTexCoord3 != _formatTexCoord3 )
		{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxMesh][LoadSubMesh] wrong formatColor3");
			#endif
			return RX_ERROR; //coordonnées textures : format différent
		}

		if( formatColor4 != _formatColor4 )
		{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxMesh][LoadSubMesh] wrong formatColor4");
			#endif
			return RX_ERROR;//couleurs : format différent
		}
	}


	if(tNormal.size() && ptNormals == NULL)
	{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxMesh][LoadSubMesh] New mesh description not match with existing meshes : normal missing");
			#endif
			return RX_ERROR;//couleurs : format différent
	}
	if((tTexCoord2.size() || tTexCoord3.size()) && ptTexCoords == NULL)
	{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxMesh][LoadSubMesh] New mesh description not match with existing meshes : texCoord missing");
			#endif
			return RX_ERROR;//couleurs : format différent
	}
	if((tColor4.size() || tColor3.size()) && ptColors == NULL)
	{
			#ifdef _DEBUG
				RXLOGGERERROR("[RxMesh][LoadSubMesh] New mesh description not match with existing meshes : color missing");
			#endif
			return RX_ERROR;//couleurs : format différent
	}



	unsigned int positionOffset = tPosition.size();

	tPosition.reserve(positionOffset+nbVertices);
	if(ptNormals != NULL)
		tNormal.reserve(positionOffset+nbVertices);
	if(ptTexCoords !=NULL)
	{
		if(formatTexCoord3)
			tTexCoord3.reserve(positionOffset+nbVertices);
		else
			tTexCoord2.reserve(positionOffset+nbVertices);
	}
	if(ptColors != NULL)
	{
		if(formatColor4)
			tColor4.reserve(positionOffset+nbVertices);
		else
			tColor3.reserve(positionOffset+nbVertices);
	}




	//recopie des vertices
	for(unsigned int i = 0; i < nbVertices; i++)
	{
		tPosition.push_back(glm::vec3(ptPositions[i*3],ptPositions[i*3+1],ptPositions[i*3+2]));
		if(ptNormals != NULL)
			tNormal.push_back(glm::vec3(ptNormals[i*3],ptNormals[i*3+1],ptNormals[i*3+2]));
		
		if(ptTexCoords !=NULL)
		{
			if(formatTexCoord3)
				tTexCoord3.push_back(glm::vec3(ptTexCoords[i*3],ptTexCoords[i*3+1],ptTexCoords[i*3+2]));
			else
				tTexCoord2.push_back(glm::vec2(ptTexCoords[i*2],ptTexCoords[i*2+1]));
		
		}
		if(ptColors != NULL)
		{
			if(formatColor4)
				tColor4.push_back(glm::vec4(ptColors[i*4],ptColors[i*4+1],ptColors[i*4+2],ptColors[i*4+3]));
			else
				tColor3.push_back(glm::vec3(ptColors[i*3],ptColors[i*3+1],ptColors[i*3+2]));
		}
	}

	unsigned int offsetface = getNbFaces();



	//enregistre les faces du groupe
	tSubMeshes.push_back(RxMesh::RxSubMesh());
	tSubMeshes.back().tFace.reserve(nbFaces);
	tSubMeshes.back().vertexOffset = positionOffset;
	tSubMeshes.back().faceOffset = offsetface;
	
	RxFace face;

	for(unsigned int i=0; i < nbFaces; i++)
	{
		face.id[0] = ptFaces[i*3+0]+offsetface;
		face.id[1] = ptFaces[i*3+1]+offsetface;
		face.id[2] = ptFaces[i*3+2]+offsetface;
		tSubMeshes.back().tFace.push_back(face);
		
	}
	if(pMaterial)
		tSubMeshes.back().nMaterial = (*pMaterial);

	return RX_OK;
}
unsigned int RxMesh::getNbFaces()
{
	nbFaces = 0;
	for(unsigned int i = 0; i < tSubMeshes.size(); i++)
			nbFaces += tSubMeshes[i].tFace.size();

		return nbFaces;
}


void RxMesh::ScaleMesh(glm::vec3 scale)
{
	if(tPosition.size())
	{
		#ifdef _DEBUG
		  RXLOGGERWARNING("[RxMesh][ScaleMesh] tPosition.size() == 0");
		#endif
		return;
	}

	if( scale == glm::vec3(0.0f,0.0f,0.0f) || scale == glm::vec3(1.0f,1.0f,1.0f) ) return;

	unsigned int i;
	for(i = 0; i < tPosition.size() ; i++)
	{
		tPosition[i].x *=scale.x;
		tPosition[i].y *=scale.y;
		tPosition[i].z *=scale.z;
	}

	boundingBox.size = glm::vec3(0.0,0.0,0.0); //réinitialise le bounding box
}
void RxMesh::Draw()
{

	#ifdef _DEBUG
	if(!IsValide() || tSubMeshes.empty())
	{
		  RXLOGGERERROR("[RxMesh][Draw] Missing Call to Compute()");
		  return;
	}
	#endif


			//if(shader) shader->SendMat4f(transform_uniform_name.c_str(),GL_FALSE,transform);
	
	vbo.Enable();
		for(unsigned int i = 0; i < tSubMeshes.size(); i++)
		{
			
			tSubMeshes[i].nMaterial.Bind();

			glDrawElements(GL_TRIANGLES, (GLsizei)tSubMeshes[i].tFace.size()*3, GL_UNSIGNED_INT, ((GLchar*)NULL)+(tSubMeshes[i].faceOffset*3*sizeof(unsigned int)));
			
		}
	vbo.Disable();

}

void RxMesh::Draw(GLuint group)
{

	if( !vbo.IsValide() ) return;
	//if(shader) shader->SendMat4f(transform_uniform_name.c_str(),GL_FALSE,transform);

	vbo.Enable();
	tSubMeshes[group].nMaterial.Bind();

	glDrawElements(GL_TRIANGLES, (GLsizei)tSubMeshes[group].tFace.size()*3, GL_UNSIGNED_INT, ((GLchar*)NULL)+tSubMeshes[group].faceOffset*3*sizeof(unsigned int));

	vbo.Disable();

}

void RxMesh::setMaterial(RxMaterial &m)
{
		material = m;
		unsigned int i;
		for( i = 0 ; i < tSubMeshes.size(); i++)
			tSubMeshes[i].nMaterial = m;
}


void RxMesh::setTexture(RxTexture &t)
{
		material.colorMap = t;
		unsigned int i;
		for( i = 0 ; i < tSubMeshes.size(); i++)
			tSubMeshes[i].nMaterial.colorMap = t;
}

void RxMesh::setTexture(GLuint id)
{
		RxTexture &tex = RxTexManager::Instance().getTexture(id);
		if(tex.glID == 0) return;
		material.colorMap = tex;
		unsigned int i;
		for( i = 0 ; i < tSubMeshes.size(); i++)
			tSubMeshes[i].nMaterial.colorMap = tex;
}
void RxMesh::setNormalMap(GLuint id)
{
		RxTexture &tex = RxTexManager::Instance().getTexture(id);
		if(tex.glID == 0) return;
		material.normalMap = tex;
		unsigned int i;
		for( i = 0 ; i < tSubMeshes.size(); i++)
			tSubMeshes[i].nMaterial.normalMap = tex;
}
void RxMesh::setTexture(std::string &filename)
{
		material.colorMap = RxTexManager::Instance().LoadTexture(filename.c_str());
		unsigned int i;
		for( i = 0 ; i < tSubMeshes.size(); i++)
			tSubMeshes[i].nMaterial.normalMap = material.colorMap;
}
void RxMesh::setTextureSubMesh(unsigned int idg, RxTexture &texture)
{
	if( idg < tSubMeshes.size())
	{
			tSubMeshes[idg].nMaterial.colorMap = texture;
	}
}
void RxMesh::setTextureSubMesh(unsigned int idg, GLuint id)
{
	if( idg < tSubMeshes.size())
	{
		RxTexture &tex = RxTexManager::Instance().getTexture(id);
		if(tex.glID )
			tSubMeshes[idg].nMaterial.colorMap = tex;
	}
}
void RxMesh::setTextureSubMesh(unsigned int idg, std::string &filename)
{
	if( idg < tSubMeshes.size())
	{
		tSubMeshes[idg].nMaterial.colorMap = RxTexManager::Instance().LoadTexture(filename.c_str());
	}
}
void RxMesh::setMaterialSubMesh(unsigned int idg, const RxMaterial &m)
{
	if( idg < tSubMeshes.size())
		tSubMeshes[idg].nMaterial = m;
}
void RxMesh::setColorSubMesh(unsigned int idg, glm::vec4 &color)
{
	if( idg < tSubMeshes.size() )
		tSubMeshes[idg].nMaterial.color = color;

}

RxMaterial &RxMesh::getMaterialSubMesh(unsigned int idg)
{
	if(idg < tSubMeshes.size() )
		return (tSubMeshes[idg].nMaterial);

	return (material);
}


void RxMesh::ComputeBoundingBox()
{
#ifdef _DEBUG
			if(tPosition.empty())
				RXLOGGERWARNING("[RxMesh][ComputeBoundingBox] tPosition empty");
#endif
			if(tPosition.empty()) return;
			if(boundingBox.IsValide()) return;
			
			glm::vec3 vmin,vmax;
			vmin = vmax = tPosition[0]; //initialisation
			for(unsigned int i = 1; i < tPosition.size(); i++)
			{
				for(int j = 0; j < 3; j++) //pour chacune des composantes
				{
					if( vmin[j] > tPosition[i][j] ) vmin[j] = tPosition[i][j];

					if( vmax[j] < tPosition[i][j] ) vmax[j] = tPosition[i][j];
				}
			}
			
			boundingBox.size = (vmax-vmin);
			boundingBox.position = 0.5f*(vmax+vmin);
			//boundingBox.size*=0.5;
			//boundingBox.position*=0.5;

}

void RxMesh::CenterDataToOrigin(glm::vec3 offset)
{
	#ifdef _DEBUG
			if(tPosition.empty())
				RXLOGGERWARNING("[RxMesh][CenterDataToOrigin] tPosition empty");

			if(computed)
				RXLOGGERWARNING("[RxMesh][CenterDataToOrigin]  Already Computed");

			if(!boundingBox.IsValide())
				RXLOGGERWARNING("[RxMesh][CenterDataToOrigin]  Bounding Box no valide");
	#endif

	if(!boundingBox.IsValide())
		ComputeBoundingBox();


	unsigned int i;

	glm::vec3 origine = boundingBox.position - offset;
	if(boundingBox.position != glm::vec3(0,0,0))
	{
		for(i = 0; i < tPosition.size() ; i++)
		{
			tPosition[i]-=(origine);
		}
		boundingBox.position = glm::vec3(0.0f,0.0f,0.0f);
	}

	//boundingBox.size = glm::vec3(0.0,0.0,0.0);
}

void RxMesh::NormalizeData(bool keepRatio)
{
	#ifdef _DEBUG
			if(tPosition.empty())
				RXLOGGERWARNING("[RxMesh][NormalizeData] tPosition empty");

			if(computed)
				RXLOGGERWARNING("[RxMesh][NormalizeData] Already Computed");
	#endif
		if(!boundingBox.IsValide())
			ComputeBoundingBox();
		


		float dx = 1.0f,dy = 1.0f,dz = 1.0f;

		if(boundingBox.size.x)
		{
			dx = 1.0f/boundingBox.size.x;
			boundingBox.size.x = 1.0f;
		}
		if(boundingBox.size.y)
		{
			dy = 1.0f/boundingBox.size.y;
			boundingBox.size.y = 1.0f;
		}
		if(boundingBox.size.z)
		{
			dz = 1.0f/boundingBox.size.z;
			boundingBox.size.z = 1.0f;
		}
		

		if(keepRatio)
		{
			dx=dy=dz=std::min(std::min(dx,dy),dz);
		}

		if(dx != 1.0f)
		for(unsigned int i = 0; i < tPosition.size() ; i++)
				tPosition[i].x *= dx;

		if(dy != 1.0f)
		for(unsigned int i = 0; i < tPosition.size() ; i++)		
				tPosition[i].y *= dy;	

		if(dz != 1.0f)
		for(unsigned int i = 0; i < tPosition.size() ; i++)
				tPosition[i].z *= dz;

		boundingBox.size = glm::vec3(0.0,0.0,0.0);
		
}

void RxMesh::TransformMesh(glm::mat4 &mat)
{
	if(tPosition.size())
	{
		#ifdef _DEBUG
		  RXLOGGERWARNING("[RxMesh][ScaleMesh] tPosition.size() == 0");
		#endif
		return;
	}

	computed = 0;
	boundingBox.size = glm::vec3(0.0,0.0,0.0); //réinitialise le bounding box

	for(unsigned int i = 0; i < tPosition.size(); i++)
		tPosition[i] = glm::vec3(mat*glm::vec4(tPosition[i],1));
	
	for(unsigned int i = 0; i < tNormal.size(); i++)
		tNormal[i] = glm::vec3(mat*glm::vec4(tNormal[i],1));

	for(unsigned int i = 0; i < tTangent.size(); i++)
		tTangent[i] = glm::vec3(mat*glm::vec4(tTangent[i],1));


}


std::vector<glm::vec3>&	RxMesh::getTabPosition()
{
	return tPosition;
}
std::vector<glm::vec3>&	RxMesh::getTabNormal()
{
	return tNormal;
}
std::vector<glm::vec2>&	RxMesh::getTabTexCoord2()
{
	return tTexCoord2;
}
std::vector<glm::vec3>&	RxMesh::getTabTexCoord3()
{
	return tTexCoord3;
}
std::vector<glm::vec3>&  RxMesh::getTabColor3()
{
	return tColor3;
}
std::vector<glm::vec4>&  RxMesh::getTabColor4()
{
	return tColor4;
}
std::vector<glm::vec3>&  RxMesh::getTabTangent()
{
	return tTangent;
}
std::vector<RxMesh::RxSubMesh>&  RxMesh::getTabSubMeshes()
{
	return tSubMeshes;
}

