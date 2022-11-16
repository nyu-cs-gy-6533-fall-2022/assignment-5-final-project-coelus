
#pragma once

#include "Texture.h"
#include "BufferObject.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;

enum AnimationState
{
	Idle,
	Run,
	Jump,
	Fall
};

struct AnimationData
{
	AnimationState state;
	int centerX, centerY, width, height, frameCount;
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

	glm::vec2 GetScale() { return glm::vec2(width, height); }
	AnimationState State;
	Texture *texture;

private:
	double deltaTime, frameTime, secPerFrame;
	int frameIndex, frameCount;
	int centerX, centerY, width, height;
	bool isLoop;

	vector<vec4> frames;
};