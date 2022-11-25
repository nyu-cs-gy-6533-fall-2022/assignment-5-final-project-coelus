
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Hitbox.h"

using namespace std;
using namespace glm;

#ifndef _Collision_
#define _Collision_

enum CollisionType
{
    ColNone,
    ColRight,
    ColLeft,
    ColTop,
    ColDown
};

struct CollisionStatus
{

    bool isColRight = false;
    bool isColLeft = false;
    bool isColTop = false;
    bool isColDown = false;
    vec4 distance;
};

class Collision
{
public:
    static CollisionType GetOverlapType(vec4 r1, vec4 r2, float &err, vec4 &distance);
    static void ResolveCollision(vec2 &pos, CollisionType type, float resErr);
    static CollisionStatus CollisonSystem(vec2 &targetPos, vec4 r0, const vector<vec4> &rects);

    // for stage
    static bool IsCollided(vec4 r0, const vector<HitboxData> &hitboxs, int &resIndex);
    static bool IsCollided(vec4 r0, const vector<vec4> &rects, int &resIndex);
    static bool IsCollided(vec4 r1, vec4 r2);
};
#endif