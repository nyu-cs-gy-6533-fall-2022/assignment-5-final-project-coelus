

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

#define DEBUG  false

class Global
{
public:
    const static constexpr float GravityRatio = 1.6f;
    const static constexpr float MaxSpd = 20.f;
};
