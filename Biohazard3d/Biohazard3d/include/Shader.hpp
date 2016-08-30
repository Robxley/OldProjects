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


#ifndef _BH3D_SHADER_H_
#define _BH3D_SHADER_H_


#ifdef __ANDROID__
#include <SDL.h>  //utilisation de la fonction SDL_RWFromFile pour ouvrir un fichier
#endif

#include <SDL.h>  //utilisation de la fonction SDL_RWFromFile pour ouvrir un fichier

//Attribution par defaut
#define BH3D_VERTEX_ATTRIB_INDEX 0
#define BH3D_COLOR_ATTRIB_INDEX  1
#define BH3D_COORD0_ATTRIB_INDEX 2
#define BH3D_NORMAL_ATTRIB_INDEX 3
#define BH3D_COORD1_ATTRIB_INDEX 4
#define BH3D_COORD2_ATTRIB_INDEX 5
#define BH3D_DATA0_ATTRIB_INDEX  6
#define BH3D_DATA1_ATTRIB_INDEX  7
#define BH3D_DATA2_ATTRIB_INDEX  8

#define BH3D_VERTEX_ATTRIB_NAME  "in_Vertex"
#define BH3D_COLOR_ATTRIB_NAME   "in_Color"
#define BH3D_COORD0_ATTRIB_NAME  "in_Coord0"
#define BH3D_NORMAL_ATTRIB_NAME  "in_Normal"
#define BH3D_COORD1_ATTRIB_NAME  "in_Coord1"
#define BH3D_COORD2_ATTRIB_NAME  "in_Coord2"
#define BH3D_DATA0_ATTRIB_NAME   "in_Data0"
#define BH3D_DATA1_ATTRIB_NAME   "in_Data1"
#define BH3D_DATA2_ATTRIB_NAME   "in_Data2"

#define BH3D_NUM_DEFAULT_UNIFORM 3

#define BH3D_UNIFORM_PROJECTION_NAME "projection"
#define BH3D_UNIFORM_MODELVIEW_NAME "modelview"
#define BH3D_UNIFORM_TRANSFORM_NAME "transform"

#define BH3D_PROJECTION_ID 0
#define BH3D_MODELVIEW_ID 1
#define BH3D_TRANSFORM_ID 2

