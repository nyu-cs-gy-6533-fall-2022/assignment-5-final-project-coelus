#pragma once
#include "Creature.h"
class Rat : public Creature
{
public:
    Rat(SoundSystem *sndSys, const vector<Shader *> &s, double &t) : Creature(sndSys, s, t)
    {
        InitFSM();
        fsm->Add<FSRatIdle>(RatIdle);
        fsm->Add<FSRatRun>(RatRun);
        fsm->Add<FSRatRollStart>(RatRollStart);
        fsm->Add<FSRatRollLoop>(RatRollLoop);
        fsm->Add<FSRatRollStop>(RatRollStop);
        fsm->Add<FSRatFall>(RatFall);
        fsm->Add<FSRatDamaged>(RatDamaged);
        fsm->Add<FSMonsterDied>(MonsterDied);

        loadData();
        attackCD.Init(4.f, deltaTime, 1);
        Reset();
    }
    void Reset()
    {
        setRandCtrlX();
        dissolveTime = 0;
        hp = initHp;
        isDamaged = false;
        attackEnd = false;
        attackCD.Reset();
        position = initPosition;
        sprite->Set(RatIdle);
        fsm->Set(RatIdle);
        fsm->Enter();
    }
    void Update(Control ctrl)
    {

        updateHitBox();
        fsmInput.Init();

        if (IsHP0())
        {
            fsmInput.Add(MonsterDied);
        }
        fsmInput.Add(RatIdle);
        fsmInput.Add(RatRun);
        if (attackCD.IsEnd())
        {
            fsmInput.Add(RatRollStart);
            fsmInput.Add(RatRollLoop);
            fsmInput.Add(RatRollStop);
        }else{
            attackCD.Update();
        }
        
        if (attackEnd)
        {
            attackCD.Reset();
            attackEnd = false;
        }
        

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
    CountDown attackCD;

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
