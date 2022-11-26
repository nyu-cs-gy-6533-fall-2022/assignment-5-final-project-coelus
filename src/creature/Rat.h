#pragma once
#include "Creature.h"
#include <cstdlib>
#include <ctime>
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

        srand(time(0));
        ctrlX = 1;
        loadData();
        timeInit();
        idleFlag = false;
    }
    void Reset()
    {
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        idleFlag = false;
        timeInit();
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
        // after been attack do idle
        if (idleFlag && !isDamaged)
        {
            idleFlag = false;
            timeInit();
            attackTime = 0;
        }

        if (!isGround)
        {
            fsmInput.Add(RatFall);
        }
        else if (attackTime > 0)
        {

            if (isFront || willFall)
            {
                ctrlX *= -1;
            }

            fsmInput.Add(RatRun);
            attackTime -= deltaTime;
        }
        else
        {
            fsmInput.Add(RatIdle);
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
            fsmInput.Add(RatDamaged);
        }

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
        attackTime = 1 + rand() % 3;
        waitTime = 1 + rand() % 3;
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
