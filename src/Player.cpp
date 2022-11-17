#include "Player.h"

Player::Player(SoundSystem *sndSys, Shader *s, double &time)
	: soundSys(sndSys),
	  shader(s),
	  deltaTime(time),
	  state(Idle),
	  nextState(EmptyState),
	  position(vec2(0, 0)),
	  ctrlX(false)
{

	sprite = new AnimSprite();
	debug = new Debug(shader);
	pTx = &sprite->Tx;

	fsm = new FSM(
		FSMInput{soundSys, sprite,
				 runSpeed, jumpSpeed,
				 pTx->dirX,
				 velocity,
				 state,
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
	ctrlX = ctrl.right || ctrl.left;
	if (ctrl.right)
	{
		setDirX(1);
		nextState = Run;
	}
	else if (ctrl.left)
	{
		setDirX(-1);
		nextState = Run;
	}
	else
	{
		nextState = Idle;
	}

	if (!isGround)
	{
		nextState = Fall;
	}
	if (ctrl.attack && isGround)
	{
		nextState = Attack1;
	}
	if (ctrl.jump && isGround)
	{
		nextState = Jump;
	}

	stateUpdate();
	fsm->Update();
	movement();
}
void Player::stateUpdate()
{
	if (!fsm->TryNextState(nextState))
		return;
	if (nextState != EmptyState)
	{
		state = nextState;
		nextState = EmptyState;
		fsm->Exit();
		fsm->Set(state);
		fsm->Enter();
	}
}
void Player::setDirX(int dir)
{
	if ((dir == 1 & pTx->dirX < 0) ||
		(dir == -1 & pTx->dirX > 0))
		pTx->dirX *= -1;
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

	sprite->Set(state, position, rigidbody);
	shader->SetMat("modelMatrix", pTx->Get());
	sprite->Draw(deltaTime, state);
}
