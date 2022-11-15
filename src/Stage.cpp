#include "Stage.h"

Stage::Stage(Player *pl, Shader *s) : player(pl), shader(s)
{
    bg = new Sprite();
    fg = new Sprite();
    debug = new Debug(shader);
    loadData();
}
Stage::~Stage()
{
    delete bg;
    delete fg;
    delete debug;
}
void Stage::loadData()
{
    json js = Loader::Load("L8.json", vector<Sprite *>{bg, fg});

    for (auto e : js["entry"])
    {
        spawn.push_back(vec2(e["spawnX"], e["spawnY"]));
        vec4 newEntry = vec4(e["x"], e["y"], e["w"], e["h"]);
        entries.push_back(newEntry);
        debug->AddDebug(newEntry);
    }
    for (auto col : js["collision"])
    {
        Sprite *s = new Sprite();
        vec4 newCol = vec4(col["x"], col["y"], col["w"], col["h"]);
        collisions.push_back(newCol);
        debug->AddDebug(newCol);
    }
}
void Stage::SetPlayerEntry(int index)
{
    player->SetPos(spawn[index]);
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
    debug->DrawDebug();
}

