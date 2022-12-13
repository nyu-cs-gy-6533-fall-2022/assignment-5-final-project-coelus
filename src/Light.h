

#pragma once
#include "Sprite.h"
#include "Shader.h"
#include "LightData.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;


class Light
{
public:
    Light(const vector<Shader *> &s, string type, vec2 pos)
        : shaders(s), position(pos)
    {
        sprite = new Sprite();
        if (type == "lightDown")
        {
            lightDir = vec2(0, -1);
            sprite->Set("light/light3-down.png", vec2(128, 128), pos);
            sprite->Tx.pivot.x = 64;
            lightType = LightDirect;
        }
        else if (type == "lightRight")
        {
            lightDir = vec2(-1, 0);
            sprite->Set("light/light3-right.png", vec2(128, 128), pos);
            sprite->Tx.pivot.y = 64;
            lightType = LightDirect;
        }
        else if (type == "lightLeft")
        {
            lightDir = vec2(1, 0);
            sprite->Set("light/light3-left.png", vec2(128, 128), pos);
            sprite->Tx.pivot.x = 128;
            sprite->Tx.pivot.y = 64;
            lightType = LightDirect;
        }
        else if (type == "lightCenter")
        {
            lightDir = vec2(0, 0);
            sprite->Set("light/lightcenter.png", vec2(128, 128), pos);
            sprite->Tx.pivot.x = 64;
            sprite->Tx.pivot.y = 64;
            lightType = LightPoint;
        }
    }
    ~Light()
    {
        delete sprite;
    };
    LightData GetLightData()
    {
        return LightData{lightType, position, lightDir};
    }
    void Draw()
    {
        shaders[0]->Use();
        shaders[0]->SetMat4("modelMatrix", sprite->Tx.Get());
        sprite->Draw();
    }

private:
    LightType lightType;
    const vector<Shader *> &shaders;
    Sprite *sprite;
    vec2 position, lightDir;
};