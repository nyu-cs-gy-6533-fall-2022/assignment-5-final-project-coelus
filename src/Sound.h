#pragma once
#include <bass.h>
#include <iostream>

using namespace std;
class Sound
{
public:
    Sound(const string filename);

    ~Sound();

    void Play(bool isLoop = false);

    void Stop();

private:
    unsigned int channel;
};