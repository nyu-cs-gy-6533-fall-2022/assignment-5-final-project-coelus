#pragma once
#include <iostream>
#include <math.h>
using namespace std;

struct FSMData
{
    SoundSystem *soundSys;
    AnimSprite *sprite;
    float &runSpeed, &jumpSpeed;
    int &dirX;
    vec2 &velocity;
    double &deltaTime;
    bool &isGround, &isTop;
    int &ctrlX;
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
        int b = 0;
        while (val >>= 1)
            b++;
        return b;
    }
    ActionState GetNextState(int possibleState)
    {
        int res = input & possibleState;
        return (ActionState)GetMSB(res);
    }
};