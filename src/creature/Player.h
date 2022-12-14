
#pragma once
#include "Creature.h"
#include "Chain.h"
#ifndef _PLAYER_
#define _PLAYER_

enum FullScreenType
{
	FSNONE,
	FSBLUR,
	FSFADE
};

class Player : public Creature
{
public:
	Player(SoundSystem *sndSys, const vector<Shader *> &s, double &time) : Creature(sndSys, s, time)
	{
		chain = new Chain(soundSys, shaders, deltaTime, pTx, position);
		isChainThrow = &chain->IsThrow;
		isChainHit = &chain->IsHit;
		InitFSM();

		fsm->Add<FSPlayerIdle>(Idle);
		fsm->Add<FSPlayerRun>(Run);
		fsm->Add<FSPlayerJump>(Jump);
		fsm->Add<FSPlayerFall>(Fall);
		fsm->Add<FSPlayerAttack1>(Attack1);
		fsm->Add<FSPlayerAttack2>(Attack2);
		fsm->Add<FSPlayerAttack3>(Attack3);
		fsm->Add<FSPlayerJumpAttack>(JumpAttack);
		fsm->Add<FSPlayerChainU>(ChainU);
		fsm->Add<FSPlayerChainFlyU>(ChainFlyU);
		fsm->Add<FSPlayerDamaged>(Damaged);
		fsm->Add<FSPlayerDied>(Died);
		fsm->Add<FSPlayerDiedIdle>(DiedIdle);

		loadData();
		chainCD.Init(0.6f, deltaTime, 0);
		chainCD.Close();
		fadeCD.Init(pi, deltaTime, 0);
		fadeCD.Close();

		sprite->Set(Idle);
		fsm->Set(Idle);
		fsm->Enter();
	}

	void Init()
	{
		isDamaged = false;
		hp = initHp;
		velocity = vec2(0);
		force = vec2(0);
		chainCD.Close();
		fsm->Set(Idle);
		fsm->Enter();
	}
	void Update(Control ctrl)
	{
		if (fadeCD.IsRun())
		{
			fadeCD.Update(2);
			if (IsHP0() && abs(cos(fadeCD.t)) <= 0.05f)
			{
				Init();
				isRespawn = true;
			}
		}
		else if (fsm->GetState() == DiedIdle)
		{
			fadeCD.Reset();
		}
		fsmInput.Init();

		if (IsHP0())
		{
			fsmInput.Add(Died);
			fsmInput.Add(DiedIdle);
		}

		if (isDamaged)
		{

			fsmInput.Add(Damaged);
		}
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
			chainCD.Close();
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
		if (chainEnd)
		{
			chainEnd = false;
			chainCD.Reset();
		}
		chainCD.Update();
		if (ctrl.chain && chainCD.IsEnd())
		{
			fsmInput.Add(ChainU);
		}

		if (*isChainHit)
		{
			fsmInput.Add(ChainFlyU);
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
		updateHitBox();
		stateUpdate();
		fsm->Update();
		movement();
		chain->Update();
	}
	void Draw()
	{
		Creature::Draw();
	}
	void DrawChain()
	{
		chain->Draw();
	}
	vec4 GetChainCol() { return chain->GetCol(); }
	void SetChainHit(MaterialType mat)
	{
		chain->SetHit(mat);
	}
	float GetFadeTime()
	{
		return fadeCD.t;
	}
	bool IsRespawn()
	{
		return isRespawn;
	}
	void RespawnDone()
	{
		isRespawn = false;
	}
	FullScreenType GetFullScreenShader()
	{
		if (fadeCD.IsRun())
		{
			return FSFADE;
		}
		else if (fsm->GetState() == Damaged)
		{
			return FSBLUR;
		}
		else
		{
			return FSNONE;
		}
	}

private:
	Chain *chain;
	CountDown chainCD, fadeCD;

	bool isRespawn = false;
	const double pi = atan(1) * 4;
	void loadData()
	{
		json js = Loader::Load("player.json", vector<Sprite *>{sprite});
		runSpeed = js["runSpeed"];
		jumpSpeed = js["jumpSpeed"];
		setInitHp(js["hp"]);

		auto rb = js["rigidbody"];
		rigidbody = vec2(rb["w"], rb["h"]);
		debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
	}
};
#endif