
#pragma once
#include "Collision.h"
#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"
#include "Debug.h"
#include "Global.h"
#include "Sound.h"
#include "FSM.h"

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
	void Input(Control ctrl);
	void Draw(double deltaTime);

	vec2 GetPos() { return position; };
	void SetPos(vec2 pos) { position = pos; }
	vec2 GetCenterPos() { return position + vec2(rigidbody.x / 2.f, rigidbody.y / 2.f); };
	vec4 GetCol() { return vec4(position, rigidbody.x, rigidbody.y); }
	void SetColStatus(CollisionStatus status)
	{
		isGround = status.isColDown;
		isTop = status.isColTop;
	}

private:
	Transform *pTx;
	Shader *shader;
	AnimSprite *sprite;
	AnimationState state;
	SoundSystem *soundSys;
	Debug *debug;

	float runSpeed, jumpSpeed;
	double &deltaTime;
	vec2 position, velocity, rigidbody;
	bool isGround, isTop, isAttack;

	void loadData();
	void running(int dir);
	void falling();
	void setIdle();
	void setAttack();
	void setJump();
	void movement();
	void soundUpdate(Control ctrl);
	void animStateUpdate(Control ctrl);
};
#endif