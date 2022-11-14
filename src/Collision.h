

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
    ColDown
};

class Collision
{
public:
    static bool DetectOverlap(vec4 a,vec4 b);

private:
};
#endif