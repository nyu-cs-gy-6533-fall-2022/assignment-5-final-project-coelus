#pragma once
#include "Animation.h"
#include "DefferedKey.h"
#include "FSMData.h"
#include "CountDown.h"

#include <glm/glm.hpp>
using namespace glm;

class FiniteState
{
public:
    FiniteState(FSMData data)
        : soundSys(data.soundSys),
          sprite(data.sprite),
          debug(data.debug),
          runSpeed(data.runSpeed),
          jumpSpeed(data.jumpSpeed),
          pTx(data.pTx),
          dirX(pTx->dirX),
          position(data.position),
          velocity(data.velocity),
          force(data.force),
          deltaTime(data.deltaTime),
          isGround(data.isGround),
          isTop(data.isTop),
          isFront(data.isFront),
          willFall(data.willFall),
          canJumpAttack(data.canJumpAttack),
          isDamaged(data.isDamaged),
          ctrlX(data.ctrlX),
          dAttack(data.dAttack),
          dChain(data.dChain),
          dJump(data.dJump),
          downDistance(data.downDistance),
          hitboxs(data.hitboxs),
          hp(data.hp),
          damage(data.damage),
          attackEnd(data.attackEnd)

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
    Debug *debug;
    float &runSpeed, &jumpSpeed;
    Transform *pTx;
    vec2 &position, &velocity, &force;
    double &deltaTime;
    bool &isGround, &isTop, &isFront, &willFall;
    bool &canJumpAttack, &isDamaged;
    int &ctrlX;
    DefferedKey &dAttack, &dChain, &dJump;
    float &downDistance;
    vector<HitboxData> &hitboxs;
    int &dirX;
    int &hp;
    DamageData &damage;
    bool &attackEnd;

    FSMInput possibleState, interruptState;

    void setDirX()
    {
        if ((ctrlX == 1 & dirX < 0) ||
            (ctrlX == -1 & dirX > 0))
            dirX *= -1;
    }
    void setRandCtrlX()
    {
        ctrlX = (rand() % 2) == 0 ? 1 : -1;
    }
    // prevent fall down the edge
    void updateCtrlX()
    {
        if (isFront || willFall)
        {
            ctrlX *= -1;
        }
    }
    void deceleration(float amount = 10.f)
    {
        int sign = velocity.x >= 0 ? 1 : -1;
        velocity.x -= sign * amount * deltaTime;
        if (velocity.x * sign < 0)
        {
            velocity.x = 0;
        }
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
    void addHitBox(SoundType sound, vec4 hitbox, vec2 force, int damage, float time = 0.18f)
    {
        debug->AddDebug(hitbox);
        hitboxs.push_back(HitboxData{hitbox, force, damage, time, sound});
    }
};

class FSPlayerIdle : public FiniteState
{
public:
    FSPlayerIdle(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Run, Fall, Jump, Attack1, Damaged, ChainU});
    };

    void Enter()
    {
        FiniteState::Enter();
        force = vec2(0);
        velocity = vec2(0);
    };
    void Update(){};
    void Exit()
    {
        force = vec2(0);
        velocity = vec2(0);
    };
};

class FSPlayerRun : public FiniteState
{
public:
    FSPlayerRun(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Fall, Jump, Attack1, Damaged, ChainU});
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

class FSPlayerJump : public FiniteState
{
public:
    FSPlayerJump(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{JumpAttack, Damaged});
        possibleState.Add(vector<ActionState>{Fall, Damaged});
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
        interruptState.Add(vector<ActionState>{JumpAttack, Damaged});
        possibleState.Add(vector<ActionState>{Idle, Run, Jump, Damaged});
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
        possibleState.Add(vector<ActionState>{Idle, Run, Fall, Jump, Attack2, Damaged});
        interruptState.Add(vector<ActionState>{Run, Fall, Jump, Attack2, Damaged});
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(position, 100, 330), position.y - 60, 330, 280), vec2(130, -70), 20);
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

class FSPlayerAttack2 : public FiniteState
{
public:
    FSPlayerAttack2(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Run, Fall, Jump, Attack3, Damaged});
        interruptState.Add(vector<ActionState>{Run, Fall, Jump, Attack3, Damaged});
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(position, 230, 360), position.y - 40, 350, 280), vec2(150, -70), 30);
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

class FSPlayerAttack3 : public FiniteState
{
public:
    FSPlayerAttack3(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{Damaged});
        possibleState.Add(vector<ActionState>{Idle, Jump, Run, Fall, Attack1});
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(position, 350, 600), position.y - 40, 600, 240), vec2(200, -90), 50);
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

class FSPlayerJumpAttack : public FiniteState
{
public:
    FSPlayerJumpAttack(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{Damaged});
        possibleState.Add(vector<ActionState>{Fall, Idle, Run});
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(position, 100, 330), position.y - 60, 330, 280), vec2(130, -70), 20);
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

