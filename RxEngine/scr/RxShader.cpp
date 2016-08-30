
//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#




#include "RxShader.h"
RxShader *RxShader::bindShader  = NULL;

RxShader::RxShader():
programID(0), vertexID(0) ,fragmentID(0)
{
}

RxShader::RxShader(const char *vertexSource, const char *fragmentSource):
programID(0), vertexID(0) ,fragmentID(0)
{
	Load(vertexSource, fragmentSource);
}
RxShader::~RxShader()
{
	if(bindShader && (bindShader->getGlProgramID() == programID))
		bindShader = NULL;
	Destroy();
}
	
int RxShader::Load(const char *vertexSource, const char *fragmentSource)
{


   // Variables
    GLint link = 0, errorLength= 0;


    // Création des shaders

    if(LoadTypeShader(vertexID, GL_VERTEX_SHADER, vertexSource) == RX_ERROR)
        return RX_ERROR;

    if(LoadTypeShader(fragmentID, GL_FRAGMENT_SHADER, fragmentSource) == RX_ERROR)
        return RX_ERROR;

    // Création du programme

    programID = glCreateProgram();

    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);


    // Bindage des variables in/out et linkage
	BindAllAttribLocation();

    glLinkProgram(programID);

    // On vérifie le succès du linkage

    glGetProgramiv(programID, GL_LINK_STATUS, &link);

    if(link != GL_TRUE)
    {
        // Si le linkage a échoué, on récupère l'erreur puis on l'affiche

        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorLength);

		error.resize(errorLength +1, '\0');

        glGetProgramInfoLog(programID, errorLength, &errorLength, (GLchar*)error.data());

        RXLOGGERERROR("[RxShader][Load] LINK FAIL : "<<error);

        return RX_ERROR;
    }


	BindUniformDefault();


	#ifdef _DEBUG
	RXCHECKGLERROR("[RxShader][Load]");
	#endif

	RXLOGGER("[RxShader][Load]"<<"Shader -> [VERT] : "<<vertexSource<<" -> [FRAG] : "<<fragmentSource);
    return RX_OK;
}

 

int RxShader::LoadTypeShader(GLuint &shader, GLenum type, const char *source)
{
   // Variables

    int bufferLenght = 0;
    char *buffer;
    GLint errorCompilation = 0, errorLenght = 0;



    // Création du shader
    shader = glCreateShader(type);


    // On ouvre le fichier du code source

	 std::ifstream file(source, std::ios::in);


    // On verifie que le fichier existe

    if(!file.is_open())
    {
        RXLOGGERERROR("[RxShader][LoadTypeShader] File no exist : "<<source);
        return RX_ERROR;
    }


    // On détermine la taille du fichier

  // get length of file:
	file.seekg (0, std::ios::end); //positionne le pointeur à la fin du fichier
	bufferLenght = (int)file.tellg(); //position du pointeur
	file.seekg (0, std::ios::beg); //positionne le pointeur au début

    if(bufferLenght <= 1)
    {
        RXLOGGERERROR("[RxShader][LoadTypeShader] File Empty : "<<source);
        return RX_ERROR;
    }


    // On alloue un tableau pour accueillir le code source du shader
	buffer = new char[bufferLenght+1];
	for(int i=0; i < bufferLenght+1; i++)
		buffer[i] = '\0';


	//on lit le code shader
	file.read(buffer, bufferLenght);
  
	//fermeture du fichier
	file.close();

    // Compilation du shader
    glShaderSource(shader, 1, (const GLchar**) &buffer, NULL);
    glCompileShader(shader);


	delete[] buffer; //suppression des réssources
     
    

    // On test la compilation

    glGetShaderiv(shader, GL_COMPILE_STATUS, &errorCompilation);

    if(errorCompilation != GL_TRUE)
    {
        // Si la compilation a échouée, on récupère l'erreur puis on l'affiche
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLenght);

		error.resize(errorLenght+1, '\0');
        glGetShaderInfoLog(shader, errorLenght, &errorLenght, (GLchar*)error.data());
        

		RXLOGGERERROR("[RxShader][LoadTypeShader] -> File : "<<source<<" -- error : "<<error);
	
        return RX_ERROR;
    }

	


	#ifdef _DEBUG
	RXCHECKGLERROR("[RxShader][LoadTypeShader]");
	#endif

    // Si tout c'est bien passé, on renvoie 0
    return RX_OK;
    
}

