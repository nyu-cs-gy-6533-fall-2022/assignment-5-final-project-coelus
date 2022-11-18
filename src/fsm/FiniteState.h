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
          canJumpAttack(data.canJumpAttack),
          ctrlX(data.ctrlX)
    {
    }
    virtual int GetPossibleState()
    {
        return possibleState.input;
    }
    virtual void Enter() { sprite->Reset(); };
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
    bool &canJumpAttack;
    int &ctrlX;

    FSMInput possibleState, interruptState;

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
        possibleState.Add(vector<ActionState>{Run, Fall, Jump, Attack1});
    };

    void Enter()
    {
        FiniteState::Enter();
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
        possibleState.Add(vector<ActionState>{Idle, Fall, Jump, Attack1});
    };

    void Enter()
    {
        FiniteState::Enter();
    };
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
        interruptState.Add(vector<ActionState>{JumpAttack});
        possibleState.Add(vector<ActionState>{Fall});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return interruptState.input;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
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
        interruptState.Add(vector<ActionState>{JumpAttack});
        possibleState.Add(vector<ActionState>{Idle, Run});
    };

    int GetPossibleState()
    {
        if (!isGround)
            return interruptState.input;
        return possibleState.input;
    }

    void Enter()
    {
        FiniteState::Enter();
    };
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
        possibleState.Add(vector<ActionState>{Idle, Run, Fall, Attack2});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        setDirX();
        velocity.x = 50 * dirX * deltaTime;
    };
    void Update()
    {
        if (sprite->IsFrame(3))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit(){};
};

class PlayerAttack2 : public FiniteState
{
public:
    PlayerAttack2(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Run, Fall, Attack3});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        setDirX();
        velocity.x = 50 * dirX * deltaTime;
    };
    void Update()
    {
        if (sprite->IsFrame(2))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit(){};
};

class PlayerAttack3 : public FiniteState
{
public:
    PlayerAttack3(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Run, Fall});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        setDirX();
        velocity.x = 50 * dirX * deltaTime;
    };
    void Update()
    {
        if (sprite->IsFrame(2))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit(){};
};

class PlayerJumpAttack : public FiniteState
{
public:
    PlayerJumpAttack(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Fall});
    };
    int GetPossibleState()
    {
        if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        setDirX();
        canJumpAttack = false;
        velocity.x = 0;
    };
    void Update()
    {
        if (sprite->IsFrame(2))
        {
            soundSys->Play(SFXPlayerAttack);
        }
        velocity.y = 0;
    };
    void Exit(){};
};
