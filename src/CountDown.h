
#pragma once

#include <cstdlib>
#include <ctime>

struct CountDown
{
    int randErr = 0;
    float remainTime;
    float t = 0;
    double *deltaTime;
    void Init(float val, double &dt, int err = 2)
    {
        srand(time(0));
        remainTime = val;
        deltaTime = &dt;
        randErr = err;
        Reset();
    }
    bool IsEnd()
    {
        return t <= 0;
    }
    bool IsRun()
    {
        return t > 0;
    }
    void Close()
    {
        t = 0;
    }
    void Reset()
    {
        float err = 0;
        if (randErr > 0)
        {
            err = rand() % randErr;
        }
        t = remainTime + err;
    }
    void Update()
    {
        t -= *deltaTime;
    }
};
