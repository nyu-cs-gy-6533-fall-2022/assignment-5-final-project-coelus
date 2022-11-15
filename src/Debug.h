#pragma once
#include "Sprite.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
using namespace glm;

#define DEBUG true

class Debug
{
public:
    Debug(Shader *s) : shader(s){};
    ~Debug()
    {
        for (auto rect : debugRect)
        {
            delete rect;
        }
    };
    void AddDebug(vec4 rect);
    void DrawDebug();
    void SetDebugTx(int index, vec4 rect);

private:
    Shader *shader;
    vector<Sprite *> debugRect;
};