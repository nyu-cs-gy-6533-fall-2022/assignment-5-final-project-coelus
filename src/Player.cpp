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

	fsm = new FSM(
		FSMData{soundSys, sprite,
				runSpeed, jumpSpeed,
				pTx->dirX,
				velocity,
				deltaTime,
				isGround, isTop,
				ctrlX});
	fsm->Add<PlayerIdle>(Idle);
	fsm->Add<PlayerRun>(Run);
	fsm->Add<PlayerJump>(Jump);
	fsm->Add<PlayerFall>(Fall);
	fsm->Add<PlayerAttack1>(Attack1);
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

void Player::Input(Control ctrl)
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
	if (ctrl.attack && isGround)
	{
		fsmInput.Add(Attack1);
	}
	if (ctrl.jump && isGround)
	{
		fsmInput.Add(Jump);
	}

	stateUpdate();
	fsm->Update();
	movement();
}
void Player::stateUpdate()
{
	AnimationState nextState = fsmInput.GetNextState(fsm->GetPossibleState());
	if (nextState != EmptyState && nextState != fsm->GetState())
	{
		fsm->Exit();
		fsm->Set(nextState);
		fsm->Enter();
	}
}

void Player::movement()
{
	velocity.y = clamp(velocity.y, -Global::MaxSpd, Global::MaxSpd);
	position.x += velocity.x;
	position.y += velocity.y;
}

void Player::Draw(double deltaTime)
{
	debug->SetDebugTx(0, GetCol());
	debug->DrawDebug();

	sprite->Set(fsm->GetState(), position, rigidbody);
	shader->SetMat("modelMatrix", pTx->Get());
	sprite->Draw(deltaTime, fsm->GetState());
}
