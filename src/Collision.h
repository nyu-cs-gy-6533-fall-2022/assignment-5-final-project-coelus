

#include <glm/glm.hpp>
#include <vector>
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
};

class Collision
{
public:
    static CollisionType GetOverlapType(vec4 r1, vec4 r2, float &err);
    static void ResolveCollision(vec2 &pos, CollisionType type, float resErr);
    static CollisionStatus CollisonSystem(vec2 &targetPos, vec4 r0, vector<vec4> rects);
    static bool IsCollided(vec4 main, vector<vec4> rects);
    static bool IsCollided(vec4 r1, vec4 r2);
};
#endif