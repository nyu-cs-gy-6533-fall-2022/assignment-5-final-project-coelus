#pragma once
#include "Animation.h"
#include "DefferedKey.h"
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
          position(data.position),
          velocity(data.velocity),
          force(data.force),
          deltaTime(data.deltaTime),
          isGround(data.isGround),
          isTop(data.isTop),
          canJumpAttack(data.canJumpAttack),
          ctrlX(data.ctrlX),
          dAttack(data.dAttack),
          dChain(data.dChain),
          dJump(data.dJump),
          downDistance(data.downDistance)

    {
    }
    virtual int GetPossibleState()
    {
        return possibleState.input;
    }
    virtual void Enter()
    {
        dAttack.Reset();
        dChain.Reset();
        dJump.Reset();
        sprite->Reset();
    };
    virtual void Update() = 0;
    virtual void Exit() = 0;

protected:
    SoundSystem *soundSys;
    AnimSprite *sprite;
    float &runSpeed, &jumpSpeed;
    int &dirX;
    vec2 &position, &velocity, &force;
    double &deltaTime;
    bool &isGround, &isTop;
    bool &canJumpAttack;
    int &ctrlX;
    DefferedKey &dAttack, &dChain, &dJump;
    float &downDistance;

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

class FSPlayerIdle : public FiniteState
{
public:
    FSPlayerIdle(FSMData data) : FiniteState(data)
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

class FSPlayerRun : public FiniteState
{
public:
    FSPlayerRun(FSMData data) : FiniteState(data)
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
        bool canDeffered = sprite->IsFrameGreater(1);
        dAttack.Set(canDeffered);

        setDirX();
        moveX();
        falling();
    };
    void Exit(){};
};

class FSPlayerFall : public FiniteState
{
public:
    FSPlayerFall(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{JumpAttack});
        possibleState.Add(vector<ActionState>{Idle, Run, Jump});
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
        if (downDistance <= toleranceH)
            dJump.Set(true);
        else
            dJump.Set(false);

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

private:
    // toleranc distance for jump deffered
    float toleranceH = 150.f;
};

class FSPlayerAttack1 : public FiniteState
{
public:
    FSPlayerAttack1(FSMData data) : FiniteState(data)
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
        setDirX();
        stopX();
    };
    void Update()
    {
        bool canDeffered = sprite->IsFrameGreater(4);
        dAttack.Set(canDeffered);
        dJump.Set(canDeffered);

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

        setDirX();
        stopX();
    };
    void Update()
    {

        bool canDeffered = sprite->IsFrameGreater(3);
        dAttack.Set(canDeffered);
        dJump.Set(canDeffered);

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
        possibleState.Add(vector<ActionState>{Idle, Jump, Run, Fall, Attack1});
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
        bool canDeffered = sprite->IsFrameGreater(4);
        dAttack.Set(canDeffered);
        dJump.Set(canDeffered);

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

class FSSnailIdle : public FiniteState
{
public:
    FSSnailIdle(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{SnailAttack, SnailFall});
    };

    void Enter()
    {
        FiniteState::Enter();
        force = vec2(0);
        velocity = vec2(0);
    };
    void Update(){ };
    void Exit(){};
};

class FSSnailAttack : public FiniteState
{
public:
    FSSnailAttack(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{SnailIdle, SnailFall});
    };

    void Enter()
    {
        FiniteState::Enter();
    };
    void Update()
    {
        setDirX();
        moveX();
    };
    void Exit(){};
};

class FSSnailFall : public FiniteState
{
public:
    FSSnailFall(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{SnailIdle, SnailAttack});
    };

    int GetPossibleState()
    {
        if (!isGround)
            return 0;
        return possibleState.input;
    }

    void Enter()
    {
        FiniteState::Enter();
    };
    void Update()
    {
        setDirX();
        falling();
    };
    void Exit()
    {

        if (isGround)
        {
            stopY();
        }
    };

private:
};