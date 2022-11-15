#include "Player.h"

Player::Player(Shader *s, double &time)
	: shader(s), deltaTime(time), state(Idle)
{

	sprite = new AnimSprite();
	pTx = &sprite->Tx;
	debug = new Debug(shader);
	loadData();
}
void Player::loadData()
{
	json js = Loader::Load("player.json", vector<Sprite *>{sprite});
	runSpeed = js["runSpeed"];
	jumpSpeed = js["jumpSpeed"];

	auto rb = js["rigidbody"];
	rigidbody = vec4(rb["errX"], rb["errY"], rb["w"], rb["h"]);
	debug->AddDebug(vec4(0, 0, rb["w"], rb["h"]));
}
Player::~Player()
{
	delete sprite;
	delete debug;
}

void Player::Input(Control ctrl)
{
	if (ctrl.right)
	{
		running(1);
	}
	else if (ctrl.left)
	{
		running(-1);
	}
	if (ctrl.jump)
	{
		setJump();
	}
	if (!(ctrl.right || ctrl.left))
	{
		setIdle();
	}
	falling();
	movement();
	animStateUpdate(ctrl);
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
void Player::animStateUpdate(Control ctrl)
{
	if (isGround)
	{
		if (ctrl.right || ctrl.left)
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
void Player::setJump()
{
	vectorSpd.y += -jumpSpeed * deltaTime;
}
void Player::setIdle()
{
	vectorSpd.x = 0;
}
void Player::movement()
{
	pTx->Position.x += vectorSpd.x;
	pTx->Position.y += vectorSpd.y;
}

void Player::Draw(double deltaTime)
{
	debug->SetDebugTx(0, GetCol());
	debug->DrawDebug();
	shader->SetMat("modelMatrix", pTx->Get());
	sprite->Draw(deltaTime, state);
}
