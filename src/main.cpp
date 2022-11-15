#include "App.h"


int main()
{
    App *app = new App(1600, 900);
    app->MainLoop();
    delete app;
    return 0;
}