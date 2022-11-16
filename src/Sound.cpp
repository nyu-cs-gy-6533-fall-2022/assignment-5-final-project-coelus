#include "Sound.h"

// from ../sound/
Sound::Sound(const string filename)
{
    BASS_Init(-1, 44100, 0, NULL, NULL);
    channel = BASS_StreamCreateFile(false, ("../sound/" + filename).c_str(), 0, 0, BASS_SAMPLE_LOOP);
}
Sound::~Sound()
{
    BASS_Free();
}

void Sound::Play(bool isLoop)
{
    BASS_ChannelPlay(channel, isLoop);
}
void Sound::Stop()
{
    BASS_ChannelStop(channel);
}