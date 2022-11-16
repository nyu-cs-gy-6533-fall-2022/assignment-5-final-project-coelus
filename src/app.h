

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "Player.h"
#include "Camera.h"
#include "Shader.h"
#include "StageSystem.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Sound.h"

using namespace std;
using namespace glm;

class App
{
public:
    App(int width, int height);
    ~App();
    
    void MainLoop();

private:
    int mWidth, mHeight;
    double prevTime, deltaTime;
    GLFWwindow *pWindow;

    StageSystem *stageSys;
    Player *player;
    Shader *shader;
    Camera *camera;
    SoundSystem *soundSys;
    

    void init();
    void resize();
    void input();
    void draw();
    void update();
    void getDeltaTime();
    void loadIcon();
};