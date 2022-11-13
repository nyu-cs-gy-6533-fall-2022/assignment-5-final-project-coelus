

#pragma once
#include <glm/glm.hpp>


#include <iostream>


using namespace std;
class Texture
{
public:
	Texture(string filepath);

	~Texture();

	void Bind();

	int GetWidth();

	int GetHeight();

	unsigned char* GetData();


private:
	int width, height, channels;
	unsigned char* data;
	unsigned int texID;
};