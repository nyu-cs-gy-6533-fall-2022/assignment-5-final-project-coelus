#include "Sprite.h"

Sprite::Sprite()
{
	buffer = new BufferObject();
}

Sprite::~Sprite()
{
	delete buffer;
	delete texture;
}
void Sprite::Set(string filePath,vec2 scale)
{
	texture = new Texture(filePath);
    Transform.Scale = scale;
}

void Sprite::Draw()
{
	texture->Bind();
	buffer->Draw();
}

//AnimSprite
void AnimSprite::Draw(double deltaTime, AnimationState state)
{

	if (anim[state])
	{
		Transform.Scale = anim[state]->GetScale();
		anim[state]->Play(*buffer, deltaTime);
		anim[state]->texture->Bind();
	}
	buffer->Draw();
}

void AnimSprite::Add(AnimationData animData)
{
	anim[animData.state] = new Animation(animData);
}