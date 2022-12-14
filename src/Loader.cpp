#include "Loader.h"

// from ../data/
json Loader::Load(const string dataName)
{
    ifstream file("../data/" + dataName);
    return json::parse(file);
}
json Loader::Load(const string dataName, const vector<Sprite *> sprite)
{
    json j = Load(dataName);
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
        double secLastFrame = anim["secPerFrame"];
        if (anim.contains("secLastFrame"))
        {
            secLastFrame = anim["secLastFrame"];
        }
        string state = anim["state"];
        sprite[0]->Add(AnimationData{
            ActionStateText[state],
            anim["pivotX"],
            anim["pivotY"],
            anim["width"],
            anim["height"],
            anim["frameCount"],
            anim["secPerFrame"],
            secLastFrame,
            anim["isLoop"],
            anim["filename"]});
    }
}

void Loader::loadStage(const vector<Sprite *> sprite, json j)
{
    auto ob = j["background"];
    sprite[0]->Set(ob["filename"], vec2(ob["width"], ob["height"]));
    ob = j["frontground"];
    sprite[1]->Set(ob["filename"], vec2(ob["width"], ob["height"]));
}