
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
	int centerX, centerY, width, height, frameCount;
	double secPerFrame;
	string filename;
};
class Animation
{
public:
	Animation(AnimationData data);
	~Animation();

	void Play(BufferObject &buffer, double deltatime);

	void SetSecPerFrame(float sec);

	Texture *texture;

private:
	double frameTime, secPerFrame;
	int frameIndex, frameCount;

	vector<vec4> frames;
};