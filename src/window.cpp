#include "window.h"

Window::Window(int width, int height)
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

Window::~Window()
{
    glfwTerminate();
}

void Window::Input()
{
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
}

void Window::Resize()
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

void Window::Update()
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