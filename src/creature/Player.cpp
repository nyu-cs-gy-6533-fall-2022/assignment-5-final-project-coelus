#include "Player.h"

Player::Player(SoundSystem *sndSys, Shader *s, double &time)
	: Creature(sndSys, s, time)
{
	
	fsm->Add<FSPlayerIdle>(Idle);
	fsm->Add<FSPlayerRun>(Run);
	fsm->Add<PlayerJump>(Jump);
	fsm->Add<FSPlayerFall>(Fall);
	fsm->Add<FSPlayerAttack1>(Attack1);
	fsm->Add<PlayerAttack2>(Attack2);
	fsm->Add<PlayerAttack3>(Attack3);
	fsm->Add<PlayerJumpAttack>(JumpAttack);

	fsm->Set(Idle);

	loadData();
}
void Player::loadData()
{
	json js = Loader::Load("player.json", vector<Sprite *>{sprite});
	runSpeed = js["runSpeed"];
	jumpSpeed = js["jumpSpeed"];

	auto rb = js["rigidbody"];
	rigidbody = vec2(rb["w"], rb["h"]);
	debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
}
void Player::Update(Control ctrl)
{
	fsmInput.Init();
	if (ctrl.right)
	{
		ctrlX = 1;
		fsmInput.Add(Run);
	}
	else if (ctrl.left)
	{
		ctrlX = -1;
		fsmInput.Add(Run);
	}
	else
	{
		ctrlX = 0;
		fsmInput.Add(Idle);
	}

	if (!isGround)
	{
		fsmInput.Add(Fall);
	}
	else
	{
		if (ctrl.jump)
		{

			fsmInput.Add(Jump);
		}
		canJumpAttack = true;
	}
	if (ctrl.attack)
	{
		if (isGround)
		{
			fsmInput.Add(Attack1);
			fsmInput.Add(Attack2);
			fsmInput.Add(Attack3);
		}
		else if (canJumpAttack)
		{
			fsmInput.Add(JumpAttack);
		}
	}

	dAttack.Press(ctrl.attack);
	dJump.Press(ctrl.jump);

	// deffered keys
	if (dAttack.IsDeffered())
	{
		if (isGround)
		{
			fsmInput.Add(Attack1);
			fsmInput.Add(Attack2);
			fsmInput.Add(Attack3);
		}
		else if (canJumpAttack)
		{
			fsmInput.Add(JumpAttack);
		}
	}
	if (dJump.IsDeffered())
	{
		fsmInput.Add(Jump);
	}
	StateUpdate();
	fsm->Update();
	Movement();
}
