#pragma once
#include "Animation.h"
#include "FiniteState.h"
#include "FSMData.h"
#include <glm/glm.hpp>
using namespace glm;

class FSM
{
public:
    FSM(FSMData in) : data(in){};
    ~FSM()
    {
        for (auto fs : fsm)
        {
            delete fs.second;
        }
    }
    template <class T>
    void Add(const AnimationState state)
    {
        fsm[state] = new T(data);
    }
    void Set(AnimationState state)
    {
        fs = fsm[state];
    }
    bool TryNextState(AnimationState state){return fs->TryNextState(state); }
    void Enter() { fs->Enter(); };
    void Update() { fs->Update(); };
    void Exit() { fs->Exit(); };

private:
    SoundSystem *soundSys;
    FSMData data;
    FiniteState *fs;
    unordered_map<AnimationState, FiniteState *> fsm;
};