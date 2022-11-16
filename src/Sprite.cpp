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
	Tx.Set(pos, vec2(0, 0), scale);
}

void Sprite::Draw()
{
	texture->Bind();
	buffer->Draw();
}
void AnimSprite::Set(AnimationState state, vec2 pos, vec2 rigidBody)
{
	if (anim[state])
	{
		if (!currentAnim || currentAnim->State != state)
		{
			currentAnim = anim[state];
			currentAnim->Reset();
		}
		Tx.Set(anim[state]->GetTx(pos, rigidBody));
	}
}
// AnimSprite
void AnimSprite::Draw(double deltaTime, AnimationState state)
{

	if (anim[state])
	{
		anim[state]->Play(*buffer, deltaTime);
		anim[state]->texture->Bind();
	}
	buffer->Draw();
}

void AnimSprite::Add(AnimationData animData)
{
	anim[animData.state] = new Animation(animData);
}