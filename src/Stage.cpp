#include "Stage.h"

Stage::Stage(Shader *s) : shader(s)
{
    transfrom.Position = vec2(0, 0);
    transfrom.Scale = vec2(1920, 3840);
    buffer = new BufferObject();
    texture = new Texture("stage/B1-BackBG.jpg");
}
Stage::~Stage()
{
    delete texture;
    delete buffer;
}
void Stage::Draw()
{
    shader->SetMat("modelMatrix", transfrom.Get());
    texture->Bind();
    buffer->Draw();
}