
#pragma once
#include "Collision.h"
#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"
#include "Debug.h"
#include "Global.h"
#include "Sound.h"
#include "DefferedKey.h"

#include "fsm/FSM.h"

#include <algorithm>
using namespace glm;

#ifndef _PLAYER_
#define _PLAYER_

struct Control
{
	bool right, left, up, down;
	bool jump, attack, drag;
};
class Player
{
public:
	Player(SoundSystem *sndSys, Shader *s, double &time);
	~Player();
	void Update(Control ctrl);
	void Draw(double deltaTime);

	vec2 GetPos() { return position; };
	void SetPos(vec2 pos) { position = pos; }
	vec2 GetCenterPos() { return position + vec2(rigidbody.x / 2.f, rigidbody.y / 2.f); };
	vec4 GetCol() { return vec4(position, rigidbody.x, rigidbody.y); }
	void SetColStatus(CollisionStatus status)
	{
		isGround = status.isColDown;
		isTop = status.isColTop;
		downDistance = status.distance.y;
	}

private:
	Transform *pTx;
	Shader *shader;
	AnimSprite *sprite;
	SoundSystem *soundSys;
	Debug *debug;
	FSM *fsm;
	FSMInput fsmInput;

	float runSpeed, jumpSpeed;
	double &deltaTime;
	vec2 position, velocity, force, rigidbody;
	bool isGround, isTop;
	bool canJumpAttack;
	int ctrlX;
	DefferedKey dAttack, dChain, dJump;

	float downDistance;

	void loadData();
	void stateUpdate();
	void movement();
};
#endif