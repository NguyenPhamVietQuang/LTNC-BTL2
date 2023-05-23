#include "stdafx.h"
#include "ImgTextObject.h"


ImgTextObject::ImgTextObject()
{
    m_Font = NULL;
    m_Text.SetColor(TextObject::WHITE_TEXT);
}


ImgTextObject::~ImgTextObject()
{

}

bool ImgTextObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    return BaseObject::LoadImg(path, screen);
}

void ImgTextObject::SetRectImg(int x, int y)
{
    m_Rect.x = x;
    m_Rect.y = y;
}

void ImgTextObject::SetText(std::string text)
{
    m_Text.SetText(text);
}

void ImgTextObject::SetPos(int xp, int yp)
{
    m_Text.SetPos(xp, yp);
}

void ImgTextObject::Show(SDL_Renderer* screen)
{
    Render(screen); // show ảnh
    // show text
    m_Text.LoadFromRenderedText(m_Font, screen);
    m_Text.RenderText(screen);
}
