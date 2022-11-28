#pragma once

#include "Sprite.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
using namespace glm;

class Tube
{
public:
    Tube(vector<Shader *> &s, string type, vec2 p)
        : shaders(s), pos(p)
    {
        sprite = new Sprite();
        if (type == "tube4")
        {
            sprite->Set("tube/tube-4.png", vec2(512, 128), pos);
        }
        else if (type == "tube6")
        {
            sprite->Set("tube/tube-6.png", vec2(768, 128), pos);
        }
        else if (type == "tube8")
        {
            sprite->Set("tube/tube-8.png", vec2(1024, 128), pos);
        }
    };
    ~Tube()
    {
        delete sprite;
    };
    void Draw()
    {

        shaders[0]->Use();
        shaders[0]->SetMat4("modelMatrix", sprite->Tx.Get());
        sprite->Draw();
    }
    vec4 GetCol()
    {
        return vec4(pos.x, pos.y + 40, sprite->Tx.scale.x, 50);
    }

private:
    vector<Shader *> &shaders;
    Sprite *sprite;
    const vec2 pos;
};