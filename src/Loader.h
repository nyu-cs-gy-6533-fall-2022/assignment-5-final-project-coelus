
#include "Sprite.h"

#include <fstream>
#include <json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

#ifndef _LOADER_
#define _LOADER_
enum LoaderType
{
    CreatureType,
    SceneType
};

class Loader
{
public:
    static json Load(const string dataName);
    static json Load(const string dataName, const vector<Sprite *> sprite);

private:
    static void loadCreature(const vector<Sprite *> sprite, json j);
    static void loadStage(const vector<Sprite *> sprite, json j);
};
#endif