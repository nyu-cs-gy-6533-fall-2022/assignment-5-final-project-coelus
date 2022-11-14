#include "Loader.h"

// from ../data/
Loader::Loader(string dataName, const vector<Sprite*> sprite)
{
    ifstream file("../data/" + dataName);
    json j = json::parse(file);
    LoaderType type = (LoaderType)j["type"];
    switch (type)
    {
    case CreatureType:
        loadCreature(sprite, j);
        break;
    case SceneType:
        loadStage(sprite, j);
        break;
    default:
        break;
    }
}
void Loader::loadCreature(const vector<Sprite*> sprite, json j)
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

void Loader::loadStage(const vector<Sprite*> sprite, json j)
{
    auto ob = j["background"];
    sprite[0]->Set(ob["filename"], vec2(ob["width"], ob["height"]));
    ob = j["frontground"];
    sprite[1]->Set(ob["filename"], vec2(ob["width"], ob["height"]));
}