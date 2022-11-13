
#include "Texture.h"
#include "BufferObject.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include <GL/glew.h>

using namespace glm;

enum AnimationState
{
	Idle,
	Run,
	Jump
};

struct AnimationData
{
	AnimationState state;
	int width, height, frameCount;
	double secPerFrame;
};
class Animation
{
public:
	Animation(AnimationData data);

	void Play(Texture &texture, BufferObject &buffer, double deltatime);

	void setSecPerFrame(float sec);

private:
	double frameTime, secPerFrame;
	int frameIndex, frameCount;
	vector<vec4> frames;
};