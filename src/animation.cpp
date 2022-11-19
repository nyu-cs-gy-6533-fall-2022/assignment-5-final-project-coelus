#include "Animation.h"

using namespace std;

Animation::Animation(AnimationData data)
	: frameTime(0),
	  frameIndex(0),
	  State(data.state),
	  frameCount(data.frameCount),
	  secPerFrame(data.secPerFrame),
	  secLastFrame(data.secLastFrame),
	  isLoop(data.isLoop),
	  isEnd(false)
{
	tx.Set(vec2(0, 0), vec2(data.pivotX, data.pivotY), vec2(data.width, data.height));
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
	isEnd = false;
}
void Animation::Play(BufferObject &buffer, double dt)
{
	deltaTime = dt;
	frameTime += deltaTime;

	double currentFramsec = secPerFrame;
	if (frameIndex == frameCount - 1)
	{
		currentFramsec = secLastFrame;
	}
	if (frameTime > currentFramsec)
	{
		frameTime = 0;
		if (isLoop)
		{
			frameIndex = (frameIndex + 1) % frameCount;
		}
		else
		{
			if (frameIndex + 1 >= frameCount)
			{
				isEnd = true;
			}
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
