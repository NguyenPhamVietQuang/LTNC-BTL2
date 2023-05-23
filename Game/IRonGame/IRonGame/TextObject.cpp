
#include "stdafx.h"
#include "TextObject.h"

////////////////////////////////////////////////////////////////////////////
// Lớp dại diện vẽ đối tượng text trên game
//
//////////////////////////////////////////////////////////////////////////
TextObject::TextObject()
{
  text_color_.r = 255;
  text_color_.g = 255;
  text_color_.b = 255;
  texture_ = NULL;
}


TextObject::~TextObject()
{
    Free();
}


bool TextObject::LoadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen)
{
    //Render text surface
    Free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, str_val_.c_str(), text_color_);
    if (textSurface != NULL)
    {
        //Create texture from surface pixels
        texture_ = SDL_CreateTextureFromSurface(screen, textSurface);
        if (texture_ != NULL)
        {
            //Get image dimensions
            text_pos_.w = textSurface->w;
            text_pos_.h = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return texture_ != NULL;
}

void TextObject::Free()
{
    if (texture_ != NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}

void TextObject::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void TextObject::SetColor(int type)
{
    SDL_Color color;
    if (type == RED_TEXT)
    {
        color = { 255, 0, 0 };
    }
    else if (type == WHITE_TEXT)
    {
        color = { 255, 255, 255 };
    }
    else if (type == GREEN_TEXT)
    {
        color = { 0, 255, 0 };
    }
    else if (type == BLUE_TEXT)
    {
        color = { 0, 0, 255 };
    }
    else if (type == ORANGE_TEXT)
    {
        color = { 20, 200, 100 };
    }
    else
    {
        color = { 0, 0, 0 };
    }

    text_color_ = color;
}

void TextObject::RenderText(SDL_Renderer* screen,  SDL_Rect* clip /* = NULL */, SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
{
    SDL_Rect renderQuad = text_pos_;
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle_, &pt_center_, flip);
}
