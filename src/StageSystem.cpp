#include "StageSystem.h"

StageSystem::StageSystem(SoundSystem *sndSys, Player *pl, vector<Shader *> &shaders, double &time)
    : soundSys(sndSys), player(pl), deltaTime(time)
{

    json js = Loader::Load("game.json");
    for (string name : js["stages"])
    {
        stages[name] = new Stage(name + ".json", soundSys, player, shaders, time);
    }
    startStage = js["startStage"];
    startEntry = js["startEntry"];
    SetPlayerEntry(startStage, startEntry);
}

void StageSystem::SetPlayerEntry(string name, int index)
{
    currentStage = stages[name];
    currentStage->PlayBGM();
    currentStage->SetPlayerEntry(index);
}
void StageSystem::Update()
{

    NextStage *nt = &currentStage->NextStage;
    if (player->IsRespawn())
    {
        player->RespawnDone();
        SetPlayerEntry(startStage, startEntry);
        nt->Init();
    }
    else if (nt->isReady)
    {
        SetPlayerEntry(nt->nextStage, nt->nextEntry);
        nt->Init();
    }
    currentStage->Update();
}
void StageSystem::Draw()
{
    currentStage->Draw();
}

vec2 StageSystem::GetBoundary()
{
    return currentStage->GetBoundary();
}