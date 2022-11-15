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
    for (auto col : colDebug)
    {
        delete col;
    }
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
        collisions.push_back(vec4(col["x"], col["y"], col["w"], col["h"]));
#if DEBUG
        s->Set("red.png", vec2(col["w"], col["h"]), vec2(col["x"], col["y"]));
        colDebug.push_back(s);
#endif
    }
}
void Stage::SetPlayerEntry(int index)
{
    player->SetPos(entries[index]);
}
void Stage::Update()
{
    updateCollision();
}

void Stage::updateCollision()
{
    vec2 plPos = player->GetPos();
    CollisionStatus status = Collision::CollisonSystem(plPos, player->GetCol(), collisions);
    player->SetPos(plPos);
    player->SetGround(status.isColDown);
}

void Stage::DrawBG()
{
    shader->SetMat("modelMatrix", bg->Tx.Get());
    bg->Draw();
}

void Stage::DrawFG()
{
    shader->SetMat("modelMatrix", fg->Tx.Get());
    fg->Draw();
#if DEBUG
    drawCollision();
#endif
}

#if DEBUG
void Stage::drawCollision()
{
    for (auto col : colDebug)
    {
        shader->SetMat("modelMatrix", col->Tx.Get());
        col->Draw();
    }
}
#endif