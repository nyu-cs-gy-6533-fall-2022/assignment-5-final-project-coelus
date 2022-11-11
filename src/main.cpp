#include "Window.h"


int main()
{
    Window *window = new Window(800, 600);
    window->Update();
    delete window;
    return 0;
}