

#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "g_define.h"
#include "TBase.h"


class TextObject
{
public:
    TextObject();
    ~TextObject();

    enum TextColor
    {
        RED_TEXT = 0,
        WHITE_TEXT = 1,
        BLACK_TEXT = 2,
        GREEN_TEXT = 3,
        BLUE_TEXT = 4,
        ORANGE_TEXT = 5
    };

    std::string GetText() const { return str_val_; }
    bool LoadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen);

    void Free();
    void SetColor(Uint8 red, Uint8 green, Uint8 blue);
    void SetColor(int type);
    void SetText(const std::string& text) { str_val_ = text; }
    void SetPos(int x, int y) { text_pos_.x = x; text_pos_.y = y; }
    void RenderText(SDL_Renderer* screen, SDL_Rect* clip = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void SetRotation(float angle, SDL_Point pt) { angle_ = angle; pt_center_ = pt; }
    SDL_Rect GetRect() { return text_pos_; }
private:
    std::string str_val_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    SDL_Rect text_pos_;

    // Rotate
    float angle_;
    SDL_Point pt_center_;
};

#endif

