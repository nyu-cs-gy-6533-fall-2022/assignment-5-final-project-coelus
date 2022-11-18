#pragma once
struct FSMData
{
    SoundSystem *soundSys;
    AnimSprite *sprite;
    float &runSpeed, &jumpSpeed;
    int &dirX;
    vec2 &velocity;
    AnimationState &state;
    double &deltaTime;
    bool &isGround, &isTop;
    bool &ctrlX;
};