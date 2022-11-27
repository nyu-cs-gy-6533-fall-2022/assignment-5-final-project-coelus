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
          shouldIdle(data.shouldIdle)

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
    bool &isGround, &isTop;
    bool &canJumpAttack, &isDamaged;
    int &ctrlX;
    DefferedKey &dAttack, &dChain, &dJump;
    float &downDistance;
    vector<HitboxData> &hitboxs;
    int &dirX;
    int &hp;
    DamageData &damage;
    bool &shouldIdle;

    FSMInput possibleState, interruptState;

    void setDirX()
    {
        if ((ctrlX == 1 & dirX < 0) ||
            (ctrlX == -1 & dirX > 0))
            dirX *= -1;
    }

    void deceleration()
    {
        int sign = velocity.x >= 0 ? 1 : -1;
        velocity.x -= sign * 10 * deltaTime;
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
        possibleState.Add(vector<ActionState>{Run, Fall, Jump, Attack1, Damaged});
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
        possibleState.Add(vector<ActionState>{Idle, Fall, Jump, Attack1, Damaged});
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(120, 300), position.y - 40, 300, 240), vec2(130, -70), 20);
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(200, 300), position.y - 40, 350, 240), vec2(150, -70), 30);
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(350, 600), position.y - 40, 600, 240), vec2(200, -90), 50);
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
            addHitBox(SFXPlayerHit, vec4(pTx->GetX(120, 300), position.y, 300, 300), vec2(130, -70), 20);
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
        possibleState.Add(vector<ActionState>{SnailAttack, SnailFall, SnailDamaged, Died});
    };

    void Enter()
    {
        FiniteState::Enter();
        force = vec2(0);
        velocity = vec2(0);
    };
    void Update(){

    };
    void Exit()
    {
        force = vec2(0);
        velocity = vec2(0);
    };
};

class FSSnailAttack : public FiniteState
{
public:
    FSSnailAttack(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{SnailIdle, SnailFall, SnailDamaged, Died});
    };

    void Enter()
    {
        FiniteState::Enter();
        addHitBox(SFXSnailHit, vec4(pTx->GetX(-30, 200), position.y, 200, 152), vec2(100, -70), 20, 0.001);
    };
    void Update()
    {
        if (time <= 0)
        {
            time = 30;
            addHitBox(SFXSnailHit, vec4(pTx->GetX(-30, 200), position.y, 200, 152), vec2(100, -70), 20, 0.001);
        }
        else
        {
            time--;
        }

        setDirX();
        moveX();
    };
    void Exit()
    {
        hitboxs.clear();
    };

private:
    int time = 30;
};

class FSSnailFall : public FiniteState
{
public:
    FSSnailFall(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{SnailDamaged});
        possibleState.Add(vector<ActionState>{SnailIdle, SnailAttack, SnailDamaged});
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
        shouldIdle = true;
        isDamaged = false;
    };
};

class FSRatIdle : public FiniteState
{
public:
    FSRatIdle(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{RatRun, RatFall, RatDamaged, Died});
    };

    void Enter()
    {
        FiniteState::Enter();
        force = vec2(0);
        velocity = vec2(0);
    };
    void Update(){

    };
    void Exit()
    {
        force = vec2(0);
        velocity = vec2(0);
    };
};

class FSRatRun : public FiniteState
{
public:
    FSRatRun(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{RatIdle, RatFall, RatDamaged, Died});
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

private:
};

class FSRatFall : public FiniteState
{
public:
    FSRatFall(FSMData data) : FiniteState(data)
    {
        interruptState.Add(vector<ActionState>{RatDamaged});
        possibleState.Add(vector<ActionState>{RatIdle, RatRun, RatDamaged});
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
        shouldIdle = true;
        isDamaged = false;
    };
};

class FSRatRollStart : public FiniteState
{
public:
    FSRatRollStart(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{RatIdle, RatFall, RatDamaged, Died});
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

private:
};

class FSRatRollLoop : public FiniteState
{
public:
    FSRatRollLoop(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{RatIdle, RatFall, RatDamaged, Died});
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

private:
};
class FSRatRollStop : public FiniteState
{
public:
    FSRatRollStop(FSMData data) : FiniteState(data)
    {
        possibleState.Add(vector<ActionState>{RatIdle, RatFall, RatDamaged, Died});
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
    };
    void Update(){};
    void Exit(){};

private:
};
