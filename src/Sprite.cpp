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
void AnimSprite::Adjusting(AnimationState state,vec2 plPos)
{
	if (anim[state])
	{
		if (!currentAnim || currentAnim->State != state)
		{
			currentAnim = anim[state];
			currentAnim->Reset();
		}
		Tx.Position = plPos - currentAnim->GetCenterPos();
		Tx.Scale = anim[state]->GetScale();
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