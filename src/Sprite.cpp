#include "Sprite.h"

Sprite::Sprite()
{
	buffer = new BufferObject();
}

Sprite::~Sprite()
{
	delete buffer;
}

void Sprite::Draw(double deltaTime, AnimationState state)
{

	if (anim[state])
	{
		anim[state]->Play(*buffer, deltaTime);
		anim[state]->texture->Bind();
	}
	buffer->Draw();
}

void Sprite::Add(AnimationData animData)
{
	anim[animData.state] = new Animation(animData);
}