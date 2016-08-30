//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxTools.h"


void Rx_Repere3D()
{
   
	static RxVBO vbo;

	if(!vbo.IsValide())
	{
		const float vertex[] = {0.0,0.0,0.0,   1.0,0.0,0.0,  0.0,0.0,0.0,   0.0,1.0,0.0,   0.0,0.0,0.0,  0.0,0.0,1.0};
		const float color[] = {1.0,0.0,0.0,   1.0,0.0,0.0,  0.0,1.0,0.0,   0.0,1.0,0.0,   0.0,0.0,1.0,  0.0,0.0,1.0};
		vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,vertex,18);
		vbo.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX ,color,18);
		vbo.Create();
	}

	vbo.Enable();
		glDrawArrays(GL_LINES, 0, 18);
	vbo.Disable();
  
}

void Rx_Repere2D()
{
	static RxVBO vbo;

	if(!vbo.IsValide())
	{
		const float vertex[] = {0.0, 0.0,0.0,   1.0,0.0,0.0,  0.0,0.0,0.0,   0.0, 1.0,0.0};
		const float color[] = {1.0,0.0,0.0,   1.0,0.0,0.0,  0.0,1.0,0.0,   0.0,1.0,0.0};
		vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,vertex,12);
		vbo.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX ,color,12);
		vbo.Create();
	}
	vbo.Enable();
		glDrawArrays(GL_LINES, 0, 8);
	vbo.Disable();

}


void RxSetSDLPixel(SDL_Surface *surface, int x, int y, Uint32 color)
{

  *((Uint32*)(surface->pixels) + x + y * surface->w) = color;

}

Uint32 RxGetSDLPixel(SDL_Surface *surface, int x, int y)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /* Ici p est l'adresse du pixel que l'on veut connaitre */
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        /*Ne devrait pas arriver, mais évite les erreurs*/
        default:
            return 0; 
    }
}




GLenum RxCheckGLError( int _error)
{
	GLenum error;
	if(_error == -1)
		error = glGetError();
	else
		error = _error;

	switch(error)
	{
		case GL_NO_ERROR:
			RXLOGGER("[CHECK_GL_ERROR] none ");
			break;
		case GL_INVALID_ENUM:
			RXLOGGERERROR("[CHECK_GL_ERROR] INVALID_ENUM ");
			break;
		case GL_INVALID_VALUE:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_INVALID_VALUE ");
			break;
		case GL_INVALID_OPERATION:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_INVALID_OPERATION ");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_INVALID_FRAMEBUFFER_OPERATION ");
			break;
		case GL_OUT_OF_MEMORY:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_OUT_OF_MEMORY ");
			break;
	}
	return error;
}


#ifdef _DEBUG

GLenum RxCheckGLError(const char* file, int line , const char *fct)
{
	GLenum error;
	error = glGetError();
	switch(error)
	{
		case GL_NO_ERROR:
			//RXLOGGERGLERROR("[CHECK_GL_ERROR] NONE ");
			break;
		case GL_INVALID_ENUM:
			RXLOGGERGLERROR("[CHECK_GL_ERROR] INVALID_ENUM ");
			break;
		case GL_INVALID_VALUE:
			RXLOGGERGLERROR("[CHECK_GL_ERROR] GL_INVALID_VALUE ");
			break;
		case GL_INVALID_OPERATION:
			RXLOGGERGLERROR("[CHECK_GL_ERROR] GL_INVALID_OPERATION ");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			RXLOGGERGLERROR("[CHECK_GL_ERROR] GL_INVALID_FRAMEBUFFER_OPERATION ");
			break;
		case GL_OUT_OF_MEMORY:
			RXLOGGERGLERROR("[CHECK_GL_ERROR] GL_OUT_OF_MEMORY ");
			break;
	}
	return error;
}


#else
GLenum RxCheckGLError()
{
	GLenum error;
	error = glGetError();
	switch(error)
	{
		case GL_NO_ERROR:
			RXLOGGER("[CHECK_GL_ERROR] none ");
			break;
		case GL_INVALID_ENUM:
			RXLOGGERERROR("[CHECK_GL_ERROR] INVALID_ENUM ");
			break;
		case GL_INVALID_VALUE:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_INVALID_VALUE ");
			break;
		case GL_INVALID_OPERATION:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_INVALID_OPERATION ");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_INVALID_FRAMEBUFFER_OPERATION ");
			break;
		case GL_OUT_OF_MEMORY:
			RXLOGGERERROR("[CHECK_GL_ERROR] GL_OUT_OF_MEMORY ");
			break;
	}
	return error;
}

#endif	


