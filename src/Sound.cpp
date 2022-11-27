#include "Sound.h"

// from ../sound/
SoundSystem::SoundSystem()
{
    BASS_Init(-1, 44100, 0, NULL, NULL);

    createSound(BGMStage1, "bgm/stage1.mp3", true);
    createSound(BGMBoss1, "bgm/boss.mp3", true);
    createSound(SFXPlayerJump, "sfx/jump.mp3");
    createSound(SFXPlayerLanding, "sfx/landing.mp3");
    createSound(SFXPlayerStep, "sfx/step.mp3");
    createSound(SFXPlayerAttack, "sfx/swordattack.mp3");
    createSound(SFXPlayerHit, "sfx/swordhit.mp3");
    createSound(SFXSnailHit, "sfx/snailhit.mp3");
    createSound(SFXRatHit, "sfx/rathit.mp3");
    createSound(SFXDisappear, "sfx/disappear.mp3");
    createSound(SFXChainHitWall, "sfx/chainhitwall.mp3");
    createSound(SFXChainThrow, "sfx/chainthrow.mp3");
    
}
SoundSystem::~SoundSystem()
{
    BASS_Free();
}
void SoundSystem::Play(SoundType type)
{
    BASS_ChannelPlay(channels[type], true);
}
void SoundSystem::Stop(SoundType type)
{
    BASS_ChannelStop(channels[type]);
}

void SoundSystem::createSound(SoundType type, const string filename, bool isLoop)
{
    int flag = isLoop ? BASS_SAMPLE_LOOP : BASS_SAMPLE_MONO;
    channels[type] = BASS_StreamCreateFile(false, ("../sound/" + filename).c_str(), 0, 0, flag);
}