class FSPlayerChainU : public FiniteState
{
public:
    FSPlayerChainU(FSMData data) : FiniteState(data)
    {
        throwChainCD.Init(0.5f, deltaTime, 0);
        interruptState.Add(vector<ActionState>{Damaged});
        possibleState.Add(vector<ActionState>{Idle, Run, Jump, Damaged});
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
        stopX();
        throwChainCD.Reset();
    };
    void Update()
    {
        if (sprite->IsFrame(3))
        {
            soundSys->Play(SFXChainThrow);
            sprite->AddFrame();
            sprite->PauseFrame();
        }
        else if (throwChainCD.IsRun())
        {
            throwChainCD.Update();
        }
        else
        {
            sprite->ResumeFrame();
        }
    };
    void Exit()
    {
        sprite->ResumeFrame();
    };

private:
    CountDown throwChainCD;
};

class FSPlayerDamaged : public FiniteState
{
public:
    FSPlayerDamaged(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{Idle, Fall});
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
        isDamaged = false;
        velocity = vec2(0);
        hp -= damage.losthp;
        dirX = damage.dirX;
        soundSys->Play(damage.sound);
    };
    void Update()
    {

        if (isGround)
        {
            stopY();
        }
        else
        {
            falling();
        }
        if (!sprite->IsFrameGreater(1))
        {
            force.x = damage.force.x * deltaTime;
            force.y = damage.force.y * deltaTime;
        }
        else
        {
            force.x = 0;
            deceleration();
        }
    };
    void Exit()
    {

        isDamaged = false;
    };
};

class FSSnailIdle : public FiniteState
{
public:
    FSSnailIdle(FSMData data) : FiniteState(data)
    {
        loopCD.Init(1.2f, deltaTime, 2);
        interruptState.Add(vector<ActionState>{SnailDamaged, SnailFall, Died});
        possibleState.Add(vector<ActionState>{SnailAttack, SnailFall, SnailDamaged, Died});
    };
    int GetPossibleState()
    {
        if (loopCD.IsRun())
            return interruptState.input;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        force = vec2(0);
        velocity = vec2(0);
        loopCD.Reset();
    };
    void Update()
    {
        loopCD.Update();
    };
    void Exit()
    {
        if (loopCD.IsEnd())
        {
            setRandCtrlX();
        }

        force = vec2(0);
        velocity = vec2(0);
    };

private:
    CountDown loopCD;
};

class FSSnailAttack : public FiniteState
{
public:
    FSSnailAttack(FSMData data) : FiniteState(data)
    {
        hitboxCD.Init(0.01f, deltaTime, 0);
        loopCD.Init(2.f, deltaTime, 2);
        interruptState.Add(vector<ActionState>{SnailDamaged, SnailFall});
        possibleState.Add(vector<ActionState>{SnailIdle, SnailFall, SnailDamaged, Died});
    };
    int GetPossibleState()
    {
        if (loopCD.IsRun())
            return interruptState.input;
        return possibleState.input;
    }

    void Enter()
    {
        FiniteState::Enter();
        addHitBox(SFXSnailHit, vec4(pTx->GetX(position, 0, 200), position.y, 200, 152), vec2(100, -70), 20, 0.001);
        hitboxCD.Reset();
        loopCD.Reset();
    };
    void Update()
    {
        updateCtrlX();
        setDirX();
        moveX();
        if (hitboxCD.IsEnd())
        {
            hitboxCD.Reset();
            addHitBox(SFXSnailHit, vec4(pTx->GetX(position, 0, 200), position.y, 200, 152), vec2(100, -70), 20, 0.001);
        }
        hitboxCD.Update();
        loopCD.Update();
    };
    void Exit()
    {
        hitboxs.clear();
    };

private:
    CountDown loopCD;
    CountDown hitboxCD;
};

class FSSnailFall : public FiniteState
{
public:
    FSSnailFall(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{SnailDamaged});
        possibleState.Add(vector<ActionState>{SnailIdle, SnailDamaged, Died});
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
class FSSnailDamaged : public FiniteState
{
public:
    FSSnailDamaged(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{SnailIdle, SnailFall});
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
        velocity = vec2(0);
        hp -= damage.losthp;
        dirX = damage.dirX;
        soundSys->Play(damage.sound);
    };
    void Update()
    {

        if (isGround)
        {
            stopY();
        }
        else
        {
            falling();
        }
        if (!sprite->IsFrameGreater(1))
        {
            force.x = damage.force.x * deltaTime;
            force.y = damage.force.y * deltaTime;
        }
        else
        {
            force.x = 0;
            deceleration();
        }
    };
    void Exit()
    {
        isDamaged = false;
    };
};

