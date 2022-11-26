
#include "Debug.h"

#if DEBUG
void Debug::AddDebug(vec4 rect)
{
    Sprite *sprite = new Sprite();
    sprite->Set("red.png", vec2(rect.z, rect.w), vec2(rect.x, rect.y));
    debugRect.push_back(sprite);
}
void Debug::DrawDebug()
{
    for (auto rect : debugRect)
    {
        shaders[0]->SetMat4("modelMatrix", rect->Tx.Get());
        rect->Draw();
    }
}
void Debug::SetDebugTx(int index, vec4 rect)
{
    debugRect[0]->Tx.Set(rect);
}
#else
void Debug::AddDebug(vec4 rect) {}
void Debug::DrawDebug() {}
void Debug::SetDebugTx(int index, vec4 rect) {}
#endif