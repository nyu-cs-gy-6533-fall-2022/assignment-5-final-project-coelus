#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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
	Shader(const string vertShaderName, const string fragShaderName);

	~Shader();

	void Use();

	void SetMat(const char* name, mat4 mat);
	void SetVec4(const char* name, vec4 vec);

private:
	unsigned int id;
};
#endif