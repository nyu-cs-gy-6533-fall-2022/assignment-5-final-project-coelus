#include "App.h"
App::App(int width, int height) : mWidth(width), mHeight(height)
{
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // full screen
    glfwWindowHint(GLFW_MAXIMIZED, true);

    pWindow = glfwCreateWindow(width, height, "Towncount Remake", NULL, NULL);
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
    delete ui;
    for (auto s : shaders)
    {
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
    shaders.push_back(new Shader("line.vert", "line.frag"));
    shaders.push_back(new Shader("ui.vert", "ui.frag"));
    shaders.push_back(new Shader("quad.vert", "quad.frag"));
    player = new Player(soundSys, shaders, deltaTime);
    stageSys = new StageSystem(soundSys, player, shaders, deltaTime);
    camera = new Camera(stageSys, player, mWidth, mHeight);
    ui = new UI(shaders);

    int width, height;
    glfwGetWindowSize(pWindow, &width, &height);
    initFrameBuffer(width, height);
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
        initFrameBuffer(width, height);
    }
}

void App::playerUpdate()
{
    bool right = glfwGetKey(pWindow, GLFW_KEY_RIGHT);
    bool left = glfwGetKey(pWindow, GLFW_KEY_LEFT);
    bool up = glfwGetKey(pWindow, GLFW_KEY_UP);
    bool down = glfwGetKey(pWindow, GLFW_KEY_DOWN);
    bool jump = InputSystem::Get(GLFW_KEY_Z);
    bool chain = InputSystem::Get(GLFW_KEY_X);
    bool attack = InputSystem::Get(GLFW_KEY_C);
    InputSystem::Reset();

    player->Update(Control{right, left, up, down, jump, attack, chain});
}

// frame buffer
void App::initFrameBuffer(int width, int height)
{

    // index
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    vector<ivec3> index = {
        ivec3(0, 1, 2),
        ivec3(3, 4, 5)};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ivec3) * index.size(), index.data(), GL_STATIC_DRAW);

    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // frame buffer
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

    // depth
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Frame Buffer is not complete" << endl;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void App::update()
{

    playerUpdate();
    stageSys->Update();
}
void App::drawAllObjects()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shaders[0]->Use();
    shaders[0]->SetMat4("projMatrix", camera->Projection());
    shaders[1]->Use();
    shaders[1]->SetMat4("projMatrix", camera->Projection());
    shaders[2]->Use();
    shaders[2]->SetMat4("projMatrix", camera->Projection());
    shaders[3]->Use();
    shaders[3]->SetMat4("projMatrix", camera->Projection());
    stageSys->Draw();
    ui->Draw();
}
void App::drawFullScreen()
{
    if (player->IsDamaged())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        drawAllObjects();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // draw full screen quad
        shaders[5]->Use();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        drawAllObjects();
    }
}

void App::MainLoop()
{

    while (!glfwWindowShouldClose(pWindow))
    {

        double startTime = glfwGetTime();
        resize();
        update();
        drawFullScreen();
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
        deltaTime = glfwGetTime() - startTime;
    }
}