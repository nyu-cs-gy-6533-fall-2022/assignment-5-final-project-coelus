#include "Player.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
	Camera(int w, int h, Player *player);

	mat4 Projection();
	void UpdateProjection();

private:
	int width, height;
	Player *player;
	mat4 projection;
};