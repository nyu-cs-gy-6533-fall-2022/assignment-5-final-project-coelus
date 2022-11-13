
#include "Texture.h"
#include "Transform.h"
#include "Shader.h"
#include "BufferObject.h"

#ifndef _STAGE_
#define _STAGE_
class Stage
{
public:
	Stage(Shader *s);

	~Stage();

	void Draw();

private:
	Transform transfrom;
	Shader *shader;
	Texture *texture;
	BufferObject *buffer;
};
#endif