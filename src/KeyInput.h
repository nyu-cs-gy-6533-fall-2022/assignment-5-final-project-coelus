
#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace std;

static unordered_map<int, bool> keyDown;
class KeyInput
{
public:
    static void KeyCallback(GLFWwindow *window, int activeKey, int scancode, int action, int mods)
    {
        for (auto ob : keyDown)
        {
            int key = ob.first;
            keyDown[key] = (key == activeKey) && action == GLFW_PRESS;
        }
    }
    static void Add(vector<int> keys)
    {
        for (auto key : keys)
        {
            keyDown[key] = false;
        }
    }
    static void Reset()
    {
        for (auto ob : keyDown)
        {
            keyDown[ob.first] = false;
        }
    }
    static bool Get(int key)
    {
        return keyDown[key];
    }
};
