


#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"

using namespace glm;

class Player
{
public:
	Player(Shader *s);

	~Player();
	void Move(int dir);
	void Stop();
	void Draw(double deltatime);

private:
	Transform *transform;
	float walkSpeed;
	Shader *shader;
	Loader *loader;
	Sprite *sprite;
	AnimationState state;
};