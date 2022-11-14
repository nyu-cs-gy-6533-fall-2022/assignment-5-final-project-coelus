#include "Player.h"

Player::Player(Shader *s)
	: shader(s), state(Idle), walkSpeed(5)
{

	sprite = new AnimSprite();
	transform = &sprite->Transform;

	Loader::Load("player.json", vector<Sprite *>{sprite});


}

Player::~Player()
{
	delete sprite;
}
vec2 Player::getPos()
{
	return transform->Position;
}
void Player::setPos(vec2 pos)
{
	transform->Position = pos;
}
void Player::Move(int dir)
{
	if ((dir == 1 & transform->dirX < 0) ||
		(dir == -1 & transform->dirX > 0))
		transform->dirX *= -1;

	state = Run;
	transform->Position.x += walkSpeed * dir;
}

void Player::Stop()
{
	state = Idle;
}

void Player::Draw(double deltatime)
{

	shader->SetMat("modelMatrix", transform->Get());
	sprite->Draw(deltatime, state);
}