#include "Animation.h"

using namespace std;

Animation::Animation(AnimationData data)
	: frameTime(0),
	  frameIndex(0),
	  secPerFrame(data.secPerFrame),
	  frameCount(data.frameCount)
{
	texture = new Texture(data.filename);
	width = data.width;
	height = data.height;
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
void Animation::Play(BufferObject &buffer, double deltaTime)
{
	frameTime += deltaTime;

	if (frameTime > secPerFrame)
	{
		frameIndex = (frameIndex + 1) % frameCount;
		frameTime = 0;
	}

	vec4 frame = frames[frameIndex];
	// normalize
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

glm::vec2 Animation::GetScale()
{
	return glm::vec2(width, height);
}
void Animation::SetSecPerFrame(float sec)
{
	secPerFrame = sec;
}