

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
    double deltaTime = 0.f;
    GLFWwindow *pWindow;

    StageSystem *stageSys;
    Player *player;
    vector<Shader*> shaders;
    Camera *camera;
    SoundSystem *soundSys;
    GLuint frameBuffer,depthBuffer,renderTexture;

    void init();
    void resize();
    void drawFullScreen();
    void drawAllObjects();
    void update();
    void playerUpdate();
    void loadIcon();
    void initFrameBuffer();
};