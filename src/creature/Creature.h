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
          position(vec2(0, 0)),
          velocity(vec2(0, 0)),
          ctrlX(0)
    {
        sprite = new AnimSprite();
        debug = new Debug(shader);
        pTx = &sprite->Tx;
        pTx->dirX = -1;
        fsm = new FSM(
            FSMData{soundSys, sprite, debug,
                    runSpeed, jumpSpeed,
                    pTx,
                    position, velocity, force,
                    deltaTime,
                    isGround, isTop,
                    canJumpAttack,
                    isDamaged,
                    ctrlX,
                    dAttack, dChain, dJump,
                    downDistance,
                    hitboxs, hitboxtime});
    }
    ~Creature()
    {
        delete sprite;
        delete debug;
        delete fsm;
    }
    virtual void Update(Control ctrl){};
    void SetDamage() { isDamaged = true; }

    Transform GetTx() { return *pTx; };
    vec2 GetPos() { return position; };
    void SetPos(vec2 pos) { position = pos; }
    vec2 GetCenterPos() { return position + vec2(rigidbody.x / 2.f, rigidbody.y / 2.f); };
    vec4 GetCol() { return vec4(position, rigidbody.x, rigidbody.y); }
    vector<vec4> GetHitbox() { return hitboxs; }
    void SetColStatus(CollisionStatus status)
    {
        isGround = status.isColDown;
        isTop = status.isColTop;
        isFront = status.isColLeft & pTx->dirX == -1 | status.isColRight & pTx->dirX == 1;
        downDistance = status.distance.y;
    }
    void SetPreditColStatus(CollisionStatus status)
    {
        willFall = !status.isColDown;
    }

    void Draw()
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
    bool isGround, isTop, isFront;
    bool willFall = false;
    bool canJumpAttack;
    bool isDamaged = false;
    float downDistance;
    int ctrlX;
    DefferedKey dAttack, dChain, dJump;

    vector<vec4> hitboxs;
    vector<float> hitboxtime;

    void updateHitBox()
    {
        for (int i = hitboxtime.size() - 1; i >= 0; i--)
        {
            if (hitboxtime[i] <= 0)
            {
                hitboxs.erase(hitboxs.begin() + i);
                hitboxtime.erase(hitboxtime.begin() + i);
            }
            hitboxtime[i] -= deltaTime;
        }
    }
    void stateUpdate()
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

    void movement()
    {
        velocity.x += force.x;
        velocity.y += force.y;
        velocity.y = clamp(velocity.y, -Global::MaxSpd, Global::MaxSpd);
        position.x += velocity.x;
        position.y += velocity.y;
    }
};
#endif