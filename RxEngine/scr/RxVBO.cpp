//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :		RxVBO.cpp							       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxVBO.h"
#include "RxConst.h"


RxVBO::RxVBO():arrayBufferID(0),elementBufferID(0),vertexArraysID(0)
{
}

RxVBO::~RxVBO()
{
	Destroy();
}



bool RxVBO::Create(GLenum usage)
{

	if(arrayBufferID || elementBufferID)
		Destroy();

	//creation vbo
	if(tArrayBuffer.size())
	{
		unsigned int fullSize = 0;

		//calcul la taille max
		for(unsigned int i = 0; i < tArrayBuffer.size(); i++)
		{
			fullSize += tArrayBuffer[i].size;
		}

		//creation du VBO
		glGenBuffers(1, &arrayBufferID); //génération d'un buffer array
		glBindBuffer(GL_ARRAY_BUFFER, arrayBufferID);  //activation du buffer array
		glBufferData(GL_ARRAY_BUFFER,fullSize,NULL,usage); //allocation mémoire du buffer array

		//remplissage du VBO avec les données
		unsigned int offset = 0;
		for(unsigned int i = 0; i < tArrayBuffer.size(); i++)
		{
			glBufferSubData(GL_ARRAY_BUFFER, offset, tArrayBuffer[i].size, tArrayBuffer[i].data);
			offset+=tArrayBuffer[i].size; //on décale l'offset aux données suivantes.
		}
	}

	//creation element array
	if(tElementBuffer.size())
	{

		//calcul la taille maximal
		unsigned int fullSize = 0;
		unsigned int i;
		for(i = 0 ; i < tElementBuffer.size() ; i++)
			fullSize += tElementBuffer[i].dataUIntSize;


		glGenBuffers(1, &elementBufferID); //génération d'un buffer array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);  //activation du buffer array
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT)*fullSize, NULL, usage);

		//remplissage du buffer element
		int offset = 0;
		for(i = 0 ; i < tElementBuffer.size(); i++)
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,offset*sizeof(GL_UNSIGNED_INT), tElementBuffer[i].dataUIntSize*sizeof(GL_UNSIGNED_INT),tElementBuffer[i].data);
			offset+=tElementBuffer[i].dataUIntSize; //on décale l'offset aux données suivantes.
		}

	}


	//création et définition du VAO
	glGenVertexArrays(1, &vertexArraysID);
	glBindVertexArray(vertexArraysID);
	

	//Liaison avec Element Array
	if(elementBufferID)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);

	if(arrayBufferID)
	{
		//Liaison avec le VBO
		glBindBuffer(GL_ARRAY_BUFFER, arrayBufferID);

		unsigned int offset = 0;
		for(unsigned int i = 0; i < tArrayBuffer.size(); i++)
		{
			
			//pour chaque attribut
			for(unsigned int k = 0; k < tArrayBuffer[i].nbAttrib; k++)
			{
				glEnableVertexAttribArray (tArrayBuffer[i].tAttribIndex[k]);

				if(tArrayBuffer[i].tAttribInType.size() && tArrayBuffer[i].tAttribInType[k] == RX_ATTRIBUT_IVEC)
					glVertexAttribIPointer(tArrayBuffer[i].tAttribIndex[k], tArrayBuffer[i].tAttribSize[k],  tArrayBuffer[i].tAttribType[k], tArrayBuffer[i].stride, ((GLchar*)NULL)+offset+tArrayBuffer[i].tAttribOffsetStart[k]);
				else
					glVertexAttribPointer(tArrayBuffer[i].tAttribIndex[k], tArrayBuffer[i].tAttribSize[k],  tArrayBuffer[i].tAttribType[k], GL_FALSE, tArrayBuffer[i].stride, ((GLchar*)NULL)+offset+tArrayBuffer[i].tAttribOffsetStart[k]);
				
					
			}
			offset+=tArrayBuffer[i].size;
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	


	#if _DEBUG
	RXCHECKGLERROR("[RxVBO][Create]");
	#endif

	return (arrayBufferID && (elementBufferID || tElementBuffer.empty()) && vertexArraysID);
}
void RxVBO::Enable()
{
	#if _DEBUG
	if(!arrayBufferID)
		RXLOGGERERROR("[RxVBO][Enable] Buffer id = 0");
	#endif

	glBindVertexArray(vertexArraysID);
		
	#if _DEBUG
	RXCHECKGLERROR("[RxVBO][Enable]");
	#endif	

}
void RxVBO::Disable()
{
	glBindVertexArray(0);
}

void RxVBO::Destroy()
{
	if(arrayBufferID) glDeleteBuffers(1, &arrayBufferID);
	if(elementBufferID) glDeleteBuffers(1, &elementBufferID);
	if(elementBufferID) glDeleteVertexArrays(1,&vertexArraysID);
	arrayBufferID = elementBufferID = vertexArraysID= 0;

	tElementBuffer.clear();
	tArrayBuffer.clear();
}
GLuint RxVBO::getElementSize()
{
	unsigned int i, size=0;
	for(i = 0; i < tElementBuffer.size();i++)
		size;

	return size;
}
GLuint RxVBO::getVertexArraysID()
{
	return vertexArraysID;
}
GLuint RxVBO::getArrayBufferID()
{
	return arrayBufferID;
}
GLuint RxVBO::getElementBufferID()
{
	return elementBufferID;
}

bool RxVBO::IsValide()
{
	return (arrayBufferID || elementBufferID);
}

/****************float ***********************************************/
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const float* data, unsigned int dataSize, GLint vertexSize)
{	
	if( dataSize == 0 || data == NULL)
	{
		#ifdef _DEBUG
			RXLOGGERERROR("[RxVBO][AddArrayBufferData] data == NULL or empty");
		#endif

		return;
	}
	
	tArrayBuffer.push_back(ArrayBuffer());
	tArrayBuffer.back().nbAttrib = 1;
	tArrayBuffer.back().data = data;
	tArrayBuffer.back().size = dataSize*sizeof(float);
	tArrayBuffer.back().tAttribIndex.push_back(indexAttrib);
	tArrayBuffer.back().tAttribOffsetStart.push_back(0);
	tArrayBuffer.back().tAttribSize.push_back(vertexSize);
	tArrayBuffer.back().tAttribType.push_back(GL_FLOAT);


	
}


