#include "Collision.h"

// return CollisionType
CollisionType Collision::GetOverlapType(vec4 r0, vec4 r1, float &resErr)
{
    float r0l = r0.x;
    float r0r = r0.x + r0.z;
    float r0t = r0.y;
    float r0b = r0.y + r0.w;
    float r1l = r1.x;
    float r1r = r1.x + r1.z;
    float r1t = r1.y;
    float r1b = r1.y + r1.w;

    bool notOverlapL = r1l > r0r;
    bool notOverlapR = r1r < r0l;
    bool notOverlapT = r1t > r0b;
    bool notOverlapB = r1b < r0t;
    bool isOverlap = !(notOverlapL || notOverlapR || notOverlapT || notOverlapB);

    if (isOverlap)
    {
        // top buttom left right
        float err[] = {abs(r0t - r1b), abs(r0b - r1t), abs(r0l - r1r), abs(r0r - r1l)};
        int resIndex = 0;
        resErr = err[0];
        for (int i = 1; i < 4; i++)
        {
            if (err[i] < resErr)
            {
                resErr = err[i];
                resIndex = i;
            }
        }
        if (resIndex == 2 && !(notOverlapT || notOverlapB))
        {
            return ColLeft;
        }
        else if (resIndex == 3 && !(notOverlapT || notOverlapB))
        {
            return ColRight;
        }
        else if (resIndex == 0 && !(notOverlapL || notOverlapR))
        {
            return ColTop;
        }
        else if (resIndex == 1 && !(notOverlapL || notOverlapR))
        {
            return ColDown;
        }
    }
    else
    {
        return ColNone;
    }
}

void Collision::ResolveCollision(vec2 &pos, CollisionType type, float resErr)
{
    switch (type)
    {
    case ColLeft:
        pos.x += resErr;
        break;
    case ColRight:
        pos.x -= resErr;
        break;
    case ColTop:
        pos.y += resErr;
        break;
    case ColDown:
        pos.y -= resErr;
        break;

    default:
        break;
    }
}

CollisionStatus Collision::CollisonSystem(vec2 &targetPos, vec4 r0, vector<vec4> rects)
{
    CollisionStatus status;
    for (auto r1 : rects)
    {
        float err = 0;
        CollisionType type = GetOverlapType(r0, r1, err);
        if (type != ColNone)
        {
            ResolveCollision(targetPos, type, err);
            if (type == ColDown)
                status.isColDown = true;
            if (type == ColTop)
                status.isColTop = true;
            if (type == ColRight)
                status.isColRight = true;
            if (type == ColLeft)
                status.isColLeft = true;
        }
    }
    return status;
}

bool Collision::IsCollided(vec4 r0, vector<vec4> rects)
{
    bool isCol = false;
    for (auto r1 : rects)
    {
        if (Collision::IsCollided(r0, r1))
        {
            isCol = true;
            break;
        }
    }
    return isCol;
}
// return bool
bool Collision::IsCollided(vec4 r0, vec4 r1)
{
    return (r0.x < r1.x + r1.z &&
            r0.x + r0.z > r1.x &&
            r0.y < r1.y + r1.w &&
            r0.y + r0.w > r1.y);
}