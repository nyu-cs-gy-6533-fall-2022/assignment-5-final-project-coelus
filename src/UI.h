#pragma once
#include "Sprite.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <math.h>

using namespace glm;

class UI
{
public:
    UI(vector<Shader *> &s)
        : shaders(s)
    {
        sprite = new Sprite();
        sprite->Set("UI/blood1_b.png", vec2(0), vec2(0));
    };
    ~UI()
    {
        delete sprite;
    };
    void Draw()
    {

        shaders[4]->Use();
        sprite->Draw();
    }

private:
    vector<Shader *> &shaders;
    Sprite *sprite;
};