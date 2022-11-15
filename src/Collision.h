

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

class Collision
{
public:
    static CollisionType GetOverlapType(vec4 r1, vec4 r2);
    static bool IsCollided(vec4 main, vector<vec4> rects);
    static bool IsCollided(vec4 r1, vec4 r2);
};
#endif