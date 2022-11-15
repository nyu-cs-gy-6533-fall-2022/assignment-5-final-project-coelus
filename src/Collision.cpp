#include "Collision.h"

// return CollisionType
CollisionType Collision::GetOverlapType(vec4 r1, vec4 r2)
{
    float r1l = r1.x - r1.z / 2.f;
    float r1r = r1.x + r1.z / 2.f;
    float r1t = r1.y - r1.w / 2.f;
    float r1b = r1.y + r1.w / 2.f;
    float r2l = r2.x - r2.z / 2.f;
    float r2r = r2.x + r2.z / 2.f;
    float r2t = r2.y - r2.w / 2.f;
    float r2b = r2.y + r2.w / 2.f;

    bool notOverlapL = r2l > r1r;
    bool notOverlapR = r2r < r1l;
    bool notOverlapT = r2t > r1b;
    bool notOverlapB = r2b < r1t;
    bool isOverlap = !(notOverlapL || notOverlapR || notOverlapT || notOverlapB);

    if (isOverlap)
    {
        float err[] = {abs(r1b - r2t), abs(r1l - r2r) * 2, abs(r1r - r2l) * 2};
    }
    else
    {
        return ColNone;
    }
}

bool Collision::IsCollided(vec4 r1, vector<vec4> rects)
{
    bool isCol = false;
    for (auto r : rects)
    {
        if (Collision::IsCollided(r1, r))
        {
            isCol = true;
            break;
        }
    }
    return isCol;
}
// return bool
bool Collision::IsCollided(vec4 r1, vec4 r2)
{
    return (r1.x < r2.x + r2.z &&
            r1.x + r1.z > r2.x &&
            r1.y < r2.y + r2.w &&
            r1.y + r1.w > r2.y);
}