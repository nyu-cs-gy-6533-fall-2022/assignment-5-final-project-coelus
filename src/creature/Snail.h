#pragma once
#include "Creature.h"


class Snail : public Creature
{
public:
    Snail(SoundSystem *sndSys, vector<Shader *> &s, double &t) : Creature(sndSys, s, t)
    {
        fsm->Add<FSSnailIdle>(SnailIdle);
        fsm->Add<FSSnailAttack>(SnailAttack);
        fsm->Add<FSSnailFall>(SnailFall);
        fsm->Add<FSSnailDamaged>(SnailDamaged);
        fsm->Add<FSDied>(Died);
        fsm->Set(SnailIdle);

        ctrlX = 1;
        loadData();

        attackCD.Init(2.f, deltaTime, 4);
        waitCD.Init(2.f, deltaTime, 1);
    }
    void Reset()
    {
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        shouldIdle = false;
        initCD();
        position = initPosition;
        fsm->Set(SnailIdle);
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
            attackCD.Close();
        }

        if (!isGround)
        {
            fsmInput.Add(SnailFall);
        }
        else if (attackCD.IsRun())
        {

            if (isFront || willFall)
            {
                ctrlX *= -1;
            }

            fsmInput.Add(SnailAttack);
            attackCD.Update();
        }
        // idle wait
        else if (attackCD.IsEnd())
        {
            if (waitCD.IsEnd())
            {
                initCD();
                setRandCtrlX();
            }
            waitCD.Update();
            fsmInput.Add(SnailIdle);
        }

        if (isDamaged)
        {
            fsmInput.Add(SnailDamaged);
        }

        stateUpdate();
        fsm->Update();
        movement();
    }

private:
    CountDown attackCD;
    CountDown waitCD;

    void initCD()
    {
        attackCD.Reset();
        waitCD.Reset();
    }
    void loadData()
    {
        json js = Loader::Load("snail.json", vector<Sprite *>{sprite});
        runSpeed = js["runSpeed"];
        auto rb = js["rigidbody"];
        setInitHp(js["hp"]);
        rigidbody = vec2(rb["w"], rb["h"]);
        debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
    };
};