#include "MultiPlatformOpenGL.hpp"
#include "File.hpp"
#include "Assertion.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace BH3D
{
	class Shader
	{


	public:
		//attribut
		typedef struct attrib_struct
		{
			GLuint index = 0;
			std::string name;
		} Attrib;

		std::vector<Attrib> tAttribs;

		static Shader *CurrentBindShader();

		unsigned int defaultUniform[BH3D_NUM_DEFAULT_UNIFORM];
		
	public:

		Shader();
		Shader(const File & vertexSource, const File & fragmentSource);
		~Shader();

		//chargement du vertex shader et fragment
		int Load(const File & vertexSource, const File & fragmentSource);

		//bool LoadRWFromFile(const char *filename, std::string dataRW);

		//Activation shader
		inline void Enable();

		//Desactivation shader
		inline void Disable();


		//attribut name			-> index 
		void AddAttribLocation(GLuint index, const GLchar* name);

		//attribution -> voir #define
		virtual void AddAttribLocationDefault();


		virtual void DefaultUniformLocation();

		//retourne l'info sur programID
		inline bool IsValid();

		//retourne la référence sur error
		inline const std::string& GetError();

		inline GLuint GetGLProgramID();


		//Fonction d'envoie uniform
		//-----------------------------------------------------------------------------
		// Matrice spécifique
		//----------------------------------------------------------------------------
		inline void SendProjection(const glm::mat4 & m);
		inline void SendModelView(const glm::mat4 & m);
		inline void SendTransform(const glm::mat4 & m);

		//-----------------------------------------------------------------------------
		// Send1i/Send1f
		//-----------------------------------------------------------------------------
		inline void Send1i(const GLchar* name, int a);
		inline void Send1f(const GLchar* name, float a);

		//-----------------------------------------------------------------------------
		// Send2i/Send2f
		//-----------------------------------------------------------------------------
		inline void Send2i(const GLchar* name, int a, int b);
		inline void Send2f(const GLchar* name, float a, float b);

		//-----------------------------------------------------------------------------
		// Send3i/Send3f
		//-----------------------------------------------------------------------------
		inline void Send3i(const GLchar* name, int a, int b, int c);
		inline void Send3f(const GLchar* name, float a, float b, float c);

		//-----------------------------------------------------------------------------
		// Send4i/Send4f
		//-----------------------------------------------------------------------------
		inline void Send4i(const GLchar* name, int a, int b, int c, int d);
		inline void Send4f(const GLchar* name, float a, float b, float c, float d);


		//-----------------------------------------------------------------------------
		// Sendmat4i/Sendmat4i
		//-----------------------------------------------------------------------------
		inline void SendMat4f(const GLchar* name, GLboolean transpose, const GLfloat *value);


		//-----------------------------------------------------------------------------
		// Send2i/Send2f
		//-----------------------------------------------------------------------------
		inline void Send2i(const GLchar* name, const glm::ivec2 &v);
		inline void Send2f(const GLchar* name, const glm::vec2 &v);

		//-----------------------------------------------------------------------------
		// Send3i/Send3f
		//-----------------------------------------------------------------------------
		inline void Send3i(const GLchar* name, const glm::ivec3 &v);
		inline void Send3f(const GLchar* name, const glm::vec3 &v);

		//-----------------------------------------------------------------------------
		// Send4i/Send4f
		//-----------------------------------------------------------------------------
		inline void Send4i(const GLchar* name, const glm::ivec4 &v);
		inline void Send4f(const GLchar* name, const glm::vec4 &v);

		//-----------------------------------------------------------------------------
		// SendMat4f
		//-----------------------------------------------------------------------------
		inline void SendMat4f(const GLchar* name, GLboolean transpose, const glm::mat4& m);
		inline void SendMat4f(const GLchar* name, GLboolean transpose, const std::vector<glm::mat4> & m);

	private:

		//chargement du vertex shader ou du fragment shader
		int LoadTypeShader(GLuint &shader, GLenum type, const File & source);
		
		void Destroy();

		void BindAllAttribLocation();


		//log error pour la compilation
		std::string error;

		//GL ID
		GLuint programID = 0;
		GLuint vertexID = 0;
		GLuint fragmentID = 0;

		static Shader *usedShader;

	};

	//inline fonction
	//--------------------------------------
	inline void Shader::Enable()
	{
		BH3D_ASSERT_MSG(programID, "Can't Enable the shader (invalid ID)", );
		usedShader = this;
		glUseProgram(programID);

	}
	inline void Shader::Disable()
	{
		usedShader = nullptr;
		glUseProgram(0);
	}

	inline bool Shader::IsValid()
	{
		return (programID > 0);
	}

	inline void Shader::Destroy()
	{
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteProgram(programID);
	}


	inline const std::string& Shader::GetError()
	{
		return error;
	}

	inline GLuint Shader::GetGLProgramID()
	{
		return programID;
	}

	inline void Shader::SendProjection(const glm::mat4 & m)
	{
		glUniformMatrix4fv
			(
				defaultUniform[BH3D_PROJECTION_ID],
				1,
				0,
				&m[0][0]
				);
	}
	inline void Shader::SendModelView(const glm::mat4 & m)
	{
		glUniformMatrix4fv
			(
				defaultUniform[BH3D_MODELVIEW_ID],
				1,
				0,
				&m[0][0]
				);
	}
	inline void Shader::SendTransform(const glm::mat4 & m)
	{
		glUniformMatrix4fv
			(
				defaultUniform[BH3D_TRANSFORM_ID],
				1,
				0,
				&m[0][0]
				);
	}

	//-----------------------------------------------------------------------------
	// Send1i/Send1f
	//-----------------------------------------------------------------------------
	inline void Shader::Send1i(const GLchar* name, int a)
	{
		glUniform1i(glGetUniformLocation(programID, name), a);
	}
	inline void Shader::Send1f(const GLchar* name, float a)
	{
		glUniform1f(glGetUniformLocation(programID, name), a);
	}

	//-----------------------------------------------------------------------------
	// Send2i/Send2f
	//-----------------------------------------------------------------------------
	inline void Shader::Send2i(const GLchar* name, int a, int b)
	{
		glUniform2i(glGetUniformLocation(programID, name), a, b);
	}
	inline void Shader::Send2f(const GLchar* name, float a, float b)
	{
		glUniform2f(glGetUniformLocation(programID, name), a, b);
	}

	//-----------------------------------------------------------------------------
	// Send3i/Send3f
	//-----------------------------------------------------------------------------
	inline void Shader::Send3i(const GLchar*name, int a, int b, int c)
	{
		glUniform3i(glGetUniformLocation(programID, name), a, b, c);
	}

	inline void Shader::Send3f(const GLchar* name, float a, float b, float c)
	{
		glUniform3f(glGetUniformLocation(programID, name), a, b, c);
	}

	//-----------------------------------------------------------------------------
	// Send4i/Send4f
	//-----------------------------------------------------------------------------
	inline void Shader::Send4i(const GLchar* name, int a, int b, int c, int d)
	{
		glUniform4i(glGetUniformLocation(programID, name), a, b, c, d);
	}
	inline void Shader::Send4f(const GLchar* name, float a, float b, float c, float d)
	{
		glUniform4f(glGetUniformLocation(programID, name), a, b, c, d);
	}
	//-----------------------------------------------------------------------------
	// SendMat4f
	//-----------------------------------------------------------------------------
	inline void Shader::SendMat4f(const GLchar* name, GLboolean transpose, const GLfloat *value)
	{
		glUniformMatrix4fv
			(
				glGetUniformLocation(programID, name),
				1,
				transpose,
				value
				);

	}


	//-----------------------------------------------------------------------------
	// Send2i/Send2f
	//-----------------------------------------------------------------------------
	inline void Shader::Send2i(const GLchar* name, const glm::ivec2 &v)
	{
		glUniform2iv(glGetUniformLocation(programID, name), 1, &v[0]);
	}
	inline void Shader::Send2f(const GLchar* name, const glm::vec2 &v)
	{
		glUniform2fv(glGetUniformLocation(programID, name), 1, &v[0]);
	}

	//-----------------------------------------------------------------------------
	// Send3i/Send3f
	//-----------------------------------------------------------------------------
	inline void Shader::Send3i(const GLchar* name, const glm::ivec3 &v)
	{
		glUniform3iv(glGetUniformLocation(programID, name), 1, &v[0]);
	}
	inline void Shader::Send3f(const GLchar* name, const glm::vec3 &v)
	{
		glUniform3fv(glGetUniformLocation(programID, name), 1, &v[0]);
	}

	//-----------------------------------------------------------------------------
	// Send4i/Send4f
	//-----------------------------------------------------------------------------
	inline void Shader::Send4i(const GLchar* name, const glm::ivec4 &v)
	{
		glUniform4iv(glGetUniformLocation(programID, name), 1, &v[0]);
	}
	inline void Shader::Send4f(const GLchar* name, const glm::vec4 &v)
	{
		glUniform4fv(glGetUniformLocation(programID, name), 1, &v[0]);
	}

	//-----------------------------------------------------------------------------
	// SendMat4f
	//-----------------------------------------------------------------------------
	inline void Shader::SendMat4f(const GLchar* name, GLboolean transpose, const glm::mat4& m)
	{
		glUniformMatrix4fv
			(
				glGetUniformLocation(programID, name),
				1,
				transpose,
				glm::value_ptr(m)
				);

	}
	//-----------------------------------------------------------------------------
	// SendMat4fv
	//----------------------------------------------------------------------------
	inline void Shader::SendMat4f(const GLchar* name, GLboolean transpose, const std::vector<glm::mat4> & m)
	{
		int id = glGetUniformLocation(programID, name);
		glUniformMatrix4fv(id, (GLsizei) m.size(), transpose, glm::value_ptr(m[0]));
	}
}
#endif //
