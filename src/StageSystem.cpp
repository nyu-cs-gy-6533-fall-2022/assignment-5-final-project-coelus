#include "StageSystem.h"

StageSystem::StageSystem(SoundSystem *sndSys, Player *pl, vector<Shader*> &shaders, double &time)
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
    soundSys->Play(BGMStage1);
}

void StageSystem::SetPlayerEntry(string name, int index)
{
    currentStage = stages[name];
    currentStage->SetPlayerEntry(index);
}
void StageSystem::Update()
{
    
    NextStage *nt = &currentStage->NextStage;
    if (nt->isReady)
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