#pragma once
#include <bass.h>
#include <unordered_map>
#include <iostream>

using namespace std;

enum MaterialType
{
    MatWall,
    MatTube
};

enum SoundType
{
    BGMStage1,
    BGMStageNoise,
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
    SFXChainHitTube,
    SFXChainThrow,
    SoundNone
};
class SoundSystem
{
public:
    SoundSystem();

    ~SoundSystem();

    void Play(SoundType type);
    void PlayBGM(SoundType type);
    void Stop(SoundType type);

private:
    SoundType bgm = SoundNone;
    unordered_map<SoundType, HSTREAM> channels;
    void createSound(SoundType type, const string filename, bool isLoop = false);
};