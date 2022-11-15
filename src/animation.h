
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
	void SetSecPerFrame(float sec);
	glm::vec2 GetScale();
	AnimationState State;
	Texture *texture;

private:
	double frameTime, secPerFrame;
	int frameIndex, frameCount;
	int centerX, centerY, width, height;
	bool isLoop;

	vector<vec4> frames;
};