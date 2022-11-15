
#pragma once
#include "Collision.h"
#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"

using namespace glm;
class Player
{
public:
	Player(Shader *s, double &time);
	~Player();
	void Control(bool right, bool left, bool up, bool down);

	void Draw(double deltaTime);
	void SetPos(vec2 pos);
	vec2 GetPos();
	
private:
	Transform *pTx;
	Shader *shader;
	AnimSprite *sprite;
	AnimationState state;

	float runSpeed;
	double &deltaTime;
	bool isGround = true;
	vec2 vectorSpd;
	vec4 collision;
	Sprite *colDebug;

	void loadData();
	void running(int dir);
	void falling();
	void setIdle();
	void movement();
};
