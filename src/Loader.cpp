#include "Loader.h"

// from ../data/
Loader::Loader(Sprite *sprite, string dataName)
{
    ifstream file("../data/" + dataName);
    json j = json::parse(file);
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

        sprite->Add(animData);
    }
}