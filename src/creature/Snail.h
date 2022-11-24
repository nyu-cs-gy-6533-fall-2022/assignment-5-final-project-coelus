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
        fsm->Set(SnailIdle);

        loadData();
    }
    void Update(Control ctrl)
    {
        fsmInput.Init();
        if (!isGround)
        {
            fsmInput.Add(SnailFall);
        }else{
            ctrlX = 1;
            fsmInput.Add(SnailAttack);
        }
        StateUpdate();
        fsm->Update();
        Movement();
    }

private:
    void loadData()
    {
        json js = Loader::Load("snail.json", vector<Sprite *>{sprite});
        runSpeed = js["runSpeed"];
        auto rb = js["rigidbody"];
        rigidbody = vec2(rb["w"], rb["h"]);
        debug->AddDebug(vec4(0, 0, rigidbody.x, rigidbody.y));
    };
};
