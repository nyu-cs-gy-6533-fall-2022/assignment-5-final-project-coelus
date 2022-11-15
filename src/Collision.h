

#include <glm/glm.hpp>

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
    static CollisionType GetOverlapType(vec4 a, vec4 b);
    static bool IsCollided(vec4 a, vec4 b);
};
#endif