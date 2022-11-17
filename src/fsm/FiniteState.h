#pragma once
#include "Animation.h"
#include "FSMInput.h"
#include <glm/glm.hpp>
using namespace glm;

class FiniteState
{
public:
    FiniteState(FSMInput input)
        : soundSys(input.soundSys),
          sprite(input.sprite),
          runSpeed(input.runSpeed),
          jumpSpeed(input.jumpSpeed),
          dirX(input.dirX),
          velocity(input.velocity),
          state(input.state),
          deltaTime(input.deltaTime),
          isGround(input.isGround),
          isTop(input.isTop),
          ctrlX(input.ctrlX)
    {
    }
    virtual bool TryNextState(AnimationState state) = 0;
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;

protected:
    SoundSystem *soundSys;
    AnimSprite *sprite;
    float &runSpeed, &jumpSpeed;
    int &dirX;
    vec2 &velocity;
    AnimationState &state;
    double &deltaTime;
    bool &isGround, &isTop;
    bool &ctrlX;

    void moveX()
    {
        if (ctrlX)
        {
            velocity.x = runSpeed * dirX * deltaTime;
        }
    }
    void falling()
    {
        if (!isGround)
        {
            if (isTop)
            {
                velocity.y = 0;
            }
            velocity.y += 9.8f * deltaTime * Global::GravityRatio;
        }
    }
};

class PlayerIdle : public FiniteState
{
public:
    PlayerIdle(FSMInput input) : FiniteState(input){};
    bool TryNextState(AnimationState state) { return state != Idle; }
    void Enter()
    {
        velocity.x = 0;
        velocity.y = 0;
    };
    void Update(){};
    void Exit(){};
};

class PlayerRun : public FiniteState
{
public:
    PlayerRun(FSMInput input) : FiniteState(input){};
    bool TryNextState(AnimationState state) { return state != Run; }
    void Enter(){};
    void Update()
    {
        moveX();
        if (sprite->IsFrame(3) || sprite->IsFrame(8))
        {
            soundSys->Play(SFXPlayerStep);
        }
    };
    void Exit(){};
};

class PlayerJump : public FiniteState
{
public:
    PlayerJump(FSMInput input) : FiniteState(input){};
    bool TryNextState(AnimationState state)
    {
        return state == Fall;
    }
    void Enter()
    {
        velocity.y = -jumpSpeed * deltaTime;
        soundSys->Play(SFXPlayerJump);
    };
    void Update()
    {
        moveX();
        falling();
    };
    void Exit(){};
};

class PlayerFall : public FiniteState
{
public:
    PlayerFall(FSMInput input) : FiniteState(input){};
    bool TryNextState(AnimationState state)
    {
        return state == Run || state == Idle;
    }
    void Enter(){};
    void Update()
    {
        moveX();
        falling();
    };
    void Exit()
    {
        soundSys->Play(SFXPlayerLanding);
        velocity.y = 0;
    };
};

class PlayerAttack1 : public FiniteState
{
public:
    PlayerAttack1(FSMInput input) : FiniteState(input){};
    bool TryNextState(AnimationState state)
    {
        return sprite->IsFrame(8);
    }
    void Enter()
    {
        velocity.x = 50 * dirX * deltaTime;
    };
    void Update()
    {
        if (sprite->IsFrame(1))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit(){};
};