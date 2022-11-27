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


        loadData();
        waitCD.Init(1.2f, deltaTime, 2);
        Reset();
    }
    void Reset()
    {
        setRandCtrlX();
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        shouldIdle = true;
        waitCD.Reset();
        position = initPosition;
        sprite->Set(SnailIdle);
        fsm->Set(SnailIdle);
        fsm->Enter();
    }
    void Update(Control ctrl)
    {

        updateHitBox();
        fsmInput.Init();
        fsmInput.Add(SnailIdle);
        if (hp <= 0)
        {
            fsmInput.Add(Died);
        }
        // sould Idle flag
        if (shouldIdle)
        {
            if (waitCD.IsEnd())
            {
                shouldIdle = false;
                waitCD.Reset();
                setRandCtrlX();
            }
            waitCD.Update();
        }
        else
        {
            if (isFront || willFall)
            {
                ctrlX *= -1;
            }

            fsmInput.Add(SnailAttack);
        }

        if (!isGround)
        {
            fsmInput.Add(SnailFall);
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
    CountDown waitCD;

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
