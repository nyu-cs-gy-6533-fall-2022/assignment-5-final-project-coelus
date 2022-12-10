
#pragma once
#include "creature/Player.h"
#include "creature/Snail.h"
#include "creature/Rat.h"
#include "Loader.h"
#include "Sprite.h"
#include "Shader.h"
#include "Collision.h"
#include "Debug.h"
#include "Sound.h"
#include "Hitbox.h"
#include "Light.h"
#include "Tube.h"
#include <LightData.h>

#include <json.hpp>
using json = nlohmann::json;

#include <GLFW/glfw3.h>

#ifndef _STAGE_
#define _STAGE_

struct NextStage
{
	bool isReady = false;
	string nextStage = "";
	int nextEntry = -1;
	void Init()
	{
		isReady = false;
		nextStage = "";
		nextEntry = -1;
	}
};
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
	Stage(string filename, SoundSystem *sndSys, Player *pl, vector<Shader*> &s, double &time);

	~Stage();

	void Update();
	void Draw();
	void SetPlayerEntry(int index);
	void PlayBGM();
	vec2 GetBoundary();

	NextStage NextStage;

private:
	Debug *debug;
	Sprite *bg;
	Sprite *fg;
	Player *player;
	vector<LightData> lightData;
	vector<Light *> lights;
	vector<Tube *> tubes;
	vector<Creature *> monsters;
	vector<Shader*> &shaders;
	SoundSystem *soundSys;
	double &deltaTime;
	SoundType bgm;

	vector<vec4> tubeTriggers;
	vector<vec4> collisions;
	vector<vec4> entriesvec4;
	vector<Entry> entries;

	void loadData(string filename);
	void useLightShader();
	void drawFG();
	void drawBG();
	void drawLights();
	void drawTubes();
	void drawMonsters();
	void updateMonsters();
	void updateTrigger();
	void updateCollision();
};
#endif