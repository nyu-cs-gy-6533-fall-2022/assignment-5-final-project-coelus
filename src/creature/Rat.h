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
        fsm->Set(RatIdle);

        ctrlX = 1;
        loadData();
        runCD.Init(2.f, deltaTime, 2);
        waitCD.Init(1.f, deltaTime, 1);
    }
    void Reset()
    {
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        shouldIdle = false;
        initCD();
        position = initPosition;
        fsm->Set(RatIdle);
    }
    void Update(Control ctrl)
    {

        updateHitBox();
        fsmInput.Init();
        if (hp <= 0)
        {
            fsmInput.Add(Died);
        }
        // sould Idle flag
        if (shouldIdle)
        {
            shouldIdle = false;
            waitCD.Reset();
            runCD.Close();
        }

        if (!isGround)
        {
            fsmInput.Add(RatFall);
        }
        else if (runCD.IsRun())
        {

            if (isFront || willFall)
            {
                ctrlX *= -1;
            }

            fsmInput.Add(RatRun);
            runCD.Update();
        }
        // idle wait
        else if (runCD.IsEnd())
        {
            if (waitCD.IsEnd())
            {
                initCD();
                setRandCtrlX();
            }
            waitCD.Update();
            fsmInput.Add(RatIdle);
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
    CountDown runCD;
    CountDown waitCD;

    void initCD()
    {
        runCD.Reset();
        waitCD.Reset();
    }
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
