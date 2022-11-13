#include "Animation.h"

using namespace std;

Animation::Animation(AnimationData data)
	: frameTime(0),
	  frameIndex(0),
	  secPerFrame(data.secPerFrame),
	  frameCount(data.frameCount)
{
	for (int i = 0; i < data.frameCount; i++)
	{
		vec4 frame = vec4(1, data.height * i, data.width, data.height);
		frames.push_back(frame);
	}
}

void Animation::Play(Texture &texture, BufferObject &buffer, double deltaTime)
{
	frameTime += deltaTime;

	if (frameTime > secPerFrame)
	{
		frameIndex = (frameIndex + 1) % frameCount;
		frameTime = 0;
	}

	vec4 frame = frames[frameIndex];
	// normalize
	frame.x /= texture.GetWidth();
	frame.y /= texture.GetHeight();
	frame.z /= texture.GetWidth();
	frame.w /= texture.GetHeight();

	vector<vec2> uv = {
		vec2(frame.x, frame.y),
		vec2(frame.x, frame.y + frame.w),
		vec2(frame.x + frame.z, frame.y),
		vec2(frame.x + frame.z, frame.y),
		vec2(frame.x, frame.y + frame.w),
		vec2(frame.x + frame.z, frame.y + frame.w)};
	buffer.UpdateUVBO(uv);
}

void Animation::setSecPerFrame(float sec)
{
	secPerFrame = sec;
}