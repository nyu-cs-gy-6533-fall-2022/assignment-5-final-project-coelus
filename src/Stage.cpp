#include "Stage.h"

Stage::Stage(string filename, Player *pl, Shader *s)
    : player(pl), shader(s)
{
    bg = new Sprite();
    fg = new Sprite();
    debug = new Debug(shader);
    loadData(filename);
}
Stage::~Stage()
{
    delete bg;
    delete fg;
    delete debug;
}
void Stage::loadData(string filename)
{
    json js = Loader::Load(filename, vector<Sprite *>{bg, fg});

    for (auto e : js["entry"])
    {
        Entry entry = Entry{
            e["spawnX"],
            e["spawnY"],
            e["x"],
            e["y"],
            e["w"],
            e["h"],
            e["nextStage"],
            e["nextEntry"]};
        entries.push_back(entry);
        entriesvec4.push_back(entry.GetTrigger());
        debug->AddDebug(entry.GetTrigger());
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
    player->SetPos(entries[index].GetSpawnPos());
}
void Stage::Update()
{
    updateTrigger();
    updateCollision();
}

void Stage::updateTrigger()
{
    // entry detect

    int resIndex = -1;
    if (Collision::IsCollided(player->GetCol(), entriesvec4, resIndex))
    {
        NextStage.isReady = true;
        NextStage.nextStage = entries[resIndex].nextStage;
        NextStage.nextEntry = entries[resIndex].nextEntry;
    }
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

vec2 Stage::GetBoundary()
{
    return bg->Tx.Scale;
}
