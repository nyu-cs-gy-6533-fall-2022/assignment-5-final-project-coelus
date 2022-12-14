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
#include "Hitbox.h"
#include "CountDown.h"

#include <algorithm>

using namespace glm;

#ifndef _Creature_
#define _Creature_

struct Control
{
    bool right, left, up, down;
    bool jump, attack, chain;
};

class Creature
{
public:
    Creature(SoundSystem *sndSys, const vector<Shader *> &s, double &t)
        : soundSys(sndSys),
          shaders(s),
          deltaTime(t),
          position(vec2(0, 0)),
          velocity(vec2(0, 0)),
          ctrlX(0)
    {
        sprite = new AnimSprite();
        debug = new Debug(shaders);
        pTx = &sprite->Tx;
        pTx->dirX = -1;
    }
    void InitFSM()
    {
        fsm = new FSM(
            FSMData{soundSys, sprite, debug,
                    runSpeed, jumpSpeed,
                    pTx,
                    position, velocity, force,
                    deltaTime,
                    isGround, isTop, isFront, willFall,
                    canJumpAttack,
                    isDamaged,
                    ctrlX,
                    dAttack, dChain, dJump,
                    downDistance,
                    hitboxs,
                    hp, damage,
                    attackEnd, chainEnd,
                    isChainThrow, isChainHit});
    }
    ~Creature()
    {
        delete sprite;
        delete debug;
        delete fsm;
    }
    virtual void Init(){};
    virtual void Update(Control ctrl){};
    void SetDirX(int dir) { pTx->dirX = dir; }
    void SetDamage(Creature *attacker, HitboxData hitbox)
    {
        int dir = attacker->GetPos().x > position.x ? 1 : -1;
        ctrlX = dir;
        damage.dirX = dir;
        damage.force = vec2(-dir * hitbox.force.x, hitbox.force.y);
        damage.losthp = hitbox.damage;
        damage.sound = hitbox.sound;
        if (fsm->GetState() != Damaged && fsm->GetState() != SnailDamaged)
        {
            isDamaged = true;
        }
    }
    float GetHpPercent()
    {
        return hp / (float)initHp;
    }
    bool IsHP0()
    {
        return hp <= 0;
    }

    Transform GetTx() { return *pTx; };
    vec2 GetPos() { return position; };
    void SetPos(vec2 pos) { position = pos; }
    void SetInitPos(vec2 pos)
    {
        initPosition = pos;
        position = pos;
    }
    virtual void Reset() {}
    vec2 GetCenterPos() { return position + vec2(rigidbody.x / 2.f, rigidbody.y / 2.f); };
    vec4 GetCol() { return vec4(position, rigidbody.x, rigidbody.y); }
    vector<HitboxData> GetHitbox() { return hitboxs; }
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

    virtual void Draw()
    {
        shaders[0]->Use();
        debug->SetDebugTx(0, GetCol());
        debug->DrawDebug();

        sprite->Set(position, rigidbody);
        if (fsm->GetState() == MonsterDied)
        {

            shaders[1]->Use();
            shaders[1]->SetMat4("modelMatrix", pTx->Get());
            shaders[1]->SetFloat("dissolveTime", dissolveTime);
            dissolveTime += deltaTime;
            if (dissolveTime > 1)
            {
                dissolveTime = 1;
            }
        }
        else
        {
            shaders[0]->Use();
            shaders[0]->SetMat4("modelMatrix", pTx->Get());
        }

        sprite->Draw(deltaTime);
    }

protected:
    Transform *pTx;
    const vector<Shader *> &shaders;
    AnimSprite *sprite;
    SoundSystem *soundSys;
    Debug *debug;
    FSM *fsm;
    FSMInput fsmInput;
    int initHp, hp;
    float runSpeed, jumpSpeed;
    double &deltaTime;
    vec2 initPosition, position, velocity, force, rigidbody;
    bool isGround, isTop, isFront;
    bool willFall = false;
    bool canJumpAttack;
    DamageData damage;
    bool isDamaged = false;
    float downDistance;
    int ctrlX;
    DefferedKey dAttack, dChain, dJump;
    bool attackEnd = true, chainEnd = false;
    bool *isChainThrow, *isChainHit;

    float dissolveTime = 0;

    vector<HitboxData> hitboxs;

    void setRandCtrlX()
    {
        ctrlX = (rand() % 2) == 0 ? 1 : -1;
    }
    void setInitHp(int val)
    {
        initHp = val;
        hp = initHp;
    }

    void updateHitBox()
    {
        for (int i = hitboxs.size() - 1; i >= 0; i--)
        {
            if (hitboxs[i].time <= 0)
            {
                hitboxs.erase(hitboxs.begin() + i);
            }
            hitboxs[i].time -= deltaTime;
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
        velocity.x += force.x * deltaTime / 2.f;
        velocity.y += force.y * deltaTime / 2.f;
        velocity.y = clamp(velocity.y, -Global::MaxSpd, Global::MaxSpd);
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
    }
};
#endif