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

// AnimSprite
void AnimSprite::Draw(double deltaTime)
{

	anim[action]->Play(*buffer, deltaTime);
	anim[action]->texture->Bind();
	buffer->Draw();
}

void AnimSprite::Add(AnimationData animData)
{
	anim[animData.state] = new Animation(animData);
}