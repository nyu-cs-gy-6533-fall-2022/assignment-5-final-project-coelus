#include "App.h"


int main()
{
    App *app = new App(1600, 900);
    app->Update();
    delete app;
    return 0;
}