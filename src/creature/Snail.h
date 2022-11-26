#pragma once
#include "Creature.h"
#include <cstdlib>
#include <ctime>
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
        json js = Loader::Load("snail.json", vector<Sprite *>{sprite});
        runSpeed = js["runSpeed"];
        auto rb = js["rigidbody"];
        setInitHp(js["hp"]);
        rigidbody = vec2(rb["w"], rb["h"]);
        debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
    };
};
