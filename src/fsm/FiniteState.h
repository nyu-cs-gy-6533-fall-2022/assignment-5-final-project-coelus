#pragma once
#include "Animation.h"
#include "FSMData.h"
#include <glm/glm.hpp>
using namespace glm;

class FiniteState
{
public:
    FiniteState(FSMData data)
        : soundSys(data.soundSys),
          sprite(data.sprite),
          runSpeed(data.runSpeed),
          jumpSpeed(data.jumpSpeed),
          dirX(data.dirX),
          velocity(data.velocity),
          deltaTime(data.deltaTime),
          isGround(data.isGround),
          isTop(data.isTop),
          ctrlX(data.ctrlX)
    {
    }
    virtual int GetPossibleState()
    {
        return possibleState.input;
    }
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;

protected:
    SoundSystem *soundSys;
    AnimSprite *sprite;
    float &runSpeed, &jumpSpeed;
    int &dirX;
    vec2 &velocity;
    double &deltaTime;
    bool &isGround, &isTop;
    int &ctrlX;

    FSMInput possibleState;

    void setDirX()
    {
        if ((ctrlX == 1 & dirX < 0) ||
            (ctrlX == -1 & dirX > 0))
            dirX *= -1;
    }

    void moveX()
    {
        if (ctrlX != 0)
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
    PlayerIdle(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<AnimationState>{Run, Fall, Jump, Attack1});
    };

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
    PlayerRun(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<AnimationState>{Idle, Fall, Jump, Attack1});
    };

    void Enter(){};
    void Update()
    {
        setDirX();
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
    PlayerJump(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<AnimationState>{Fall});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
    }
    void Enter()
    {
        velocity.y = -jumpSpeed * deltaTime;
        soundSys->Play(SFXPlayerJump);
    };
    void Update()
    {
        setDirX();
        moveX();
        falling();
    };
    void Exit(){};
};

class PlayerFall : public FiniteState
{
public:
    PlayerFall(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<AnimationState>{Idle, Run});
    };

    int GetPossibleState()
    {
        if (!isGround)
            return 0;
        return possibleState.input;
    }

    void Enter(){};
    void Update()
    {
        setDirX();
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
    PlayerAttack1(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<AnimationState>{Idle, Run});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
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