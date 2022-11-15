#include "Camera.h"

Camera::Camera(int w, int h, Player *pl, StageSystem *stSys)
	: width(w), height(h), player(pl), stageSys(stSys)
{
	UpdateProjection();
}
void Camera::UpdateProjection()
{
	vec2 boundary = stageSys->GetBoundary();
	vec2 pos = player->GetPos();
	vec2 scale = player->GetScale();
	float x = pos.x;
	float y = pos.y;

	x = clamp(x, width / 2.f - scale.x / 2, boundary.x - width / 2 - scale.x / 2);
	y = clamp(y, height / 2.f - scale.y / 2, boundary.y - height / 2 - scale.y / 2);

	float left = x - width / 2 + scale.x / 2;
	float right = x + width / 2 + scale.y / 2;
	float bottom = y + height / 2;
	float top = y - height / 2;

	projection = ortho(left, right, bottom, top, -1.0f, 1.0f);
}
mat4 Camera::Projection()
{
	UpdateProjection();
	return projection;
}
