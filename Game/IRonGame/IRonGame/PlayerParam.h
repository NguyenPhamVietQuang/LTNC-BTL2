#ifndef PLAYER_PARAM
#define PLAYER_PARAM

#include "g_define.h"
#include "ImgTextObject.h"

class PlayerCoin : public ImgTextObject
{
public:
    PlayerCoin(void);
    ~PlayerCoin(void);
    bool Init(SDL_Renderer* screen);
    void MakeText();
    void SetCount(const int& count) { m_count = count; }
    int GetCount() const { return m_count; }
private:
    int m_count;
};

#endif

