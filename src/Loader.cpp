#include "Loader.h"

// from ../data/
json Loader::Load(string dataName, const vector<Sprite *> sprite)
{
    ifstream file("../data/" + dataName);
    json j = json::parse(file);
    LoaderType type = (LoaderType)j["type"];
    switch (type)
    {
    case CreatureType:
        Loader::loadCreature(sprite, j);
        break;
    case SceneType:
        Loader::loadStage(sprite, j);
        break;
    default:
        break;
    }
    return j;
}
void Loader::loadCreature(const vector<Sprite *> sprite, json j)
{

    for (auto anim : j["animation"])
    {
        AnimationData animData = {
            anim["state"],
            anim["centerX"],
            anim["centerY"],
            anim["width"],
            anim["height"],
            anim["frameCount"],
            anim["secPerFrame"],
            anim["filename"]};

        sprite[0]->Add(animData);
    }
}

void Loader::loadStage(const vector<Sprite *> sprite, json j)
{
    auto ob = j["background"];
    sprite[0]->Set(ob["filename"], vec2(ob["width"], ob["height"]), vec2(0, 0));
    ob = j["frontground"];
    sprite[1]->Set(ob["filename"], vec2(ob["width"], ob["height"]), vec2(0, 0));
}