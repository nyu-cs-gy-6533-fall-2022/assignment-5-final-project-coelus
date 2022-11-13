#include "Player.h"

Player::Player() : walkSpeed(5)
{

	sprite = new Sprite("main/idle(256-line).png");
	sprite->Transform.Position = vec2(-75, -75);
	sprite->Transform.Scale = vec2(150, 150);

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