#include "Sprite.h"

Sprite::Sprite(const char *filename)
{

	texture = new Texture(filename);
	buffer = new BufferObject();
}

Sprite::~Sprite()
{
	delete texture;
	delete buffer;
}

void Sprite::Draw(double deltaTime, AnimationState state)
{

	if (anim[state])
	{
		anim[state]->Play(*texture, *buffer, deltaTime);
	}

	texture->Bind();
	buffer->Draw();
}

void Sprite::Add(AnimationData animData)
{
	anim[animData.state] = new Animation(animData);
}