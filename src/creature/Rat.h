#pragma once
#include "Creature.h"
class Rat : public Creature
{
public:
    Rat(SoundSystem *sndSys, vector<Shader *> &s, double &t) : Creature(sndSys, s, t)
    {
        fsm->Add<FSRatIdle>(RatIdle);
        fsm->Add<FSRatRun>(RatRun);
        fsm->Add<FSRatRollStart>(RatRollStart);
        fsm->Add<FSRatRollLoop>(RatRollLoop);
        fsm->Add<FSRatRollStop>(RatRollStop);
        fsm->Add<FSRatFall>(RatFall);
        fsm->Add<FSRatDamaged>(RatDamaged);
        fsm->Add<FSDied>(Died);

        loadData();
        waitCD.Init(1.f, deltaTime, 2);
        attackWaitCD.Init(5.f, deltaTime, 1);
        Reset();
    }
    void Reset()
    {
        setRandCtrlX();
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        shouldIdle = false;
        waitCD.Reset();
        attackWaitCD.Reset();
        position = initPosition;
        sprite->Set(RatIdle);
        fsm->Set(RatIdle);
        fsm->Enter();
    }
    void Update(Control ctrl)
    {

        updateHitBox();
        fsmInput.Init();
        fsmInput.Add(RatIdle);
        if (hp <= 0)
        {
            fsmInput.Add(Died);
        }
        // should Idle flag
        if (shouldIdle)
        {
            if (waitCD.IsEnd())
            {
                shouldIdle = false;
                waitCD.Reset();
                setRandCtrlX();
                if (attackWaitCD.IsEnd())
                {
                    attackWaitCD.Reset();
                }
            }
            waitCD.Update();
        }
        else if (attackWaitCD.IsEnd())
        {
            /*
            fsmInput.Add(RatRollStart);
            fsmInput.Add(RatRollLoop);
            fsmInput.Add(RatRollStop);*/
        }
        else
        {
            if (isFront || willFall)
            {
                ctrlX *= -1;
            }
            fsmInput.Add(RatRun);
        }
        attackWaitCD.Update();

        if (!isGround)
        {
            fsmInput.Add(RatFall);
        }

        if (isDamaged)
        {
            fsmInput.Add(RatDamaged);
        }

        stateUpdate();
        fsm->Update();
        movement();
    }

private:
    CountDown waitCD, attackWaitCD;

    void loadData()
    {
        json js = Loader::Load("rat.json", vector<Sprite *>{sprite});
        runSpeed = js["runSpeed"];
        auto rb = js["rigidbody"];
        setInitHp(js["hp"]);
        rigidbody = vec2(rb["w"], rb["h"]);
        debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
    };
};
