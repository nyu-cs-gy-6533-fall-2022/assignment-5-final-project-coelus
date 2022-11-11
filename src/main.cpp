#include "app.h"


int main()
{
    App *app = new App(800, 600);
    app->Update();
    delete app;
    return 0;
}