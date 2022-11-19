#include "Player.h"

Player::Player(SoundSystem *sndSys, Shader *s, double &time)
	: soundSys(sndSys),
	  shader(s),
	  deltaTime(time),
	  position(vec2(0, 0)),
	  ctrlX(0)
{

	sprite = new AnimSprite();
	debug = new Debug(shader);
	pTx = &sprite->Tx;
	pTx->dirX = -1;

	fsm = new FSM(
		FSMData{soundSys, sprite,
				runSpeed, jumpSpeed,
				pTx->dirX,
				velocity, force,
				deltaTime,
				isGround, isTop,
				canJumpAttack,
				ctrlX,
				dAttack, dChain, dJump});
	fsm->Add<PlayerIdle>(Idle);
	fsm->Add<PlayerRun>(Run);
	fsm->Add<PlayerJump>(Jump);
	fsm->Add<PlayerFall>(Fall);
	fsm->Add<PlayerAttack1>(Attack1);
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
Player::~Player()
{
	delete sprite;
	delete debug;
	delete fsm;
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
		dJump.Press(false);
		if (ctrl.jump)
		{
			dJump.Press(true);
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
		dAttack.Press(true);
	}
	else
	{
		dAttack.Press(false);
	}

	//deffered keys
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
	stateUpdate();
	fsm->Update();
	movement();
}
void Player::stateUpdate()
{
	ActionState nextState = fsmInput.GetNextState(fsm->GetPossibleState());
	if (nextState != EmptyState && nextState != fsm->GetState())
	{
		fsm->Exit();
		sprite->Set(nextState);
		fsm->Set(nextState);
		fsm->Enter();
	}
}

void Player::movement()
{
	velocity.x += force.x;
	velocity.y += force.y;
	velocity.y = clamp(velocity.y, -Global::MaxSpd, Global::MaxSpd);
	position.x += velocity.x;
	position.y += velocity.y;
}

void Player::Draw(double deltaTime)
{
	debug->SetDebugTx(0, GetCol());
	debug->DrawDebug();

	sprite->Set(position, rigidbody);
	shader->SetMat("modelMatrix", pTx->Get());
	sprite->Draw(deltaTime);
}