void Rx_CubeTest()
{
	
	static RxVBO vbo;

	if(!vbo.IsValide())
	{
		/****************** description d'un cube *************************/
		const float vertex[] = 
		{ 
		  -1.0f,-1.0f,-1.0f, //0              7__________6
		   1.0f,-1.0f,-1.0f, //1			 /|	        /|	
		   1.0f,-1.0f, 1.0f, //2		    / |	       / |
		  -1.0f,-1.0f, 1.0f, //3		  4/__|______ /5 |
		  -1.0f, 1.0f,-1.0f, //4		   | 3|______|___|2
		   1.0f, 1.0f,-1.0f, //5           |  /      |  /
		   1.0f, 1.0f, 1.0f, //6           | /       | /
		  -1.0f, 1.0f, 1.0f  //7          0|/________|/1
		};
		/****************** description d'un cube *************************/
		const float color[] = 
		{ 
		   0.0f, 0.0f, 1.0f, //0              7__________6
		   0.0f, 1.0f, 0.0f, //1			 /|	        /|	
		   0.0f, 1.0f, 1.0f, //2		    / |	       / |
		   1.0f, 0.0f, 0.0f, //3		  4/__|______ /5 |
		   1.0f, 0.0f, 1.0f, //4		   | 3|______|___|2
		   1.0f, 1.0f, 0.0f, //5           |  /      |  /
		   0.0f, 0.0f, 1.0f, //6           | /       | /
		   0.0f, 1.0f, 0.0f  //7          0|/________|/1
		};

		const float texture[]=
		{
			0.0f, 0.0f, //0
			1.0f, 0.0f, //1
			1.0f, 1.0f, //2
			0.0f, 1.0f, //3
			0.0f, 1.0f, //4
			1.0f, 1.0f, //5
			1.0f, 0.0f, //6
			0.0f, 0.0f  //7
		};

		const unsigned int faces[] =
		{
			4,1,0 , 4,5,1, //front
			5,2,1 , 5,6,2, //right
			6,3,2 , 6,7,3, //back
			7,0,3 , 7,4,0, //left
			0,2,3 , 0,1,2, //top
			6,4,7 , 6,5,4  //bottom 
		};

	/*	const float normal[] = 
		{
		  -1.732050807f,-1.732050807f,-1.732050807f, //0         
		   1.732050807f,-1.732050807f,-1.732050807f, //1			
		   1.732050807f,-1.732050807f, 1.732050807f, //2		 
		  -1.732050807f,-1.732050807f, 1.732050807f, //3	
		  -1.732050807f, 1.732050807f,-1.732050807f, //4		
		   1.732050807f, 1.732050807f,-1.732050807f, //5     
		   1.732050807f, 1.732050807f, 1.732050807f, //6      
		  -1.732050807f, 1.732050807f, 1.732050807f  //7       
		};

		const float texture[]=
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};*/

		vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,vertex,28);
		vbo.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX ,color,28);
		vbo.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX ,texture,16,2);
		vbo.AddElementBufferData(faces,36);
		vbo.Create();


	}
	vbo.Enable();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);
	vbo.Disable();
}



///////fonction de convertion//////////////////////

//fonctions de conversion

glm::vec3 str2vec2(const char *svec)
{
	glm::vec3 v;

	sscanf_s(svec,"%f%*c%f%",&v.x,&v.y,&v.z);
	return v;
}
glm::vec2 str2vec2(std::string &svec)
{
	glm::vec2 v;

	sscanf_s(svec.data(),"%f%*c%f%",&v.x,&v.y);
	return v;
}

glm::vec3 str2vec3(const char *svec)
{
	glm::vec3 v;

	sscanf_s(svec,"%f%*c%f%*c%f",&v.x,&v.y,&v.z);
	return v;
}
glm::vec3 str2vec3(std::string &svec)
{
	glm::vec3 v;

	sscanf_s(svec.data(),"%f%*c%f%*c%f",&v.x,&v.y,&v.z);
	return v;
}

glm::ivec2 str2ivec2(const char *svec)
{
	glm::ivec2 v;

	sscanf_s(svec,"%d%*c%d%",&v.x,&v.y);
	return v;
}
glm::ivec2 str2ivec2(std::string &svec)
{
	glm::ivec2 v;

	sscanf_s(svec.data(),"%d%*c%d%",&v.x,&v.y);
	return v;
}

glm::vec4 str2vec4(const char *svec)
{
	glm::vec4 v;

	sscanf_s(svec,"%f%*c%f%*c%f%*c%f",&v.x,&v.y,&v.z,&v.w);
	return v;
}
glm::vec4 str2vec4(std::string &svec)
{
	glm::vec4 v;
	
	sscanf_s(svec.data(),"%f%*c%f%*c%f%*c%f",&v.x,&v.y,&v.z,&v.w);
	return v;
}

/*
glm::quat str2quat(string &squat)
{
	glm::quat v; 
	
	sscanf_s(squat.data(),"%f%*c%f%*c%f%*c%f",&v.x,&v.y,&v.z,&v.w);
	return v;
}
glm::quat str2quat(const char* squat)
{
	glm::quat v; 
	
	sscanf_s(squat,"%f%*c%f%*c%f%*c%f",&v.x,&v.y,&v.z,&v.w);
	return v;
}*/

