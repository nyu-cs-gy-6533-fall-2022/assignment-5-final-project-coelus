#pragma once
#include "Sprite.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
using namespace glm;

#define DEBUG false

class Debug
{
public:
    Debug(const vector<Shader*> &s) : shaders(s){};
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
    const vector<Shader*> &shaders;
    vector<Sprite *> debugRect;
};