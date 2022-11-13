#include "Camera.h"

Camera::Camera(int width, int height)
{
	UpdateProjection(width, height);
}
void Camera::UpdateProjection(int width, int height)
{
	float left = 0.0f - width / 2;
	float right = width - width / 2;
	float bottom = height - height / 2;
	float top = 0.0f - height / 2;
	projection = ortho(left, right, bottom, top, -1.0f, 1.0f);
}
mat4 Camera::Projection()
{
	return projection;
}
