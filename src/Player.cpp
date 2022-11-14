#include "Player.h"

Player::Player(Shader *s, double &time)
	: shader(s), deltaTime(time), state(Idle)
{

	sprite = new AnimSprite();
	pTx = &sprite->Tx;
	loadData();
}
void Player::loadData()
{
	json js = Loader::Load("player.json", vector<Sprite *>{sprite});
	runSpeed = js["runSpeed"];
}
Player::~Player()
{
	delete sprite;
}
vec2 Player::GetPos()
{
	return pTx->Position;
}
void Player::SetPos(vec2 pos)
{
	pTx->Position = pos;
}

void Player::Control(bool right, bool left, bool up, bool down)
{
	if (right)
	{
		running(1);
	}
	else if (left)
	{
		running(-1);
	}
	if (!(right || left))
	{
		setIdle();
	}
	falling();
	movement();
}

void Player::running(int dir)
{
	if ((dir == 1 & pTx->dirX < 0) ||
		(dir == -1 & pTx->dirX > 0))
		pTx->dirX *= -1;

	state = Run;
	vectorSpd.x = runSpeed * dir * deltaTime;
}

void Player::falling()
{

	state = Fall;
	vectorSpd.y += 9.8f * deltaTime;
}

void Player::setIdle()
{
	vectorSpd.x = 0;
	state = Idle;
}
void Player::movement()
{
	pTx->Position.x += vectorSpd.x;
	pTx->Position.y += vectorSpd.y;
}

void Player::Draw(double deltaTime)
{

	shader->SetMat("modelMatrix", pTx->Get());
	sprite->Draw(deltaTime, state);
}