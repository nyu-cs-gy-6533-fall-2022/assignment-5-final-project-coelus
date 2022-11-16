#include "Sound.h"

// from ../sound/
SoundSystem::SoundSystem(const string filename)
{
    BASS_Init(-1, 44100, 0, NULL, NULL);
    channel = BASS_StreamCreateFile(false, ("../sound/" + filename).c_str(), 0, 0, BASS_SAMPLE_LOOP);
}
SoundSystem::~SoundSystem()
{
    BASS_Free();
}

void SoundSystem::Play(bool isLoop)
{
    BASS_ChannelPlay(channel, isLoop);
}
void SoundSystem::Stop()
{
    BASS_ChannelStop(channel);
}