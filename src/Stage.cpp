#include "Stage.h"

Stage::Stage(Player *pl, Shader *s) : player(pl), shader(s)
{
    bg = new Sprite();
    fg = new Sprite();

    json js = Loader::Load("L8.json", vector<Sprite *>{bg, fg});

    for (auto e : js["entry"])
    {
        entries.push_back(vec2(e["x"], e["y"]));
    }
}
Stage::~Stage()
{
    delete bg;
    delete fg;
}
void Stage::SetPlayerEntry(int index)
{
    player->setPos(entries[index]);
}
void Stage::DrawBG()
{
    shader->SetMat("modelMatrix", bg->Transform.Get());
    bg->Draw();
}

void Stage::DrawFG()
{
    shader->SetMat("modelMatrix", fg->Transform.Get());
    fg->Draw();
}