#pragma once
#include "Creature.h"

class Snail : public Creature
{
public:
    Snail(SoundSystem *sndSys, Shader *s, double &time) : Creature(sndSys, s, time)
    {
        fsm->Add<FSSnailIdle>(SnailIdle);
        fsm->Add<FSSnailAttack>(SnailAttack);
        fsm->Add<FSSnailFall>(SnailFall);
        fsm->Add<FSSnailDamaged>(SnailDamaged);
        fsm->Add<FSDied>(Died);
        fsm->Set(SnailIdle);
        ctrlX = 1;
        loadData();
        timeInit();
        idleFlag = false;
    }
    void Reset()
    {
        hp = 200;
        isDamaged = false;
        idleFlag = false;
        timeInit();
        position = initPosition;
        fsm->Set(SnailIdle);
    }
    void Update(Control ctrl)
    {

        fsmInput.Init();
        if (hp <= 0)
        {
            fsmInput.Add(Died);
        }
        // after been attack do idle
        if (idleFlag && !isDamaged)
        {
            idleFlag = false;
            timeInit();
            attackTime = 0;
        }

        if (!isGround)
        {
            fsmInput.Add(SnailFall);
        }
        else if (attackTime > 0)
        {

            if (isFront || willFall)
            {
                ctrlX *= -1;
            }

            fsmInput.Add(SnailAttack);
            attackTime -= deltaTime;
        }
        else
        {
            fsmInput.Add(SnailIdle);
        }
        if (attackTime <= 0)
        {
            if (waitTime <= 0)
            {
                timeInit();
            }
            waitTime -= deltaTime;
        }
        if (isDamaged)
        {
            idleFlag = true;
            fsmInput.Add(SnailDamaged);
        }

        updateHitBox();
        stateUpdate();
        fsm->Update();
        movement();
    }

private:
    bool idleFlag;
    float waitTime;
    float attackTime;
    void timeInit()
    {
        attackTime = 2 + rand() % 1;
        waitTime = 3 + rand() % 1;
    }
    void loadData()
    {
        json js = Loader::Load("snail.json", vector<Sprite *>{sprite});
        runSpeed = js["runSpeed"];
        auto rb = js["rigidbody"];
        rigidbody = vec2(rb["w"], rb["h"]);
        debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
    };
};
