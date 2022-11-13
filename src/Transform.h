#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
struct Transform
{
	vec2 Position = vec2(0);
	vec2 Scale = vec2(1);
	float Angle = 0;

	mat4 Get()
	{
		return translate(mat4(1.0f), vec3(Position.x - Scale.x / 2, Position.y - Scale.y / 2, 1.0f)) *
			   rotate(mat4(1.0f), Angle, vec3(0.0f, 0.0f, 1.0f)) *
			   glm::scale(mat4(1.0f), vec3(Scale, 1.0f));
	}
};