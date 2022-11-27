#pragma once
#include <bass.h>
#include <unordered_map>
#include <iostream>

using namespace std;

enum SoundType
{
    BGMStage1,
    BGMBoss1,
    SFXPlayerJump,
    SFXPlayerStep,
    SFXPlayerLanding,
    SFXPlayerAttack,
    SFXPlayerHit,
    SFXSnailHit,
    SFXRatHit,
    SFXDisappear,
    SFXChainHitWall,
    SFXChainThrow
};
class SoundSystem
{
public:
    SoundSystem();

    ~SoundSystem();

    void Play(SoundType type);

    void Stop(SoundType type);

private:
    unordered_map<SoundType, HSTREAM> channels;
    void createSound(SoundType type, const string filename, bool isLoop = false);
};