void RxShader::BindUniformDefault()
{
	defaultUniform[RX_PROJECTION_MATRIX_ID] = glGetUniformLocation (programID,RX_PROJECTION_MATRIX);
	defaultUniform[RX_MODELVIEW_MATRIX_ID] = glGetUniformLocation (programID,RX_MODELVIEW_MATRIX);
	defaultUniform[RX_TRANSFORM_MATRIX_ID] = glGetUniformLocation (programID,RX_TRANSFORM_MATRIX);
}

void RxShader::BindAttribLocation(GLuint index, const GLchar* name)
{
	RxAttrib new_attrib;
	new_attrib.index = index;
	new_attrib.name = name;
	tab_attribs.push_back(new_attrib);
}

void RxShader::BindAttribLocationDefault()
{
	RxAttrib tmp;
	tmp.index = RX_VERTEX_ATTRIB_INDEX;
	tmp.name = RX_VERTEX_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_COLOR_ATTRIB_INDEX;
	tmp.name = RX_COLOR_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_COORD0_ATTRIB_INDEX;
	tmp.name = RX_COORD0_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_NORMAL_ATTRIB_INDEX;
	tmp.name = RX_NORMAL_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_COORD1_ATTRIB_INDEX;
	tmp.name = RX_COORD1_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_COORD2_ATTRIB_INDEX;
	tmp.name = RX_COORD2_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_DATA0_ATTRIB_INDEX;
	tmp.name = RX_DATA0_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_DATA1_ATTRIB_INDEX;
	tmp.name = RX_DATA1_ATTRIB_NAME;
	tab_attribs.push_back(tmp);

	tmp.index = RX_DATA2_ATTRIB_INDEX;
	tmp.name = RX_DATA2_ATTRIB_NAME;
	tab_attribs.push_back(tmp);
}

void RxShader::BindAllAttribLocation()
{
	if(tab_attribs.empty())
	{
		BindAttribLocationDefault();
	#ifdef _DEBUG
		RXLOGGERWARNING("[RxShader][BindAllAttribLocation] default");
	#endif
	}

	unsigned int i;
	for(i = 0; i < tab_attribs.size(); i++)
		glBindAttribLocation(programID, tab_attribs[i].index, (GLchar *)tab_attribs[i].name.data());

}
void RxShader::Enable()
{

	#ifdef _DEBUG
	if(programID == 0)
		RXLOGGERERROR("[RxShader][Enable] programID == 0");
	#endif

	bindShader = this;
	glUseProgram(programID) ;
}
void RxShader::Disable()
{
	bindShader = NULL;
	glUseProgram(0);
}
		
bool RxShader::IsValid()
{
	return (programID > 0);
}

void RxShader::Destroy()
{
	glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    glDeleteProgram(programID);
}


std::string& RxShader::getError()
{
	return error;
}

GLuint RxShader::getGlProgramID()
{
	return programID;
}

void RxShader::SendProjection(const glm::mat4 & m)
{
		glUniformMatrix4fv
		(
			defaultUniform[RX_PROJECTION_MATRIX_ID],
 	 		1,
 	  		0,
 			&m[0][0]
		);
}
void RxShader::SendModelView(const glm::mat4 & m)
{
		glUniformMatrix4fv
		(
			defaultUniform[RX_MODELVIEW_MATRIX_ID],
 	 		1,
 	  		0,
 			&m[0][0]
		);
}
void RxShader::SendTransform(const glm::mat4 & m)
{
		glUniformMatrix4fv
		(
			defaultUniform[RX_TRANSFORM_MATRIX_ID],
 	 		1,
 	  		0,
 			&m[0][0]
		);
}