//float GLM::VECx
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const glm::vec2* data, unsigned int dataSize)
{
	AddArrayBufferData(indexAttrib, (const float*) data, dataSize*2, 2);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const glm::vec3* data, unsigned int dataSize)
{
	AddArrayBufferData(indexAttrib, (const float*) data, dataSize*3, 3);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const glm::vec4* data, unsigned int dataSize)
{
	AddArrayBufferData(indexAttrib, (const float*) data, dataSize*4, 4);
}

//float <vector> GLM::VECx
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<float> &data, unsigned int dataSize, GLuint vertexSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const float*) (&data[0]), dataSize, vertexSize);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<glm::vec2> &data, unsigned int dataSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const float*) (&data[0].x), dataSize*2, 2);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<glm::vec3> &data, unsigned int dataSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const float*) (&data[0].x), dataSize*3, 3);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<glm::vec4> &data, unsigned int dataSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const float*) (&data[0].x), dataSize*4, 4);
}

/****************int ***********************************************/
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const int* data, unsigned int dataSize, GLint vertexSize)
{	


	tArrayBuffer.push_back(ArrayBuffer());
	tArrayBuffer.back().nbAttrib = 1;
	tArrayBuffer.back().data = data;
	tArrayBuffer.back().size = dataSize*sizeof(int);
	tArrayBuffer.back().tAttribIndex.push_back(indexAttrib);
	tArrayBuffer.back().tAttribOffsetStart.push_back(0);
	tArrayBuffer.back().tAttribSize.push_back(vertexSize);
	tArrayBuffer.back().tAttribType.push_back(GL_INT);
	
}
void RxVBO::AddStructArrayBufferData(GLuint nbAttrib, std::vector<GLuint> &tAttribIndex, std::vector<GLenum> &tAttribType, std::vector<GLuint> &tAttribSize, std::vector<GLuint> &tAttribOffsetStart, GLuint stride, GLuint size, const void *data, std::vector<unsigned int> &tAttribInType )
{
	unsigned int *ptAttribInType = NULL;
	if(tAttribInType.size())
	  ptAttribInType = &tAttribInType[0];

	AddStructArrayBufferData(nbAttrib,&tAttribIndex[0],&tAttribType[0],&tAttribSize[0],&tAttribOffsetStart[0],stride,size,data,ptAttribInType);
}

