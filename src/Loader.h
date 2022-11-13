
#include "Sprite.h"

#include <fstream>
#include <json.hpp>
#include <iostream>
using namespace std;
using json = nlohmann::json;

enum LoaderType
{
    Creature
};

class Loader
{
public:
    Loader(Sprite *sprite, string dataName);

private:
    void loadCreature(Sprite *sprite, json j);
};