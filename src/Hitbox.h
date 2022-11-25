#pragma once
#include <glm/glm.hpp>
using namespace glm;
struct HitboxData
{
    vec4 hitboxs;
    vec2 force;
    int damage;
    float time;
    
};

struct DamageData
{
    vec2 force;
    int dirX;
    int losthp;
    
};