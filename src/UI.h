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
        uiframe = new Sprite();
        uiframe->Set("UI/blood1_b.png", vec2(0), vec2(0));

        blood= new Sprite();
        blood->Set("UI/blood2.png", vec2(0), vec2(0));
    };
    ~UI()
    {
        delete uiframe;
    };
    void Draw()
    {

        shaders[4]->Use();
        blood->Draw();
        uiframe->Draw();
        
    }

private:
    vector<Shader *> &shaders;
    Sprite *uiframe,*blood;
};