
#pragma once

#include "Texture.h"
#include "BufferObject.h"
#include "Transform.h"
#include "Action.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;

struct AnimationData
{
	ActionState state;
	int pivotX, pivotY, width, height, frameCount;
	double secPerFrame, secLastFrame;
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
	void AddFrame() { frameTime += deltaTime; };
	bool IsFrame(int index) { return frameTime < deltaTime && index == frameIndex; }
	bool IsFrameGreater(int index) { return frameIndex >= index; }
	bool CanInterrupt() { return frameTime >= secPerFrame && frameIndex == frameCount - 1; }
	bool IsEnd() { return isEnd; }
	void PauseFrame() { isPause = true; }
	void ResumeFrame() { isPause = false; }

	Transform GetTx(vec2 pos, vec2 rigidBody)
	{
		tx.rigidBody = rigidBody;
		tx.position = pos;
		return tx;
	}
	ActionState State;
	Texture *texture;

private:
	const double secPerFrame, secLastFrame;
	double deltaTime, frameTime;
	int frameIndex, frameCount;
	bool isLoop, isEnd, isPause;
	Transform tx;

	vector<vec4> frames;
};