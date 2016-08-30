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

#include "Shader.hpp"
#include "Common.hpp"
#include "Assertion.hpp"
#include "Logger.hpp"
#include "GLErrorLogger.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>


namespace BH3D
{

	Shader *Shader::usedShader = nullptr;
	
	Shader::Shader()
	{

	}
	Shader::Shader(const File & vertexSource, const File & fragmentSource) :
		programID(0), vertexID(0), fragmentID(0)
	{
		Load(vertexSource, fragmentSource);
	}
	Shader::~Shader()
	{
		if (usedShader && (usedShader->GetGLProgramID() == programID))
			usedShader = nullptr;
		Destroy();
	}

	Shader *Shader::CurrentBindShader()
	{
		return usedShader;
	}

	int Shader::Load(const File & vertexSource, const File & fragmentSource)
	{
		// Variables
		GLint link = 0, errorLength = 0;

		// Création des shaders
		if (LoadTypeShader(vertexID, GL_VERTEX_SHADER, vertexSource) == BH3D_ERROR)
		{
			return BH3D_ERROR;
		}

		if (LoadTypeShader(fragmentID, GL_FRAGMENT_SHADER, fragmentSource) == BH3D_ERROR)
		{
			return BH3D_ERROR;
		}

		// Création du programme
		programID = glCreateProgram();

		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);

		// Bindage des variables in/out et linkage
		BindAllAttribLocation();

		glLinkProgram(programID);

		// On vérifie le succès du linkage

		glGetProgramiv(programID, GL_LINK_STATUS, &link);

		if (link != GL_TRUE)
		{
			// Si le linkage a échoué, on récupère l'erreur puis on l'affiche

			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &errorLength);

			error.resize(errorLength + 1, '\0');

			glGetProgramInfoLog(programID, errorLength, &errorLength, (GLchar*)error.data());

			BH3D_LOGGERERROR(" Shader link fail : " << error);
			
			return BH3D_ERROR;
		}


		DefaultUniformLocation();


		BH3D_CHECKGLERROR();


		BH3D_LOGGER("Shader -> [VERT] : " << vertexSource.Filename() << " -> [FRAG] : " << fragmentSource.Filename());
		return BH3D_OK;
	}


	int Shader::LoadTypeShader(GLuint &shader, GLenum type, const File & source)
	{

		GLint errorCompilation = 0, errorLenght = 0;

		// Création du shader
		shader = glCreateShader(type);

		//lecture du fichier
		std::string buffer;
		source.ReadAndSaveIn(buffer);
		const char * c_str = buffer.c_str();


		// Compilation du shader
		glShaderSource(shader, 1, &c_str, nullptr);
		glCompileShader(shader);


		// On test la compilation

		glGetShaderiv(shader, GL_COMPILE_STATUS, &errorCompilation);

		if (errorCompilation != GL_TRUE)
		{
			// Si la compilation a échouée, on récupère l'erreur puis on l'affiche
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLenght);

			error.resize(errorLenght + 1, '\0');
			glGetShaderInfoLog(shader, errorLenght, &errorLenght, (GLchar*)error.data());

			BH3D_LOGGERERROR("Compilation Shader Fail -> File : " << source.Filename() << " -- error : " << error);
			return BH3D_ERROR;
		}

		BH3D_CHECKGLERROR();

		// Si tout c'est bien passé, on renvoie 0
		return BH3D_OK;

	}

	void Shader::DefaultUniformLocation()
	{
		defaultUniform[BH3D_PROJECTION_ID] = glGetUniformLocation(programID, BH3D_UNIFORM_PROJECTION_NAME);
		defaultUniform[BH3D_MODELVIEW_ID] = glGetUniformLocation(programID, BH3D_UNIFORM_MODELVIEW_NAME);
		defaultUniform[BH3D_TRANSFORM_ID] = glGetUniformLocation(programID, BH3D_UNIFORM_TRANSFORM_NAME);
	}

	void Shader::AddAttribLocation(GLuint index, const GLchar* name)
	{
		Attrib new_attrib;
		new_attrib.index = index;
		new_attrib.name = name;
		tAttribs.push_back(new_attrib);
	}

	void Shader::AddAttribLocationDefault()
	{
		Attrib tmp;
		tmp.index = BH3D_VERTEX_ATTRIB_INDEX;
		tmp.name = BH3D_VERTEX_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_COLOR_ATTRIB_INDEX;
		tmp.name = BH3D_COLOR_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_COORD0_ATTRIB_INDEX;
		tmp.name = BH3D_COORD0_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_NORMAL_ATTRIB_INDEX;
		tmp.name = BH3D_NORMAL_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_COORD1_ATTRIB_INDEX;
		tmp.name = BH3D_COORD1_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_COORD2_ATTRIB_INDEX;
		tmp.name = BH3D_COORD2_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_DATA0_ATTRIB_INDEX;
		tmp.name = BH3D_DATA0_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_DATA1_ATTRIB_INDEX;
		tmp.name = BH3D_DATA1_ATTRIB_NAME;
		tAttribs.push_back(tmp);

		tmp.index = BH3D_DATA2_ATTRIB_INDEX;
		tmp.name = BH3D_DATA2_ATTRIB_NAME;
		tAttribs.push_back(tmp);
	}

	void Shader::BindAllAttribLocation()
	{
		if (tAttribs.empty())
		{
			AddAttribLocationDefault();

			BH3D_LOGGERWARNING("Default attribut names");

		}

		unsigned int i;
		for (i = 0; i < tAttribs.size(); i++)
			glBindAttribLocation(programID, tAttribs[i].index, (GLchar *)tAttribs[i].name.data());

	}
	

}