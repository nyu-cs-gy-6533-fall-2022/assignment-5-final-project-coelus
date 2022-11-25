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

	float GetX(float range, float w)
	{
		float halfW = w / 2;
		float x = position.x - halfW + rigidBody.x + range;
		if (dirX == -1)
		{
			x = position.x - halfW - range;
		}
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