#include "Camera.h"

Camera::Camera(int w, int h, Player *pl) : width(w), height(h), player(pl)
{
	UpdateProjection();
}
void Camera::UpdateProjection()
{
	vec2 pos = player->GetPos();
	vec2 scale = player->GetScale();
	float x = pos.x;
	float y = pos.y;
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
