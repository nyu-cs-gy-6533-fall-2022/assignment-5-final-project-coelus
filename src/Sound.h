#pragma once
#ifdef _WIN32
#include <bass.h>
#endif
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

#ifdef _WIN32
class SoundSystem
{
public:
    SoundSystem()
    {
        BASS_Init(-1, 44100, 0, NULL, NULL);

        createSound(BGMStage1, "bgm/stage1.mp3", true);
        createSound(BGMStageNoise, "bgm/stagenoise.mp3", true);
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
        createSound(SFXChainHitTube, "sfx/chainhittube.mp3");
        createSound(SFXChainThrow, "sfx/chainthrow.mp3");
    }

    ~SoundSystem()
    {
        BASS_Free();
    }

    void Play(SoundType type)
    {
        BASS_ChannelPlay(channels[type], true);
    }
    void PlayBGM(SoundType type)
    {
        if (type == bgm)
            return;
        Stop(bgm);
        bgm = type;
        BASS_ChannelPlay(channels[type], true);
    }
    void Stop(SoundType type)
    {
        BASS_ChannelStop(channels[type]);
    }

private:
    SoundType bgm = SoundNone;
    unordered_map<SoundType, HSTREAM> channels;
    void createSound(SoundType type, const string filename, bool isLoop = false)
    {
        int flag = isLoop ? BASS_SAMPLE_LOOP : BASS_SAMPLE_MONO;
        channels[type] = BASS_StreamCreateFile(false, ("../sound/" + filename).c_str(), 0, 0, flag);
    }
};
#else

class SoundSystem
{
public:
    SoundSystem(){ 
        cout<<"no music for ya"<<endl;
    }

    ~SoundSystem(){}

    void Play(SoundType type){}
    void PlayBGM(SoundType type){}
    void Stop(SoundType type){ }

private:
    SoundType bgm = SoundNone;
    unordered_map<SoundType, HSTREAM> channels;
    void createSound(SoundType type, const string filename, bool isLoop = false){}
};
#endif