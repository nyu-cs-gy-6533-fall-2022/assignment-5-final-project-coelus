#pragma once
struct FSMInput
{
    vec2 &velocity;
    AnimationState &state;
    double &deltaTime;
    bool &isGround, &isTop, &isAttack;
};