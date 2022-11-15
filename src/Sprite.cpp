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
void Sprite::Set(string filePath, vec2 scale, vec2 pos)
{
	texture = new Texture(filePath);
	Tx.Position = pos;
	Tx.Scale = scale;
}

void Sprite::Draw()
{
	texture->Bind();
	buffer->Draw();
}

// AnimSprite
void AnimSprite::Draw(double deltaTime, AnimationState state)
{

	if (anim[state])
	{
		Tx.Scale = anim[state]->GetScale();
		anim[state]->Play(*buffer, deltaTime);
		anim[state]->texture->Bind();
	}
	buffer->Draw();
}

void AnimSprite::Add(AnimationData animData)
{
	anim[animData.state] = new Animation(animData);
}