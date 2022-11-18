#pragma once
#include "Animation.h"
#include "FiniteState.h"
#include "FSMData.h"
#include <glm/glm.hpp>
using namespace glm;

class FSM
{
public:
    FSM(FSMData d) : data(d){};
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
        currentState = state;
        fs = fsm[state];
    }
    AnimationState GetState(){return currentState;};
    int GetPossibleState() { return fs->GetPossibleState(); }
    void Enter() { fs->Enter(); };
    void Update() { fs->Update(); };
    void Exit() { fs->Exit(); };

private:
    AnimationState currentState;
    SoundSystem *soundSys;
    FSMData data;
    FiniteState *fs;
    unordered_map<AnimationState, FiniteState *> fsm;
};