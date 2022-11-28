#pragma once

#include "Sprite.h"
#include "Shader.h"
#include "Sound.h"
#include "Line.h"
#include <glm/glm.hpp>
#include <vector>
#include <math.h>

using namespace glm;

class Chain
{
public:
    Chain(SoundSystem *sndSys, vector<Shader *> &s, double &time, Transform *tx, vec2 &plPos)
        : soundSys(sndSys), shaders(s), deltaTime(time), plTx(tx), playerPos(plPos)
    {
        sprite = new Sprite();
        sprite->Set("main/chain.png", vec2(22, 75), plPos);
        pos = &sprite->Tx.position;
        line = new Line(shaders);
    };
    ~Chain()
    {
        delete line;
        delete sprite;
    };
    void Update()
    {
        if (IsHit)
        {
            SetInitPos(0);
            return;
        }

        oscillate();
        SetPos(0);
    }
    void SetHit(MaterialType mat)
    {
        if (IsThrow && !IsHit)
        {
            pos->y += -30;
            IsHit = true;
            switch (mat)
            {
            case MatWall:
                soundSys->Play(SFXChainHitWall);
                break;
            case MatTube:
                soundSys->Play(SFXChainHitTube);
                break;
            default:
                break;
            }
        }
    }
    void SetInitPos(int index)
    {
        initPos.x = plTx->GetX(playerPos, err[index].x, sprite->Tx.scale.x);
        initPos.y = playerPos.y + err[index].y;
    }
    void SetPos(int index)
    {
        SetInitPos(index);
        pos->x = initPos.x;
        pos->y = initPos.y + sin(moveTime) * -300;
    }
    void Draw()
    {
        if (IsThrow)
        {
            shaders[0]->Use();
            shaders[0]->SetMat4("modelMatrix", sprite->Tx.Get());
            sprite->Draw();

            vec2 startPos = initPos + vec2(11, 75);
            vec2 endPos = *pos + vec2(11, 75);
            line->Draw(vector<vec2>{startPos, endPos});
        }
    }

    vec4 GetCol() { return vec4(*pos, sprite->Tx.scale); }
    bool IsThrow = false;
    bool IsHit = false;

private:
    const double pi = atan(1) * 4;
    const vector<vec2> err{vec2(11, -120)};
    vec2 &playerPos, initPos, *pos;
    Transform *plTx;
    vector<Shader *> &shaders;
    SoundSystem *soundSys;
    Sprite *sprite;
    Line *line;
    double &deltaTime, moveTime = 0;

    void oscillate()
    {
        if (IsThrow)
        {
            moveTime += deltaTime * pi * 2.05f;
            if (moveTime >= pi)
            {
                moveTime = 0;
                IsThrow = false;
            }
        }
        else
        {
            moveTime = 0;
        }
    }
};