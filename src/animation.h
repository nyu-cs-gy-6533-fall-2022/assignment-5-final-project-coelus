
#pragma once

#include "Texture.h"
#include "BufferObject.h"
#include "Transform.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;

enum ActionState
{
	EmptyState,
	Idle,
	Run,
	Jump,
	Fall,
	Attack1,
	Attack2,
	Attack3

};

struct AnimationData
{
	ActionState state;
	int pivotX, pivotY, width, height, frameCount;
	double secPerFrame;
	bool isLoop;
	string filename;
};
class Animation
{
public:
	Animation(AnimationData data);
	~Animation();
	void Reset();
	void Play(BufferObject &buffer, double deltatime);
	void UpdateSprite(BufferObject &buffer);
	void SetSecPerFrame(float sec) { secPerFrame = sec; }
	bool IsFrame(int index) { return frameTime < deltaTime && index == frameIndex; }
	bool IsEnd() { return isEnd; }
	Transform GetTx(vec2 pos, vec2 rigidBody)
	{
		tx.rigidBody = rigidBody;
		tx.position = pos;
		return tx;
	}
	ActionState State;
	Texture *texture;

private:
	double deltaTime, frameTime, secPerFrame;
	int frameIndex, frameCount;
	bool isLoop, isEnd;
	Transform tx;

	vector<vec4> frames;
};