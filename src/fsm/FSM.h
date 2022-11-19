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
    void Add(const ActionState state)
    {
        fsm[state] = new T(data);
    }
    void Set(ActionState state)
    {
        currentState = state;
        fs = fsm[state];
    }
    ActionState GetState(){return currentState;};
    int GetPossibleState() { return fs->GetPossibleState(); }
    void Enter() { fs->Enter(); };
    void Update() { fs->Update(); };
    void Exit() { fs->Exit(); };

private:
    ActionState currentState;
    SoundSystem *soundSys;
    FSMData data;
    FiniteState *fs;
    unordered_map<ActionState, FiniteState *> fsm;
};