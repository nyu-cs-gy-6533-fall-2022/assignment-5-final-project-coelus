#pragma once
#include "Animation.h"
#include "FSMInput.h"
#include <glm/glm.hpp>
using namespace glm;

class FiniteState
{
public:
    FiniteState(FSMInput i) : input(i){};
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;

protected:
    FSMInput input;
};

class PlayerIdle : public FiniteState
{
public:
    PlayerIdle(FSMInput i) : FiniteState(i){};
    void Enter(){};
    void Update(){};
    void Exit(){};
};

class PlayerRun : public FiniteState
{
public:
    PlayerRun(FSMInput i) : FiniteState(i){};
    void Enter(){};
    void Update(){};
    void Exit(){};
};

class PlayerJump : public FiniteState
{
public:
    PlayerJump(FSMInput i) : FiniteState(i){};
    void Enter(){};
    void Update(){};
    void Exit(){};
};

class PlayerFall : public FiniteState
{
public:
    PlayerFall(FSMInput i) : FiniteState(i){};
    void Enter(){};
    void Update()
    {
        if (!input.isGround)
        {
            if (input.isTop)
            {
                input.velocity.y = 0;
            }
            input.velocity.y += 9.8f * input.deltaTime * Global::GravityRatio;

            input.velocity.y = clamp(input.velocity.y, -Global::MaxSpd, Global::MaxSpd);
        }
        else if (input.velocity.y > 0 && input.isGround)
        {
            input.velocity.y = 0;
        }
    };
    void Exit(){};
};

class PlayerAttack1 : public FiniteState
{
public:
    PlayerAttack1(FSMInput i) : FiniteState(i){};
    void Enter(){};
    void Update(){};
    void Exit(){};
};