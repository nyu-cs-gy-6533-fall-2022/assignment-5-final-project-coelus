#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <LightData.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

using namespace std;
using namespace glm;

#ifndef _SHADER_
#define _SHADER_

class Shader
{
public:
	Shader(const string vertShaderName, const string fragShaderName)
	{
		// read file
		ifstream vertShaderFile("../shader/" + vertShaderName);
		stringstream vertRaw;
		vertRaw << vertShaderFile.rdbuf();
		ifstream fragShaderFile("../shader/" + fragShaderName);
		stringstream fragRaw;
		fragRaw << fragShaderFile.rdbuf();

		string vertShaderStr = vertRaw.str();
		string fragShaderString = fragRaw.str();

		unsigned int vert, frag;
		const char *vertShader = vertShaderStr.c_str();
		const char *fragShader = fragShaderString.c_str();

		id = glCreateProgram();

		vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, &vertShader, NULL);
		glCompileShader(vert);
		glAttachShader(id, vert);

		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &fragShader, NULL);
		glCompileShader(frag);
		glAttachShader(id, frag);

		glLinkProgram(id);

		glDeleteShader(vert);
		glDeleteShader(frag);

		Use();
		SetVec4("color", vec4(1, 1, 1, 1));
	};

	~Shader(){};

	void Use()
	{
		glUseProgram(id);
	};

	void SetInt(const char *name, int val)
	{
		GLint *glval = new GLint[1];
		glval[0] = val;
		glUniform1iv(glGetUniformLocation(id, name), 1, glval);
	};
	void SetFloat(const char *name, float val)
	{
		GLfloat *glval = new GLfloat[1];
		glval[0] = val;
		glUniform1fv(glGetUniformLocation(id, name), 1, glval);
	};
	void SetMat4(const char *name, mat4 mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value_ptr(mat));
	};
	void SetVec2(const char *name, vec2 vec)
	{
		glUniform2fv(glGetUniformLocation(id, name), 1, value_ptr(vec));
	};
	void SetVec4(const char *name, vec4 vec)
	{
		glUniform4fv(glGetUniformLocation(id, name), 1, value_ptr(vec));
	};
	void SetVec4(const char *name, const vector<vec4> &vec)
	{

		glUniform4fv(glGetUniformLocation(id, name), vec.size(), value_ptr(vec[0]));
	};
	void SetLightData(string name, const vector<LightData> &data)
	{
		for (int i = 0; i < data.size(); i++)
		{
			GLint *glval = new GLint[1];
			glval[0] = data[i].type;
			glUniform1iv(glGetUniformLocation(id, string(name + "[" + to_string(i) + "].type").c_str()), 1, glval);
			glUniform2fv(glGetUniformLocation(id, string(name + "[" + to_string(i) + "].pos").c_str()), 1, value_ptr(data[i].pos));
			glUniform2fv(glGetUniformLocation(id, string(name + "[" + to_string(i) + "].dir").c_str()), 1, value_ptr(data[i].dir));
		}
	};

private:
	unsigned int id;
};
#endif