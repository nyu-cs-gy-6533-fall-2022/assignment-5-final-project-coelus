#pragma once
#include "Transform.h"
#include "Animation.h"
#include "Texture.h"
#include "BufferObject.h"
#define GLEW_STATIC
#include <GL/glew.h>

#include <unordered_map>

class Sprite
{
public:
	Sprite();

	~Sprite();

	void Draw(double deltatime, AnimationState type);

	void Add(AnimationData animData);

	Transform Transform;

private:
	
	BufferObject *buffer;

	unordered_map<AnimationState, Animation *> anim;
};