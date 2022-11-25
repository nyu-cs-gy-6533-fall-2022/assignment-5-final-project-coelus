#pragma once
#include "DefferedKey.h"

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
    bool &isGround, &isTop;
    bool &canJumpAttack;
    bool &isDamaged;
    int &ctrlX;
    DefferedKey &dAttack, &dChain, &dJump;
    float &downDistance;
    vector<vec4> &hitboxs;
    vector<float> &hitboxtime;
    
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