//-----------------------------------------------------------------------------
// Send1i/Send1f
//-----------------------------------------------------------------------------
void RxShader::Send1i(const GLchar* name,int a)
{
	glUniform1i(glGetUniformLocation (programID,name), a);
}
void RxShader::Send1f(const GLchar* name,float a)
{
	glUniform1f(glGetUniformLocation(programID,name), a);
}

//-----------------------------------------------------------------------------
// Send2i/Send2f
//-----------------------------------------------------------------------------
void RxShader::Send2i(const GLchar* name,int a,int b)
{
	glUniform2i(glGetUniformLocation (programID,name), a, b);
}
void RxShader::Send2f(const GLchar* name,float a,float b)
{
	glUniform2f(glGetUniformLocation(programID,name), a, b);
}

//-----------------------------------------------------------------------------
// Send3i/Send3f
//-----------------------------------------------------------------------------
void RxShader::Send3i(const GLchar*name,int a,int b,int c)
{
	glUniform3i(glGetUniformLocation (programID,name), a, b, c);
}

void RxShader::Send3f(const GLchar* name,float a,float b,float c)
{
	glUniform3f(glGetUniformLocation (programID,name), a, b, c);
}

//-----------------------------------------------------------------------------
// Send4i/Send4f
//-----------------------------------------------------------------------------
void RxShader::Send4i(const GLchar* name,int a,int b,int c, int d)
{
	glUniform4i(glGetUniformLocation (programID,name), a, b, c, d);
}
void RxShader::Send4f(const GLchar* name,float a,float b,float c,float d)
{
	glUniform4f(glGetUniformLocation (programID,name), a, b, c, d);
}
//-----------------------------------------------------------------------------
// SendMat4f
//-----------------------------------------------------------------------------
void RxShader::SendMat4f(const GLchar* name, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4fv
		(
			glGetUniformLocation (programID,name),
 	 		1,
 	  		transpose,
 			value
		);

}

#ifdef _RXMATH3D_H_

//-----------------------------------------------------------------------------
// Send2i/Send2f
//-----------------------------------------------------------------------------
void RxShader::Send2i(const GLchar* name,const glm::ivec2 &v)
{
	glUniform2iv(glGetUniformLocation (programID,name),1, &v[0]);
}
void RxShader::Send2f(const GLchar* name,const glm::vec2 &v)
{
	glUniform2fv(glGetUniformLocation (programID,name),1, &v[0]);
}

//-----------------------------------------------------------------------------
// Send3i/Send3f
//-----------------------------------------------------------------------------
void RxShader::Send3i(const GLchar* name,const glm::ivec3 &v)
{
	glUniform3iv(glGetUniformLocation (programID,name),1, &v[0]);
}
void RxShader::Send3f(const GLchar* name,const glm::vec3 &v)
{
	glUniform3fv(glGetUniformLocation (programID,name),1, &v[0]);
}

//-----------------------------------------------------------------------------
// Send4i/Send4f
//-----------------------------------------------------------------------------
void RxShader::Send4i(const GLchar* name,const glm::ivec4 &v)
{
	glUniform4iv(glGetUniformLocation (programID,name),1, &v[0]);
}
void RxShader::Send4f(const GLchar* name,const glm::vec4 &v)
{
	glUniform4fv(glGetUniformLocation (programID,name),1, &v[0]);
}

//-----------------------------------------------------------------------------
// SendMat4f
//-----------------------------------------------------------------------------
void RxShader::SendMat4f(const GLchar* name, GLboolean transpose, const glm::mat4& m)
{
	glUniformMatrix4fv
		(
			glGetUniformLocation (programID,name),
 	 		1,
 	  		transpose,
 			 glm::value_ptr(m)
		);

}
//-----------------------------------------------------------------------------
// SendMat4fv
//----------------------------------------------------------------------------
void RxShader::SendMat4f(const GLchar* name, GLboolean transpose, const std::vector<glm::mat4> & m)
{
	int id = glGetUniformLocation (programID,name);
#ifdef _DEBUG
	if(id == -1)
	{
		RXLOGGERERROR("[RxShader]{SendMat4f] glGetUniformLocation fail : "<<name);
		return ;
	}
#endif
	glUniformMatrix4fv(id,m.size(),transpose, glm::value_ptr(m[0]));
}


#endif