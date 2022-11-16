
#pragma once
#include "Player.h"
#include "Loader.h"
#include "Shader.h"
#include "Stage.h"
#include "Sound.h"

#include <json.hpp>
using json = nlohmann::json;

#ifndef _STAGESYS_
#define _STAGESYS_
class StageSystem
{
public:
    StageSystem(SoundSystem *sndSys, Player *pl, Shader *s);

    ~StageSystem()
    {
        for (auto stage : stages)
        {
            delete stage.second;
        }
    };

    void Update();
    void DrawFG();
    void DrawBG();
    void SetPlayerEntry(string name, int index);
    vec2 GetBoundary();

private:
    SoundSystem *soundSys;
    Stage *currentStage;
    unordered_map<string, Stage *> stages;
    Player *player;
    string startStage;
    int startEntry;
};
#endif