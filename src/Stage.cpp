#include "Stage.h"

Stage::Stage(Shader *s) : shader(s)
{
    bg = new Sprite();
    fg = new Sprite();

    Loader("L8.json", vector<Sprite *>{bg, fg});
}
Stage::~Stage()
{
    delete bg;
    delete fg;
}
void Stage::DrawBG()
{
    shader->SetMat("modelMatrix", bg->Transform.Get());
    bg->Draw();
}

void Stage::DrawFG()
{
    shader->SetMat("modelMatrix", fg->Transform.Get());
    fg->Draw();
}