std::vector<unsigned int> str2vui(const char* svui)
{
	std::vector<unsigned int> vui;
	std::string ssvui = svui;
	
	const char *tmp ;
	unsigned int d,nb_c = 0;
	while(nb_c < ssvui.length() )
	{
		sscanf_s(svui+nb_c,"%d",&d);
		tmp = strchr ( svui+nb_c,' ');
		nb_c =  tmp- svui+1;
		vui.push_back(d);
	}
	return vui;
}

std::vector<unsigned int> str2vui(std::string &svui)
{
	std::vector<unsigned int> vui;
	
	
	const char *tmp ;
	char *csvui = &svui[0];
	unsigned int d,nb_c = 0;
	while(nb_c < svui.length() )
	{
		sscanf_s(csvui+nb_c,"%d",&d);
		tmp = strchr ( csvui+nb_c,' ');
		nb_c =  tmp- csvui+1;
		vui.push_back(d);
	}
	return vui;
}

std::vector<unsigned int> xml2gl(std::vector<unsigned int> idxml, std::vector<idXML2idGL> switchtab)
{
	unsigned int i;
	std::vector<unsigned int> idgl;

	for(i = 0; i < idxml.size(); i++ ) //pour l'ensemble des id xml
	{	
			//recherche la valeur 
			if(idxml[i] == 0){  idgl.push_back(0); continue; }
			std::vector<idXML2idGL>::iterator it = std::find (switchtab.begin(), switchtab.end(), idxml[i]);
			
			if(it != switchtab.end())
				idgl.push_back(it->idgl); //assigne la valeur opengl
			else idgl.push_back(0);
	}	

	return idgl;
}

unsigned int xml2gl(unsigned int idxml, std::vector<idXML2idGL> switchtab)
{
			if(idxml == 0 ) return 0;
			//recherche la valeur 
			std::vector<idXML2idGL>::iterator it = std::find (switchtab.begin(), switchtab.end(), idxml);
			if(it != switchtab.end())
				return it->idgl; //assigne la valeur opengl
			else return 0;
			
}
std::vector<glm::vec3> str2vectorvec3(const char * str)
{
	std::vector<float> data;
	data.clear();
	std::string svvec = str;
	
	const char *tmp ;
	char *csvui = &svvec[0];
	unsigned int nb_c = 0;
	float value;
	while(nb_c < svvec.length() )
	{
		sscanf_s(csvui+nb_c,"%f",&value);
		tmp = strchr ( csvui+nb_c,' ');
		nb_c =  tmp- csvui+1;
		data.push_back(value);
	}

	std::vector<glm::vec3> datavec3;datavec3.clear();
	for(unsigned int i = 0; i < data.size()/3; i++)
		datavec3.push_back(glm::vec3(data[i*3],data[i*3+1],data[i*3+2]));


	return datavec3;
}
std::vector<glm::vec3> str2vectorvec3(std::string &svvec)
{
	std::vector<float> data;
	data.clear();

	
	const char *tmp ;
	char *csvui = &svvec[0];
	unsigned int nb_c = 0;
	float value;
	while(nb_c < svvec.length() )
	{
		sscanf_s(csvui+nb_c,"%f",&value);
		tmp = strchr ( csvui+nb_c,' ');
		nb_c =  tmp- csvui+1;
		data.push_back(value);
	}

	std::vector<glm::vec3> datavec3;
	for(unsigned int i = 0; i < data.size()/3; i++)
		datavec3.push_back(glm::vec3(data[i*3],data[i*3+1],data[i*3+2]));


	return datavec3;
}

std::vector<float> str2vectorfloat(const char * str)
{
	std::vector<float> data;

	std::string svvec = str;
	
	const char *tmp ;
	char *csvui = &svvec[0];
	unsigned int nb_c = 0;
	float value;
	while(nb_c < svvec.length() )
	{
		sscanf_s(csvui+nb_c,"%f",&value);
		tmp = strchr ( csvui+nb_c,' ');
		nb_c =  tmp- csvui+1;
		data.push_back(value);
	}

	return data;
}
std::vector<float> str2vectorfloat(std::string &svvec)
{
	std::vector<float> data;

	
	const char *tmp ;
	char *csvui = &svvec[0];
	unsigned int nb_c = 0;
	float value;
	while(nb_c < svvec.length() )
	{
		sscanf_s(csvui+nb_c,"%f",&value);
		tmp = strchr ( csvui+nb_c,' ');
		nb_c =  tmp- csvui+1;
		data.push_back(value);
	}


	return data;
}

std::vector<int> str2vectorint(std::string &sint)
{
	std::vector<int> data;
	const char *tmp ;
	char *csvui = &sint[0];
	unsigned int nb_c = 0;
	int value;
	while(nb_c < sint.length() )
	{
		sscanf_s(csvui+nb_c,"%d",&value);
		tmp = strchr ( csvui+nb_c,' ');
		nb_c =  tmp- csvui+1;
		data.push_back(value);
	}
	return data;
}

