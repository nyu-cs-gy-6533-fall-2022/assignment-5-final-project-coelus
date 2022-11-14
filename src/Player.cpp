#include "Player.h"

Player::Player(Shader *s) : shader(s), walkSpeed(5)
{

	sprite = new AnimSprite();
	transform = &sprite->Transform;

	new Loader("player.json",vector<Sprite*>{sprite});

	state = Idle;
}

Player::~Player()
{
	delete sprite;
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