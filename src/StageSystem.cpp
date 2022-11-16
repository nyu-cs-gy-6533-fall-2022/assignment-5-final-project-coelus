#include "StageSystem.h"

StageSystem::StageSystem(Player *pl, Shader *shader) : player(pl)
{

    json js = Loader::Load("game.json");
    for (string name : js["stages"])
    {
        stages[name] = new Stage(name + ".json", player, shader);
    }
    startStage = js["startStage"];
    startEntry = js["startEntry"];
    SetPlayerEntry(startStage, startEntry);
}

void StageSystem::SetPlayerEntry(string name, int index)
{
    currentStage = stages[name];
    currentStage->SetPlayerEntry(index);
}
void StageSystem::Update()
{
    currentStage->Update();
    NextStage *nt = &currentStage->NextStage;
    if (nt->isReady)
    {
        SetPlayerEntry(nt->nextStage, nt->nextEntry);
        nt->Init();
    }
}
void StageSystem::DrawFG()
{
    currentStage->DrawFG();
}
void StageSystem::DrawBG()
{
    currentStage->DrawBG();
}
vec2 StageSystem::GetBoundary()
{
    return currentStage->GetBoundary();
}