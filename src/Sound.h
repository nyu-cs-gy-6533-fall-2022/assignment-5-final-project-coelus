#pragma once
#include <bass.h>
#include <iostream>

using namespace std;
class SoundSystem
{
public:
    SoundSystem(const string filename);

    ~SoundSystem();

    void Play(bool isLoop = false);

    void Stop();

private:
    unsigned int channel;
};