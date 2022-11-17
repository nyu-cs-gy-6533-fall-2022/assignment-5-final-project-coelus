#pragma once
#include "Animation.h"
#include "FiniteState.h"
#include "FSMInput.h"
#include <glm/glm.hpp>
using namespace glm;


class FSM
{
public:
    FSM(FSMInput i) : input(i){};
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
        fsm[state] = new T(input);
    }
    void Set(AnimationState state)
    {
        fs = fsm[state];
    }
    void Enter() { fs->Enter(); };
    void Update() { fs->Update(); };
    void Exit() { fs->Exit(); };

private:
    FSMInput input;
    FiniteState *fs;
    unordered_map<AnimationState, FiniteState *> fsm;
};