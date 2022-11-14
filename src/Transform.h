#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#ifndef _TRANSFORM_
#define _TRANSFORM_
struct Transform
{
	int dirX = 1;
	vec2 Position = vec2(0);
	vec2 Scale = vec2(1);
	float Angle = 0;

	mat4 Get()
	{
		return translate(mat4(1.0f), vec3(Position.x + (1 - dirX) * Scale.x / 2, Position.y, 1.0f)) *
			   rotate(mat4(1.0f), Angle, vec3(0.0f, 0.0f, 1.0f)) *
			   glm::scale(mat4(1.0f), vec3(Scale.x * dirX, Scale.y, 1.0f));
	}
};

#endif