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
	void Set(string filePath, vec2 scale, vec2 pos = vec2(0, 0));
	virtual void Add(AnimationData animData) {}

	Transform Tx;

protected:
	BufferObject *buffer;
	Texture *texture;
};

class AnimSprite : public Sprite
{
public:
	AnimSprite() : Sprite(){};

	~AnimSprite(){};
	void Draw(double deltatime);
	void Add(AnimationData animData);

	void Set(ActionState state) { action = state; }
	void Set(vec2 pos, vec2 rigidBody) { Tx.Set(anim[action]->GetTx(pos, rigidBody)); }

	void Reset() { anim[action]->Reset(); }

	bool IsFrame(int index) { return anim[action]->IsFrame(index); }
	bool IsEnd() { return anim[action]->IsEnd(); }

private:
	ActionState action = Idle;
	unordered_map<ActionState, Animation *> anim;
};
#endif