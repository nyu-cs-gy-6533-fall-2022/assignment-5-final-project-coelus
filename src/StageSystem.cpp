#include "StageSystem.h"

StageSystem::StageSystem(Player *pl, Shader *shader) : player(pl)
{

    json js = Loader::Load("game.json");
    for (string name : js["stage"])
    {
        stages[name] = new Stage(name + ".json", player, shader);
    }
}

void StageSystem::SetPlayerEntry(string name, int index)
{
    currentStage = stages[name];
    currentStage->SetPlayerEntry(index);
}
void StageSystem::Update()
{
    currentStage->Update();
}
void StageSystem::DrawFG()
{
    currentStage->DrawFG();
}
void StageSystem::DrawBG()
{
    currentStage->DrawBG();
}