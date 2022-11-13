

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>


#include "Player.h"
#include "Camera.h"
#include "Shader.h"
#include "Stage.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class App
{
public:
    App(int width, int height);
    ~App();
    void Update();

private:
    int mWidth, mHeight;
    GLFWwindow *pWindow;
    Stage *stage;
    Player *player;
    Shader *shader;
    Camera *camera;
    double prevTime, deltaTime;

    void init();
    void resize();
    void input();
    void getDeltaTime();
    void loadIcon();
};