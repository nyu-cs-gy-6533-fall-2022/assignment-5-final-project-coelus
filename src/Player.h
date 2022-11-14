

#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"

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
	void SetPos(vec2 pos);
	vec2 GetPos();
	
private:
	
	Transform *pTx;
	vec2 vectorSpd;
	float runSpeed;
	Shader *shader;
	AnimSprite *sprite;
	AnimationState state;
	double &deltaTime;
	bool isColX,isColY;

	void loadData();
	void running(int dir);
	void falling();
	void setIdle();
	void movement();
};
#endif