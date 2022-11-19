#pragma once

struct DefferedKey
{
    bool keyPressed = false;
    bool isDeffered = false;
    bool IsDeffered()
    {
        return isDeffered;
    }
    bool IsPress()
    {
        return keyPressed;
    }
    void Press(bool flag)
    {
        keyPressed = flag;
    }
    void Set(bool canDeffered)
    {
        if (keyPressed)
        {
            isDeffered = canDeffered;
        }
    }
    void Reset()
    {
        keyPressed = false;
        isDeffered = false;
    }
};