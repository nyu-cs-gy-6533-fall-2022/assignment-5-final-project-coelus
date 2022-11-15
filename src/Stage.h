
#include "Player.h"
#include "Loader.h"
#include "Sprite.h"
#include "Shader.h"
#include "Collision.h"
#include "Debug.h"
#include <json.hpp>
using json = nlohmann::json;

#ifndef _STAGE_
#define _STAGE_
class Stage
{
public:
	Stage(Player *pl, Shader *s);

	~Stage();
	
	void Update();
	void DrawFG();
	void DrawBG();

	void SetPlayerEntry(int index);

private:
	vector<vec4> collisions;
	vector<vec4> entries;
	vector<vec2> spawn;

	Debug *debug;
	Sprite *bg;
	Sprite *fg;
	Player *player;
	Shader *shader;
	void loadData();
	void updateCollision();

};
#endif