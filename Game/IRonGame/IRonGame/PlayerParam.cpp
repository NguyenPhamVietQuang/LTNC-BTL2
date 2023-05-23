#include "stdafx.h"
#include "PlayerParam.h"


//Hiển thị thông tin số tiền ăn được
PlayerCoin::PlayerCoin()
{
}

PlayerCoin::~PlayerCoin()
{

}

bool PlayerCoin::Init(SDL_Renderer* screen)
{
    m_Font = TTF_OpenFont(g_MenuFont, 30);

    int xText = SCR_WIDTH*0.25;
    int yText = 15;

    int xImg = SCR_WIDTH*0.25 - 50;
    int yImg = 8;

    MakeText();
    SetPos(xText, yText);
    SetRectImg(xImg, yImg);

    int bRet = LoadImg(g_PlayerCoin, screen);
    return bRet;
}

void PlayerCoin::MakeText()
{
    std::string sCount = std::to_string(m_count);
    sCount = "X " + sCount;

    SetText(sCount);
}