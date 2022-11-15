
#pragma once
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
struct Entry
{
	int spawnX, spawnY, x, y, w, h;
	string nextStage;
	int nextEntry;

	vec2 GetSpawnPos()
	{
		return vec2(spawnX, spawnY);
	};
	vec4 GetTrigger()
	{
		return vec4(x, y, w, h);
	};
};

class Stage
{
public:
	Stage(string filename, Player *pl, Shader *s);

	~Stage();

	void Update();
	void DrawFG();
	void DrawBG();

	void SetPlayerEntry(int index);

private:
	Debug *debug;
	Sprite *bg;
	Sprite *fg;
	Player *player;
	Shader *shader;

	vector<vec4> collisions;
	vector<Entry> entries;

	void loadData(string filename);
	void updateTrigger();
	void updateCollision();
};
#endif