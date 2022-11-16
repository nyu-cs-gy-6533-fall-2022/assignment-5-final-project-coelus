
#pragma once
#include "Collision.h"
#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"
#include "Debug.h"
#include "Global.h"

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
	Player(Shader *s, double &time);
	~Player();
	void Input(Control ctrl);
	void Draw(double deltaTime);

	vec2 GetScale() { return pTx->Scale; };
	vec2 GetPos() { return pTx->Position; };
	void SetPos(vec2 pos) { pTx->Position = pos; }
	vec2 GetCenterPos() { return pTx->GetCenterPos(); };
	vec4 GetCol()
	{
		return vec4(pTx->Position + vec2(rigidbody.x, rigidbody.y), rigidbody.z, rigidbody.w);
	}
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

	float runSpeed, jumpSpeed;
	double &deltaTime;
	vec2 vectorSpd;
	vec4 rigidbody;
	Debug *debug;
	bool isGround, isTop;

	void loadData();
	void running(int dir);
	void falling();
	void setIdle();
	void setJump();
	void movement();
	void animStateUpdate(Control ctrl);
};
#endif