#pragma once
#include "Sound.h"
#include <glm/glm.hpp>
using namespace glm;
struct HitboxData
{
    vec4 hitboxs;
    vec2 force;
    int damage;
    float time;
    SoundType sound;
    
};

struct DamageData
{
    SoundType sound;
    vec2 force;
    int dirX;
    int losthp;
    
};