
#pragma once

#include <cstdlib>
#include <ctime>

struct CountDown
{
    int randErr = 0;
    float currentRemainTime;
    float remainTime;
    float t = 0;
    double *deltaTime;
    void Init(float val, double &dt, int err = 2)
    {

        remainTime = val;
        deltaTime = &dt;
        randErr = err;
        Reset();
    }

    float GetSpentTime()
    {
        return currentRemainTime - t;
    }
    bool IsLessHalf()
    {
        return t > currentRemainTime / 2;
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
            err = ((double)rand() / RAND_MAX) * randErr;
        }
        currentRemainTime = remainTime + err;
        t = currentRemainTime;
    }
    void Update()
    {
        t -= *deltaTime;
    }
};
