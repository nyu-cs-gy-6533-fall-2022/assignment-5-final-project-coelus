#include "Stage.h"

Stage::Stage(string filename, SoundSystem *sndSys, Player *pl, Shader *s, double &time)
    : player(pl), shader(s), soundSys(sndSys), deltaTime(time)
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
    for (auto monster : js["monsters"])
    {
        Creature *c;
        if (monster["name"] == "snail")
        {
            c = new Snail(soundSys, shader, deltaTime);
        }

        c->SetPos(vec2(monster["x"], monster["y"]));
        monsters.push_back(c);
    }
}
void Stage::SetPlayerEntry(int index)
{
    player->SetPos(entries[index].GetSpawnPos());
}
void Stage::Update()
{
    updateMonsters();
    updateTrigger();
    updateCollision();
    
}
void Stage::Draw()
{
    drawBG();
    drawFG();
    drawMonsters();
}
vec2 Stage::GetBoundary()
{
    return bg->Tx.scale;
}

void Stage::updateMonsters()
{
    for (auto m : monsters)
    {
        m->Update(Control{});
    }
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
    player->SetColStatus(status);

    for (auto monster : monsters)
    {
        vec2 pos = monster->GetPos();
        CollisionStatus status = Collision::CollisonSystem(pos, monster->GetCol(), collisions);
        monster->SetPos(pos);
        monster->SetColStatus(status);
        vec4 rect = monster->GetCol();
        rect += vec4(rect.z * monster->GetTx().dirX, 0, 0, 0);
        CollisionStatus predictStatus = Collision::CollisonSystem(pos, rect, collisions);
        monster->SetPreditColStatus(predictStatus);
    }
}

void Stage::drawBG()
{
    shader->SetMat("modelMatrix", bg->Tx.Get());
    bg->Draw();
}

void Stage::drawFG()
{
    shader->SetMat("modelMatrix", fg->Tx.Get());
    fg->Draw();
    debug->DrawDebug();
}
void Stage::drawMonsters()
{
    for (auto m : monsters)
    {
        m->Draw();
    }
}
