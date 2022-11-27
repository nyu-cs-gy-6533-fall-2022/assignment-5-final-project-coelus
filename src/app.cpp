#include "App.h"
App::App(int width, int height) : mWidth(width), mHeight(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // full screen
    glfwWindowHint(GLFW_MAXIMIZED, true);

    pWindow = glfwCreateWindow(width, height, "Towncount", NULL, NULL);
    if (!pWindow)
    {
        cout << "can't init glfw" << endl;
        return;
    }
    glfwMakeContextCurrent(pWindow);
    glewExperimental = GL_TRUE;
    if (glewInit())
    {
        cout << "can't init glew" << endl;
        return;
    }
    // for png
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    InputSystem::Add(vector<int>{GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C});
    glfwSetKeyCallback(pWindow, InputSystem::KeyCallback);

    srand(time(0));
    init();
}

App::~App()
{
    delete player;
    delete stageSys;
    delete soundSys;

    for(auto s:shaders){
        delete s;
    }

    glfwTerminate();
}

// other than opengl init
void App::init()
{
    loadIcon();

    soundSys = new SoundSystem();
    shaders.push_back(new Shader("sprite.vert", "sprite.frag"));
    shaders.push_back(new Shader("dissolve.vert", "dissolve.frag"));
    shaders.push_back(new Shader("bg.vert", "bg.frag"));
    player = new Player(soundSys, shaders, deltaTime);
    stageSys = new StageSystem(soundSys, player, shaders, deltaTime);
    camera = new Camera(stageSys, player, mWidth, mHeight);

   
    isReady = true;
}
void App::loadIcon()
{
    Texture *ic = new Texture("Icon.png");
    GLFWimage img;
    img.height = ic->GetHeight();
    img.width = ic->GetWidth();
    img.pixels = ic->GetData();
    glfwSetWindowIcon(pWindow, 1, &img);
}

void App::resize()
{
    int width, height;
    glfwGetWindowSize(pWindow, &width, &height);
    if (width != mWidth || height != mHeight)
    {
        mWidth = width;
        mHeight = height;
        glViewport(0, 0, width, height);
    }
}


void App::playerUpdate()
{
    bool right = glfwGetKey(pWindow, GLFW_KEY_RIGHT);
    bool left = glfwGetKey(pWindow, GLFW_KEY_LEFT);
    bool up = glfwGetKey(pWindow, GLFW_KEY_UP);
    bool down = glfwGetKey(pWindow, GLFW_KEY_DOWN);
    bool jump = InputSystem::Get(GLFW_KEY_Z);
    bool attack = InputSystem::Get(GLFW_KEY_C);
    InputSystem::Reset();

    player->Update(Control{right, left, up, down, jump, attack, false});
}

void App::update()
{
    resize();
    playerUpdate();
    stageSys->Update();
}
void App::draw()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaders[0]->Use();
    shaders[0]->SetMat4("projMatrix", camera->Projection());
    shaders[1]->Use();
    shaders[1]->SetMat4("projMatrix", camera->Projection());
    shaders[2]->Use();
    shaders[2]->SetMat4("projMatrix", camera->Projection());
    stageSys->Draw();
    player->Draw();
}

void App::MainLoop()
{
    if (!isReady)
        return;
    while (!glfwWindowShouldClose(pWindow))
    {
        double startTime = glfwGetTime();
        update();
        draw();
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
        deltaTime = glfwGetTime() - startTime;
    }
}