#pragma once
#include "Creature.h"

class Snail : public Creature
{
public:
    Snail(SoundSystem *sndSys, vector<Shader *> &s, double &t) : Creature(sndSys, s, t)
    {
        InitFSM();
        fsm->Add<FSSnailIdle>(SnailIdle);
        fsm->Add<FSSnailAttack>(SnailAttack);
        fsm->Add<FSSnailFall>(SnailFall);
        fsm->Add<FSSnailDamaged>(SnailDamaged);
        fsm->Add<FSDied>(Died);

        loadData();
        Reset();
    }
    void Reset()
    {
        setRandCtrlX();
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        position = initPosition;
        sprite->Set(SnailIdle);
        fsm->Set(SnailIdle);
        fsm->Enter();
    }
    void Update(Control ctrl)
    {

        updateHitBox();
        fsmInput.Init();

        if (hp <= 0)
        {
            fsmInput.Add(Died);
        }
        fsmInput.Add(SnailIdle);
        fsmInput.Add(SnailAttack);

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
