

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>

#include "creature/Player.h"
#include "Camera.h"
#include "Shader.h"
#include "StageSystem.h"
#include "InputSystem.h"
#include "Sound.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
    double prevTime, deltaTime = 0;
    bool isReady = false;
    GLFWwindow *pWindow;

    StageSystem *stageSys;
    Player *player;
    vector<Shader*> shaders;
    Camera *camera;
    SoundSystem *soundSys;
    

    void init();
    void resize();
    void draw();
    void update();
    void playerUpdate();
    void getDeltaTime();
    void loadIcon();
};