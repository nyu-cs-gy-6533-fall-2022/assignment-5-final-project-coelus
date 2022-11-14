#include "Stage.h"

Stage::Stage(Player *pl, Shader *s) : player(pl), shader(s)
{
    bg = new Sprite();
    fg = new Sprite();
    loadData();
}
Stage::~Stage()
{
    delete bg;
    delete fg;
}
void Stage::loadData()
{
    json js = Loader::Load("L8.json", vector<Sprite *>{bg, fg});

    for (auto e : js["entry"])
    {
        entries.push_back(vec2(e["x"], e["y"]));
    }
    for (auto col : js["collision"])
    {
        Sprite *s = new Sprite();
        s->Set("red.png", vec2(col["w"], col["h"]));
        s->Transform.Position = vec2(col["x"], col["y"]);
        colDebug.push_back(s);
        collisions.push_back(vec4(col["x"], col["y"], col["w"], col["h"]));
    }
}
void Stage::SetPlayerEntry(int index)
{
    player->setPos(entries[index]);
}
void Stage::drawCollision()
{
    for (auto col : colDebug)
    {
        shader->SetMat("modelMatrix", col->Transform.Get());
        col->Draw();
    }
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
    drawCollision();
}
