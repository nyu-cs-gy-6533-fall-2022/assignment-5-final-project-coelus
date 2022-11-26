#include "creature/Player.h"
#include "StageSystem.h"

#include <glm/glm.hpp>

using namespace glm;

class Camera
{
public:
	Camera(StageSystem *stSys, Player *pl, int w, int h);

	mat4 Projection();
	void UpdateProjection();

private:
	int width, height;
	Player *player;
	StageSystem *stageSys;
	mat4 projection;
};