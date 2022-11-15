
#pragma once
#include "Player.h"
#include "Loader.h"
#include "Shader.h"
#include <json.hpp>
#include "Stage.h"

using json = nlohmann::json;

#ifndef _STAGESYS_
#define _STAGESYS_
class StageSystem
{
public:
    StageSystem(Player *pl, Shader *s);

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
    Stage *currentStage;
    unordered_map<string, Stage *> stages;
    Player *player;
};
#endif