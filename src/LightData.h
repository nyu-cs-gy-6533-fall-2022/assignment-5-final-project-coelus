
#include <glm/glm.hpp>
#pragma once

enum LightType
{
    LightDirect,
    LightPoint
};
struct LightData
{
    LightType type;
    vec2 pos;
    vec2 dir;
};