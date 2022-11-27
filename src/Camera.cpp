#include "Camera.h"

Camera::Camera(StageSystem *stSys, Player *pl, int w, int h)
	: width(w), height(h), player(pl), stageSys(stSys)
{
	UpdateProjection();
}
void Camera::UpdateProjection()
{
	vec2 boundary = stageSys->GetBoundary();
	vec2 pos = player->GetCenterPos();
	float plX = pos.x;
	float plY = pos.y;

	plX = clamp(plX, width / 2.f, boundary.x - width / 2);
	plY = clamp(plY, height / 2.f , boundary.y - height / 2);

	float left = plX - width / 2;
	float right = plX + width / 2;
	float top = plY - height / 2;
	float bottom = plY + height / 2;

	projection = ortho(left, right, bottom, top, -1.0f, 1.0f);
}
mat4 Camera::Projection()
{
	UpdateProjection();
	return projection;
}
