
#include "Sprite.h"
#include "Loader.h"
using namespace glm;

class Player
{
public:
	Player();

	~Player();

	void Move(int dir);

	void Stop();

	mat4 Transform();

	void Draw(double deltatime);


private:
	float walkSpeed;
	Loader* loader;
	Sprite* sprite;
	AnimationState state;
};