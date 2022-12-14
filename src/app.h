

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
#include "UI.h"
#include "CountDown.h"

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
    UI *ui;
    vector<Shader *> shaders;
    Camera *camera;
    SoundSystem *soundSys;
    GLuint indexBuffer, frameBuffer, depthBuffer, renderTexture;
    CountDown fsCD;

    void init();
    void resize();
    void drawFullScreen();
    void drawAllObjects();
    void update();
    void playerUpdate();
    void loadIcon();
    void initFrameBuffer(int width, int height);
};