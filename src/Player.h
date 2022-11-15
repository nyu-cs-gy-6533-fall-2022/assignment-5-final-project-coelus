
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
class Player
{
public:
	Player(Shader *s, double &time);
	~Player();
	void Control(bool right, bool left, bool up, bool down);
	void Draw(double deltaTime);

	vec2 GetPos() { return pTx->Position; };
	void SetPos(vec2 pos) { pTx->Position = pos; }
	vec4 GetCol()
	{
		return vec4(pTx->Position + vec2(rigidbody.x, rigidbody.y), rigidbody.z, rigidbody.w);
	}
	void SetGround(bool flag) { isGround = flag; }

private:
	Transform *pTx;
	Shader *shader;
	AnimSprite *sprite;
	AnimationState state;

	float runSpeed;
	double &deltaTime;
	bool isGround = true;
	vec2 vectorSpd;
	vec4 rigidbody;
	Debug *debug;

	void loadData();
	void running(int dir);
	void falling();
	void setIdle();
	void movement();
	void animStateUpdate(bool right, bool left, bool up, bool down);
	
};
#endif