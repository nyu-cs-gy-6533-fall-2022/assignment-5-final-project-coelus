
#include "Loader.h"
#include "Sprite.h"
#include "Shader.h"

#ifndef _STAGE_
#define _STAGE_
class Stage
{
public:
	Stage(Shader *s);

	~Stage();

	void DrawFG();
	void DrawBG();

private:
	Sprite *bg;
	Sprite *fg;
	Shader *shader;
};
#endif