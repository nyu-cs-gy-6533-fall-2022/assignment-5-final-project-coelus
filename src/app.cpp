#include "app.h"

App::App(int width, int height)
    : mWidth(width),
      mHeight(height)
{
    glfwInit();
    pWindow = glfwCreateWindow(width, height, "Towncount", NULL, NULL);
    if (pWindow == nullptr)
    {
        cout << "can't init glfw" << endl;
        return;
    }
    glfwMakeContextCurrent(pWindow);
}

App::~App()
{
    glfwTerminate();
}

void App::Input()
{
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
}

void App::Resize()
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

void App::Update()
{
    while (!glfwWindowShouldClose(pWindow))
    {
        Resize();
        Input();

        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}