void RxVBO::AddStructArrayBufferData(GLuint nbAttrib, GLuint *ptAttribIndex, GLenum *ptAttribType, GLuint *ptAttribSize, GLuint *ptAttribOffsetStart, GLuint stride, GLuint size, const void *data, unsigned int *ptAttribInType )
{	
	tArrayBuffer.push_back(ArrayBuffer());
	
	tArrayBuffer.back().nbAttrib = nbAttrib;
	tArrayBuffer.back().size = size;
	tArrayBuffer.back().data = data;	
	tArrayBuffer.back().stride = stride;

	tArrayBuffer.back().tAttribIndex.resize(nbAttrib);
	tArrayBuffer.back().tAttribType.resize(nbAttrib);
	tArrayBuffer.back().tAttribSize.resize(nbAttrib);
	tArrayBuffer.back().tAttribOffsetStart.resize(nbAttrib);
	tArrayBuffer.back().tAttribInType.resize(nbAttrib);


	for(unsigned int i = 0; i < nbAttrib; i++)
	{
		tArrayBuffer.back().tAttribIndex[i] = ptAttribIndex[i];
		tArrayBuffer.back().tAttribType[i] = ptAttribType[i];
		tArrayBuffer.back().tAttribSize[i] = ptAttribSize[i];
		tArrayBuffer.back().tAttribOffsetStart[i] = ptAttribOffsetStart[i];
		if(ptAttribInType)
		tArrayBuffer.back().tAttribInType[i] = ptAttribInType[i];
	}


}


//int GLM::iVECx
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const glm::ivec2* data, unsigned int dataSize)
{
	AddArrayBufferData(indexAttrib, (const int*) data, dataSize*2, 2);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const glm::ivec3* data, unsigned int dataSize)
{
	AddArrayBufferData(indexAttrib, (const int*) data, dataSize*3, 3);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, const glm::ivec4* data, unsigned int dataSize)
{
	AddArrayBufferData(indexAttrib, (const int*) data, dataSize*4, 4);
}

//int <vector> GLM::iVECx
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<int> &data, unsigned int dataSize, GLuint vertexSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const int*) (&data[0]), dataSize, vertexSize);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<glm::ivec2> &data, unsigned int dataSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const int*) (&data[0].x), dataSize*2, 2);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<glm::ivec3> &data, unsigned int dataSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const int*) (&data[0].x), dataSize*3, 3);
}
void RxVBO::AddArrayBufferData(GLuint indexAttrib, std::vector<glm::ivec4> &data, unsigned int dataSize)
{
	if(!dataSize) dataSize = data.size();
	AddArrayBufferData(indexAttrib, (const int*) (&data[0].x), dataSize*4, 4);


}




//element
void RxVBO::AddElementBufferData(const unsigned int* data, unsigned int dataSize)
{
	ElementBuffer tmp;
	tmp.dataUIntSize = dataSize;
	tmp.data = data;
	tElementBuffer.push_back(tmp);
}
