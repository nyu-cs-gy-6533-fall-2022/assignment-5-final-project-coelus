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
	auto rb = js["rigidbody"];
	rigidbody = vec4(rb["errX"], rb["errY"], rb["w"], rb["h"]);

#if DEBUG
	colDebug = new Sprite();
	colDebug->Set("red.png", vec2(rb["w"], rb["h"]), vec2(0, 0));
#endif
}
Player::~Player()
{
	delete sprite;
#if DEBUG
	delete colDebug;
#endif
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
	animStateUpdate(right, left, up, down);
}

void Player::running(int dir)
{
	if ((dir == 1 & pTx->dirX < 0) ||
		(dir == -1 & pTx->dirX > 0))
		pTx->dirX *= -1;

	vectorSpd.x = runSpeed * dir * deltaTime;
}

void Player::falling()
{
	if (!isGround)
	{

		vectorSpd.y += 9.8f * deltaTime * Global::GravityRatio;

		vectorSpd.y = clamp(vectorSpd.y, -Global::MaxSpd, Global::MaxSpd);
	}
	else
	{
		vectorSpd.y = 0;
	}
}
void Player::animStateUpdate(bool right, bool left, bool up, bool down)
{
	if (isGround)
	{
		if (right || left)
		{
			state = Run;
		}
		else
		{
			state = Idle;
		}
	}
	else
	{
		state = Fall;
	}
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
#if DEBUG
	drawCollision();
#endif
	shader->SetMat("modelMatrix", pTx->Get());
	sprite->Draw(deltaTime, state);
}
#if DEBUG
void Player::drawCollision()
{
	colDebug->Tx.Set(GetCol());
	shader->SetMat("modelMatrix", colDebug->Tx.Get());
	colDebug->Draw();
}
#endif