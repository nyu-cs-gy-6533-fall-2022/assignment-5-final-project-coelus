#pragma once
#include "Animation.h"

#include <glm/glm.hpp>
using namespace glm;

class FSM
{
public:
    FSM(AnimationState st, vec2 v) : state(st), velocity(v){};
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;

private:
    AnimationState &state;
    vec2 &velocity;
};

class PlayerIdle : public FSM
{
public:
    PlayerIdle(AnimationState st, vec2 v) : FSM(st, v){};
    void Enter();
    void Update();
    void Exit();
};