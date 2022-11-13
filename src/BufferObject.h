

#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <vector>
using namespace std;
using namespace glm;
class BufferObject
{
public:
	BufferObject();

	~BufferObject();
	void UpdateUVBO(const vector<vec2> &uv);
	void Draw();
	unsigned int VAO, VBO, UVBO;

private:
	vec2 position, scale;
	float rotaion_angle;
};