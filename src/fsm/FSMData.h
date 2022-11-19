#pragma once
#include "DefferedKey.h"

using namespace std;

struct FSMData
{
    SoundSystem *soundSys;
    AnimSprite *sprite;
    float &runSpeed, &jumpSpeed;
    int &dirX;
    vec2 &velocity, &force;
    double &deltaTime;
    bool &isGround, &isTop;
    bool &canJumpAttack;
    int &ctrlX;
    DefferedKey &dAttack, &dChain, &dJump;
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