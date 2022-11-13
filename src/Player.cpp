#include "Player.h"

Player::Player() : walkSpeed(5)
{

	sprite = new Sprite();
	sprite->Transform.Position = vec2(0, 0);
	sprite->Transform.Scale = vec2(200, 200);

	loader = new Loader(sprite, "player.json");

	state = Idle;
}

Player::~Player()
{
	delete sprite;
}

void Player::Move(int dir)
{
	if ((dir == 1 & sprite->Transform.Scale.x < 0) ||
		(dir == -1 & sprite->Transform.Scale.x > 0))
		sprite->Transform.Scale.x *= -1;

	state = Run;
	sprite->Transform.Position.x += walkSpeed * dir;
}

void Player::Stop()
{
	state = Idle;
}

glm::mat4 Player::Transform()
{
	return sprite->Transform.Get();
}

void Player::Draw(double deltatime)
{
	sprite->Draw(deltatime, state);
}