#include "BufferObject.h"

BufferObject::BufferObject()
{

	vector<vec2> vertex = {
		vec2(0, 0),
		vec2(0, 1),
		vec2(1, 0),
		vec2(1, 0),
		vec2(0, 1),
		vec2(1, 1)};

	vector<vec2> uv = {
		vec2(0, 0),
		vec2(0, 1),
		vec2(1, 0),
		vec2(1, 0),
		vec2(0, 1),
		vec2(1, 1)};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &UVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, NULL);

	UpdateUVBO(uv);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

BufferObject::~BufferObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UVBO);
}
void BufferObject::UpdateUVBO(const vector<vec2> &uv)
{
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uv.size(), uv.data(), GL_DYNAMIC_DRAW);
}

void BufferObject::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
