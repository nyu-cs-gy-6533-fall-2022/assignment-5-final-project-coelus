#pragma once
#include <string>
#include <unordered_map>

using namespace std;

enum ActionState
{
    EmptyState,
    Idle,
    Run,
    Jump,
    Attack1,
    Attack2,
    Attack3,
    Fall,
    JumpAttack,
    Damaged,
    Died,

    SnailIdle,
    SnailAttack,
    SnailFall,
    SnailDamaged,

    RatIdle,
    RatRun,
    RatRollStart,
    RatRollLoop,
    RatRollStop,
    RatFall,
    RatDamaged

};
static unordered_map<string, ActionState> ActionStateText = {
    {"EmptyState", EmptyState},
    {"Idle", Idle},
    {"Run", Run},
    {"Jump", Jump},
    {"Attack1", Attack1},
    {"Attack2", Attack2},
    {"Attack3", Attack3},
    {"Fall", Fall},
    {"JumpAttack", JumpAttack},
    {"Damaged", Damaged},
    {"Died", Died},

    {"SnailIdle", SnailIdle},
    {"SnailAttack", SnailAttack},
    {"SnailFall", SnailFall},
    {"SnailDamaged", SnailDamaged},

    {"RatIdle", RatIdle},
    {"RatRun", RatRun},
    {"RatRollStart", RatRollStart},
    {"RatRollLoop", RatRollLoop},
    {"RatRollStop", RatRollStop},
    {"RatFall", RatFall},
    {"RatDamaged", RatDamaged}

};
