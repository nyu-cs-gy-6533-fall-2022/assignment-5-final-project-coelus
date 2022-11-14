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

    init();
}

App::~App()
{
    delete player;
    delete shader;
    delete stage;

    glfwTerminate();
}

// other than opengl init
void App::init()
{
    loadIcon();
    shader = new Shader("sprite.vert", "sprite.frag");
    player = new Player(shader);
    camera = new Camera(mWidth, mHeight, player);
    stage = new Stage(player, shader);
    stage->SetPlayerEntry(0);
    prevTime = glfwGetTime();
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

void App::input()
{

    if (glfwGetKey(pWindow, GLFW_KEY_RIGHT))
    {
        player->Move(1);
    }
    else if (glfwGetKey(pWindow, GLFW_KEY_LEFT))
    {
        player->Move(-1);
    }
    else
    {
        player->Stop();
    }
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
void App::getDeltaTime()
{
    double currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;
}

void App::Update()
{
    while (!glfwWindowShouldClose(pWindow))
    {
        getDeltaTime();
        input();
        resize();

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Use();
        shader->SetMat("projMatrix", camera->Projection());
        stage->DrawBG();
        player->Draw(deltaTime);
        stage->DrawFG();

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}