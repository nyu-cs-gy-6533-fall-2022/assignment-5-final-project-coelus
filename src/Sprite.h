#pragma once
#include "Transform.h"
#include "Animation.h"
#include "Texture.h"
#include "BufferObject.h"
#define GLEW_STATIC
#include <GL/glew.h>

#include <unordered_map>

#ifndef _Sprite_
#define _Sprite_
class Sprite
{
public:
	Sprite();

	~Sprite();

	void Draw();
	void Set(string filePath, vec2 scale);
	virtual void Add(AnimationData animData) {}

	Transform Tx;

protected:
	BufferObject *buffer;
	Texture *texture;
};

class AnimSprite : public Sprite
{
public:
	AnimSprite():Sprite(){};

	~AnimSprite(){};

	void Draw(double deltatime, AnimationState type);

	void Add(AnimationData animData);

private:
	unordered_map<AnimationState, Animation *> anim;
};
#endif