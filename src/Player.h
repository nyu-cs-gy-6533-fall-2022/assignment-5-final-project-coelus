


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
	Player(Shader *s);

	~Player();
	void Move(int dir);
	void MoveY(int dir);
	void Stop();
	void Draw(double deltatime);
	void setPos(vec2 pos);
	vec2 getPos();

private:
	Transform *transform;
	float walkSpeed;
	Shader *shader;
	AnimSprite *sprite;
	AnimationState state;
};
#endif