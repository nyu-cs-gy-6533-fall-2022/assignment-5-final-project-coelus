#pragma once
#include "Sprite.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include "creature/Player.h"
#include <vector>
#include <math.h>

using namespace glm;

class UI
{
public:
    UI(const vector<Shader *> &s, Player *pl)
        : shaders(s), player(pl)
    {
        uiframe = new Sprite();
        uiframe->Set("UI/blood1_b.png", vec2(0), vec2(0));

        blood = new Sprite();
        blood->Set("UI/blood2.png", vec2(0), vec2(0));
    };
    ~UI()
    {
        delete uiframe;
        delete blood;
    };
    void Draw()
    {
        float ratio = 1 / player->GetHpPercent();
        shaders[4]->Use();
        shaders[4]->SetFloat("scale", ratio);
        shaders[4]->SetFloat("err", -397 / 1920.f * (ratio - 1));
        blood->Draw();

        shaders[4]->SetFloat("scale", 1);
        shaders[4]->SetFloat("err", 0);
        uiframe->Draw();

        
    }

private:
    Player *player;
    const vector<Shader *> &shaders;
    Sprite *uiframe, *blood;
};