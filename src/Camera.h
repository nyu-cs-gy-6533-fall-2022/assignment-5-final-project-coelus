#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
	Camera(int width, int height);

	mat4 Projection();
	void UpdateProjection(int width, int height);

private:
	mat4 projection;
};