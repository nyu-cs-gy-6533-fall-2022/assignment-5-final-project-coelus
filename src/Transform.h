#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#ifndef _TRANSFORM_
#define _TRANSFORM_
struct Transform
{
	vec2 position = vec2(0);
	vec2 pivot = vec2(0);
	vec2 scale = vec2(1);
	vec2 rigidBody = vec2(0);

	int dirX = 1;
	float angle = 0;

	void Set(vec2 pos, vec2 vpivot, vec2 vscale)
	{
		position = pos;
		pivot = vpivot;
		scale = vscale;
	}

	void Set(Transform tx)
	{
		pivot = tx.pivot;
		position = tx.position;
		scale = tx.scale;
		rigidBody = tx.rigidBody;
	}
	void Set(vec4 rect)
	{
		position = vec2(rect.x, rect.y);
		scale = vec2(rect.z, rect.w);
	}
	// get x from center x
	float GetX(vec2 mainPos, float frontX, float w)
	{
		float x = position.x - w / 2 + rigidBody.x / 2 + frontX * dirX;

		return x;
	}
	

	float GetSpriteX()
	{
		float x = position.x - pivot.x;
		if (dirX == -1)
		{
			x = position.x + pivot.x + rigidBody.x;
		}
		return x;
	}

	mat4 Get()
	{
		float x = GetSpriteX();
		float y = position.y - pivot.y;
		return translate(mat4(1.0f), vec3(x, y, 1.0f)) *
			   rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f)) *
			   glm::scale(mat4(1.0f), vec3(scale.x * dirX, scale.y, 1.0f));
	}
};

#endif