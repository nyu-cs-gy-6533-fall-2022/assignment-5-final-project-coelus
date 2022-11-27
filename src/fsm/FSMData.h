#pragma once
#include "DefferedKey.h"
#include "Hitbox.h"

using namespace std;

struct FSMData
{
    SoundSystem *soundSys;
    AnimSprite *sprite;
    Debug *debug;
    float &runSpeed, &jumpSpeed;
    Transform *pTx;
    vec2 &position, &velocity, &force;
    double &deltaTime;
    bool &isGround, &isTop, &isFront, &willFall;
    bool &canJumpAttack;
    bool &isDamaged;
    int &ctrlX;
    DefferedKey &dAttack, &dChain, &dJump;
    float &downDistance;
    vector<HitboxData> &hitboxs;
    int &hp;
    DamageData &damage;
    bool &attackEnd;
    bool *isChainThrow, *isChainHit;
};

struct FSMInput
{
    int input = 0;
    void Init()
    {
        input = 0;
    }
    void Add(ActionState state)
    {
        input |= (1 << state);
    }
    void Add(vector<ActionState> states)
    {
        for (auto s : states)
        {
            Add(s);
        }
    }
    int GetMSB(int val)
    {
        int bit = 0;
        while (val >>= 1)
            bit++;
        return bit;
    }
    ActionState GetNextState(int possibleState)
    {
        return (ActionState)GetMSB(input & possibleState);
    }
};