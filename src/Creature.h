#pragma once

#include "Collision.h"
#include "Sprite.h"
#include "Loader.h"
#include "Shader.h"
#include "Transform.h"
#include "Debug.h"
#include "Global.h"
#include "Sound.h"
#include "DefferedKey.h"

#include "fsm/FSM.h"

#include <algorithm>
using namespace glm;

#ifndef _Creature_
#define _Creature_
struct Control
{
    bool right, left, up, down;
    bool jump, attack, drag;
};
class Creature
{
public:
    Creature(SoundSystem *sndSys, Shader *s, double &time)
        : soundSys(sndSys),
          shader(s),
          deltaTime(time),
          position(vec2(0, 0))
    {
        sprite = new AnimSprite();
        debug = new Debug(shader);
        pTx = &sprite->Tx;
        pTx->dirX = -1;
    }
    ~Creature()
    {
        delete sprite;
        delete debug;
        delete fsm;
    }
    virtual void Update(Control ctrl){};

    vec2 GetPos() { return position; };
    void SetPos(vec2 pos) { position = pos; }
    vec2 GetCenterPos() { return position + vec2(rigidbody.x / 2.f, rigidbody.y / 2.f); };
    vec4 GetCol() { return vec4(position, rigidbody.x, rigidbody.y); }
    void SetColStatus(CollisionStatus status)
    {
        isGround = status.isColDown;
        isTop = status.isColTop;
        downDistance = status.distance.y;
    }
    void StateUpdate()
    {
        ActionState nextState = fsmInput.GetNextState(fsm->GetPossibleState());
        if (nextState != EmptyState && nextState != fsm->GetState())
        {
            fsm->Exit();
            sprite->Set(nextState);
            fsm->Set(nextState);
            fsm->Enter();
        }
    }

    void Movement()
    {
        velocity.x += force.x;
        velocity.y += force.y;
        velocity.y = clamp(velocity.y, -Global::MaxSpd, Global::MaxSpd);
        position.x += velocity.x;
        position.y += velocity.y;
    }

    void Draw(double deltaTime)
    {
        debug->SetDebugTx(0, GetCol());
        debug->DrawDebug();

        sprite->Set(position, rigidbody);
        shader->SetMat("modelMatrix", pTx->Get());
        sprite->Draw(deltaTime);
    }

protected:
    Transform *pTx;
    Shader *shader;
    AnimSprite *sprite;
    SoundSystem *soundSys;
    Debug *debug;
    FSM *fsm;
    FSMInput fsmInput;

    float runSpeed, jumpSpeed;
    double &deltaTime;
    vec2 position, velocity, force, rigidbody;
    bool isGround, isTop;
    float downDistance;
};
#endif