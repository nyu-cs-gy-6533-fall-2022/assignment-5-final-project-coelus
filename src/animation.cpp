#include "Animation.h"

using namespace std;

Animation::Animation(AnimationData data)
	: frameTime(0),
	  frameIndex(0),
	  State(data.state),
	  centerX(data.centerX),
	  centerY(data.centerY),
	  width(data.width),
	  height(data.height),
	  frameCount(data.frameCount),
	  secPerFrame(data.secPerFrame),
	  isLoop(data.isLoop)
{

	texture = new Texture(data.filename);

	for (int i = 0; i < data.frameCount; i++)
	{
		vec4 frame = vec4(1, data.height * i, data.width, data.height);
		frames.push_back(frame);
	}
}
Animation::~Animation()
{
	delete texture;
}
void Animation::Reset()
{
	frameTime = 0;
	frameIndex = 0;
}
void Animation::Play(BufferObject &buffer, double dt)
{
	deltaTime = dt;
	frameTime += deltaTime;

	if (frameTime > secPerFrame)
	{
		frameTime = 0;
		if (isLoop)
		{
			frameIndex = (frameIndex + 1) % frameCount;
		}
		else
		{
			frameIndex = std::min(frameIndex + 1, frameCount - 1);
		}
	}

	UpdateSprite(buffer);
}

void Animation::UpdateSprite(BufferObject &buffer)
{
	vec4 frame = frames[frameIndex];
	frame.x /= texture->GetWidth();
	frame.y /= texture->GetHeight();
	frame.z /= texture->GetWidth();
	frame.w /= texture->GetHeight();

	vector<vec2> uv = {
		vec2(frame.x, frame.y),
		vec2(frame.x, frame.y + frame.w),
		vec2(frame.x + frame.z, frame.y),
		vec2(frame.x + frame.z, frame.y),
		vec2(frame.x, frame.y + frame.w),
		vec2(frame.x + frame.z, frame.y + frame.w)};
	buffer.UpdateUVBO(uv);
}