class FSRatIdle : public FiniteState
{
public:
    FSRatIdle(FSMData data) : FiniteState(data)
    {
        loopCD.Init(1.2f, deltaTime, 1);
        interruptState.Add(vector<ActionState>{RatDamaged, RatFall, Died});
        possibleState.Add(vector<ActionState>{RatRun, RatFall, RatDamaged, RatRollStart, Died});
    };
    int GetPossibleState()
    {
        if (loopCD.IsRun())
            return interruptState.input;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        force = vec2(0);
        velocity = vec2(0);
        loopCD.Reset();
    };
    void Update()
    {
        loopCD.Update();
    };
    void Exit()
    {
        if (loopCD.IsEnd())
        {
            setRandCtrlX();
        }
        force = vec2(0);
        velocity = vec2(0);
    };

private:
    CountDown loopCD;
};

class FSRatRun : public FiniteState
{
public:
    FSRatRun(FSMData data) : FiniteState(data)
    {
        loopCD.Init(1.5f, deltaTime, 2);
        interruptState.Add(vector<ActionState>{RatDamaged, RatFall});
        possibleState.Add(vector<ActionState>{RatIdle, RatFall, RatDamaged, Died, RatRollStart});
    };
    int GetPossibleState()
    {
        if (loopCD.IsRun())
            return interruptState.input;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        loopCD.Reset();
    };
    void Update()
    {

        updateCtrlX();
        setDirX();
        moveX();
        loopCD.Update();
    };
    void Exit()
    {
        stopX();
    };

private:
    CountDown loopCD;
};

class FSRatFall : public FiniteState
{
public:
    FSRatFall(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{RatDamaged});
        possibleState.Add(vector<ActionState>{RatIdle, RatDamaged, Died});
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
class FSRatDamaged : public FiniteState
{
public:
    FSRatDamaged(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{RatIdle, RatFall});
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
        velocity = vec2(0);
        hp -= damage.losthp;
        dirX = damage.dirX;
        soundSys->Play(damage.sound);
    };
    void Update()
    {

        if (isGround)
        {
            stopY();
        }
        else
        {
            falling();
        }
        if (!sprite->IsFrameGreater(1))
        {
            force.x = damage.force.x * deltaTime;
            force.y = damage.force.y * deltaTime;
        }
        else
        {
            force.x = 0;
            deceleration();
        }
    };
    void Exit()
    {
        isDamaged = false;
    };
};

class FSRatRollStart : public FiniteState
{
public:
    FSRatRollStart(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{RatFall, RatDamaged});
        possibleState.Add(vector<ActionState>{RatRollLoop, RatFall, RatDamaged});
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
    };
    void Update()
    {
        setDirX();
    };
    void Exit(){};

private:
};

class FSRatRollLoop : public FiniteState
{
public:
    FSRatRollLoop(FSMData data) : FiniteState(data)
    {
        hitboxCD.Init(0.01f, deltaTime, 0);
        loopCD.Init(2.f, deltaTime, 2);
        interruptState.Add(vector<ActionState>{RatFall, RatDamaged});
        possibleState.Add(vector<ActionState>{RatRollStop, RatFall, RatDamaged});
    };

    int GetPossibleState()
    {
        if (loopCD.IsRun())
            return interruptState.input;
        return possibleState.input;
    }
    void Enter()
    {
        FiniteState::Enter();
        addHitBox(SFXRatHit, vec4(pTx->GetX(position, 0, 200), position.y - 50, 200, 200), vec2(150, -150), 50, 0.001);
        hitboxCD.Reset();
        loopCD.Reset();
    };
    void Update()
    {
        updateCtrlX();
        setDirX();
        if (loopCD.IsLessHalf())
        {
            force.x = dirX * 10 * deltaTime;
        }
        else
        {
            force.x = 0;
            deceleration(5);
        }
        if (velocity.x < 0 && dirX == 1 || velocity.x > 0 && dirX == -1)
        {
            force.x *= -1;
            velocity.x *= -1;
        }
        if (hitboxCD.IsEnd())
        {
            hitboxCD.Reset();
            addHitBox(SFXRatHit, vec4(pTx->GetX(position, 0, 200), position.y - 50, 200, 200), vec2(150, -150), 50, 0.001);
        }
        hitboxCD.Update();
        loopCD.Update();
    };
    void Exit()
    {
        stopX();
        hitboxs.clear();
    };

private:
    CountDown loopCD;
    CountDown hitboxCD;

private:
};
class FSRatRollStop : public FiniteState
{
public:
    FSRatRollStop(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{RatFall, RatDamaged});
        possibleState.Add(vector<ActionState>{RatIdle, RatRun, RatFall, RatDamaged});
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
    };
    void Update()
    {
        setDirX();
    };
    void Exit()
    {
        attackEnd = true;
    };

private:
};

class FSDied : public FiniteState
{
public:
    FSDied(FSMData data) : FiniteState(data){};

    int GetPossibleState()
    {
        return 0;
    }

    void Enter()
    {
        FiniteState::Enter();
        velocity = vec2(0);
        force = vec2(0);
        soundSys->Play(SFXDisappear);
    };
    void Update(){};
    void Exit(){};

private:
};
