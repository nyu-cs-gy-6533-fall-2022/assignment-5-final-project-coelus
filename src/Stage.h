
#include "Player.h"
#include "Loader.h"
#include "Sprite.h"
#include "Shader.h"
#include <json.hpp>
using json = nlohmann::json;

#ifndef _STAGE_
#define _STAGE_
class Stage
{
public:
	Stage(Player *pl, Shader *s);

	~Stage();
	void SetPlayerEntry(int index);
	void DrawFG();
	void DrawBG();

private:
	vector<Sprite *> colDebug;
	vector<vec4> collisions;
	vector<vec2> entries;
	Sprite *bg;
	Sprite *fg;
	Player *player;
	Shader *shader;
	void loadData();
	void drawCollision();
};
#endif