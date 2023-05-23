#include "stdafx.h"
#include "BaseObject.h"

//////////////////////////////////////////////////////////////////////////
// Lớp BaseObject là lớp đối tượng nguyên thủy đại diện cho một đối tượng hình ảnh
//////////////////////////////////////////////////////////////////////////
BaseObject::BaseObject()
{
    m_ObjectTexture = NULL;
    m_Surface = NULL;
    m_Rect.x = 0;
    m_Rect.y = 0;
    m_Rect.w = 0;
    m_Rect.h = 0;
    m_Flip = false;
}

BaseObject::~BaseObject()
{
    Free();
}
//////////////////////////////////////////////////////////////////////////
// Hàm Load ảnh
//////////////////////////////////////////////////////////////////////////
bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* newTexture = NULL;

    m_Surface = IMG_Load(path.c_str());
    if (m_Surface != NULL)
    {
        // Xử lý color key. Mã color key đang là 125, 125, 125
        Uint32 uKey = SDL_MapRGB(m_Surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);
        SDL_SetColorKey(m_Surface, SDL_TRUE, uKey);

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(screen, m_Surface);
        if (newTexture != NULL)
        {
            //Get image dimensions
            m_Rect.w = m_Surface->w;
            m_Rect.h = m_Surface->h;
        }
    }

    m_ObjectTexture = newTexture;
    if (m_ObjectTexture != NULL)
    {
        return true;
    }

    return false;
}

// giải phóng các đối tượng
void BaseObject::Free()
{
    if (m_ObjectTexture != NULL)
    {
        SDL_DestroyTexture(m_ObjectTexture);
        m_ObjectTexture = NULL;
        m_Rect.w = 0;
        m_Rect.h = 0;
    }

    if (m_Surface != NULL)
    {
        SDL_FreeSurface(m_Surface);
        m_Surface = NULL;
    }
}

// Hiển thị ảnh lên màn hình
void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip /*=NULL*/)
{
    SDL_Rect renderQuad = { m_Rect.x, m_Rect.y, m_Rect.w, m_Rect.h };
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    INT iFlip = SDL_FLIP_NONE;
    if (m_Flip == true)
    {
        // nếu flip = true, thì xoay ảnh ngược lại
        iFlip = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(des, m_ObjectTexture, clip, &renderQuad, 0, NULL, (SDL_RendererFlip)iFlip);
}
