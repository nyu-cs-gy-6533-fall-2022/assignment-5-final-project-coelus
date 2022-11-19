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
          force(data.force),
          deltaTime(data.deltaTime),
          isGround(data.isGround),
          isTop(data.isTop),
          canJumpAttack(data.canJumpAttack),
          ctrlX(data.ctrlX),
          ctrlAttack(data.ctrlAttack),
          isChain(data.isChain)
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
    vec2 &velocity, &force;
    double &deltaTime;
    bool &isGround, &isTop;
    bool &canJumpAttack;
    int &ctrlX;
    bool &ctrlAttack, &isChain;

    FSMInput possibleState, interruptState;

    void setDirX()
    {
        if ((ctrlX == 1 & dirX < 0) ||
            (ctrlX == -1 & dirX > 0))
            dirX *= -1;
    }
    void stopX()
    {
        velocity.x = 0;
        force.x = 0;
    }
    void stopY()
    {
        velocity.y = 0;
        force.y = 0;
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
            force.y = 9.8f * deltaTime * Global::GravityRatio;
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
        force = vec2(0);
        velocity = vec2(0);
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
        if (isGround)
        {
            soundSys->Play(SFXPlayerLanding);
            stopY();
        }
    };
};

class PlayerAttack1 : public FiniteState
{
public:
    PlayerAttack1(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Run, Fall, Jump, Attack2});
        interruptState.Add(vector<ActionState>{Run, Fall, Jump, Attack2});
    };
    int GetPossibleState()
    {
        if (sprite->CanInterrupt())
            return interruptState.input;

        else if (!sprite->IsEnd())
            return 0;

        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        isChain = false;
        setDirX();
        stopX();
    };
    void Update()
    {
        if (ctrlAttack && sprite->IsFrameGreater(6))
        {
            isChain = true;
        }

        force.x = attackForce * dirX * deltaTime;
        if (sprite->IsFrameGreater(4))
        {
            stopX();
        }

        if (sprite->IsFrame(3))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit()
    {
        isChain = false;
        stopX();
    };

private:
    float attackForce = 30;
};

class PlayerAttack2 : public FiniteState
{
public:
    PlayerAttack2(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Run, Fall, Jump, Attack3});
        interruptState.Add(vector<ActionState>{Run, Fall, Jump, Attack3});
    };
    int GetPossibleState()
    {
        if (sprite->CanInterrupt())
            return interruptState.input;
        else if (!sprite->IsEnd())
            return 0;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        isChain = false;
        setDirX();
        stopX();
    };
    void Update()
    {
        if (ctrlAttack && sprite->IsFrameGreater(5))
        {
            isChain = true;
        }
        force.x = attackForce * dirX * deltaTime;
        if (sprite->IsFrameGreater(4))
        {
            stopX();
        }
        if (sprite->IsFrame(2))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit()
    {
        isChain = false;
        stopX();
    };

private:
    float attackForce = 40;
};

class PlayerAttack3 : public FiniteState
{
public:
    PlayerAttack3(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Jump, Run, Fall});
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
        stopX();
    };
    void Update()
    {
        force.x = attackForce * dirX * deltaTime;
        if (sprite->IsFrameGreater(5))
        {
            stopX();
        }
        if (sprite->IsFrame(2))
        {
            soundSys->Play(SFXPlayerAttack);
        }
    };
    void Exit()
    {
        stopX();
    };

private:
    float attackForce = 60;
};

class PlayerJumpAttack : public FiniteState
{
public:
    PlayerJumpAttack(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Fall, Idle, Run});
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
        stopX();
    };
    void Update()
    {
        force.x = attackForce * dirX * deltaTime;
        if (sprite->IsFrameGreater(4))
        {
            stopX();
        }
        if (sprite->IsFrame(2))
        {
            soundSys->Play(SFXPlayerAttack);
        }
        velocity.y = 0;
    };
    void Exit()
    {
        stopX();
    };

private:
    float attackForce = 20;
};
