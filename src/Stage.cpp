#include "Stage.h"

Stage::Stage(string filename, SoundSystem *sndSys, Player *pl, vector<Shader *> &s, double &time)
    : player(pl), shaders(s), soundSys(sndSys), deltaTime(time)
{
    bg = new Sprite();
    fg = new Sprite();
    debug = new Debug(shaders);
    loadData(filename);
}
Stage::~Stage()
{
    delete bg;
    delete fg;
    delete debug;

    for (int i = 0; i < lights.size(); i++)
    {
        delete lights[i];
    }

    for (int i = 0; i < tubes.size(); i++)
    {
        delete tubes[i];
    }
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
            c = new Snail(soundSys, shaders, deltaTime);
        }
        else if (monster["name"] == "rat")
        {
            c = new Rat(soundSys, shaders, deltaTime);
        }
        c->SetInitPos(vec2(monster["x"], monster["y"]));
        monsters.push_back(c);
    }

    for (auto lightdata : js["lights"])
    {
        Light *light = new Light(shaders, lightdata["type"], vec2(lightdata["x"], lightdata["y"]));
        lights.push_back(light);
        lightData.push_back(light->GetLightData());
    }

    for (auto tubedata : js["tubes"])
    {
        Tube *tube = new Tube(shaders, tubedata["type"], vec2(tubedata["x"], tubedata["y"]));
        tubes.push_back(tube);
        tubeTriggers.push_back(tube->GetCol());
    }
}
void Stage::SetPlayerEntry(int index)
{
    player->SetPos(entries[index].GetSpawnPos());
    for (auto m : monsters)
    {
        m->Reset();
    }
}
void Stage::Update()
{
    updateMonsters();
    updateTrigger();
    updateCollision();
}
void Stage::Draw()
{
    useLightShader();
    drawBG();
    player->DrawChain();
    useLightShader();
    drawFG();
    drawLights();
    drawMonsters();
    drawTubes();
    player->Draw();
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
    // hitbox to monsters
    for (auto monster : monsters)
    {
        resIndex = -1;
        vector<HitboxData> hitboxs = player->GetHitbox();
        if (Collision::IsCollided(monster->GetCol(), hitboxs, resIndex))
        {
            monster->SetDamage(player, hitboxs[resIndex]);
        }
    }

    // hitbox to player
    for (auto monster : monsters)
    {
        resIndex = -1;
        vector<HitboxData> hitboxs = monster->GetHitbox();
        if (Collision::IsCollided(player->GetCol(), hitboxs, resIndex))
        {
            player->SetDamage(monster, hitboxs[resIndex]);
        }
    }

    // player chain wall
    if (Collision::IsCollided(player->GetChainCol(), collisions, resIndex))
    {
        player->SetChainHit(MatWall);
    }
    // player chain tube
    if (Collision::IsCollided(player->GetChainCol(), tubeTriggers, resIndex))
    {
        player->SetChainHit(MatTube);
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

void Stage::useLightShader()
{
    shaders[2]->Use();
    shaders[2]->SetMat4("modelMatrix", bg->Tx.Get());
    shaders[2]->SetVec2("resolution", bg->Tx.scale);
    shaders[2]->SetInt("lightLen", lightData.size());
    shaders[2]->SetVec4("lights", lightData);
    shaders[2]->SetFloat("time", glfwGetTime());
}
void Stage::drawBG()
{
    bg->Draw();
}

void Stage::drawFG()
{
    fg->Draw();
    debug->DrawDebug();
}
void Stage::drawLights()
{

    for (auto light : lights)
    {
        light->Draw();
    }
}
void Stage::drawTubes()
{

    for (auto tube : tubes)
    {
        tube->Draw();
    }
}

void Stage::drawMonsters()
{

    for (auto m : monsters)
    {
        m->Draw();
    }
}
