
#include "stdafx.h"
#include "MenuGame.h"
#include <algorithm>

MenuGame::MenuGame(bool is_bkgn /*= true*/)
{
	m_Type = 0;
	m_isDrawBkgn = is_bkgn;
    m_Select = -1;
}

MenuGame::~MenuGame()
{

}

void MenuGame::LoadBkgn(const char* file, SDL_Renderer* screen)
{
    if (m_isDrawBkgn == true)
    {
        m_Background.LoadImg(file, screen);
    }
}

void MenuGame::Render(SDL_Renderer* screen)
{
    if (m_isDrawBkgn == true)
    {
        m_Background.Render(screen);
    }
}

void MenuGame::MenuAction(SDL_Event events, SDL_Renderer* screen)
{

}

void MenuGame::Free()
{
    m_Background.Free();
}

bool MenuGame::OnRect(int x, int y, SDL_Rect& rect)
{
    if (rect.w > 0 && rect.h > 0)
    {
        if (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h)
        {
            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////

MenuGameStart::MenuGameStart()
{
    m_Select = -1;
    m_option = 0;
}

MenuGameStart::~MenuGameStart()
{
    FreeData();
}


void MenuGameStart::InitOption(SDL_Renderer* screen)
{
    m_Img1.LoadImg("image/img_1.png", screen);
    m_Img2.LoadImg("image/img_2.png", screen);

    m_Img1.SetRect(0, SCR_HEIGHT - 128);
    m_Img2.SetRect(100, SCR_HEIGHT - 128);


    m_optionBound.left_ = 1;
    m_optionBound.top_ = SCR_HEIGHT - 127;
    m_optionBound.width_ = 94;
    m_optionBound.height_ = SCR_HEIGHT - 1;
    m_optionBound.color_ = ColorData(255, 0, 0);
}

void MenuGameStart::SetImgOptionList(const VT(BaseObject*)& img_list)
{
    FreeData();
    m_ImgOptionList = img_list;
}

void MenuGameStart::FreeData()
{
    if (m_ImgOptionList.empty())
    {
        for (auto x : m_ImgOptionList)
        {
            if (x != NULL)
            {
                x->Free();
            }
        }
        m_ImgOptionList.clear();
    }
}

void MenuGameStart::Render(SDL_Renderer* screen)
{
    MenuGame::Render(screen);
    for (auto x : m_ImgOptionList)
    {
        x->Render(screen);
    }

    m_Img1.Render(screen);
    m_Img2.Render(screen);

    Gemometric::RenderOutline(m_optionBound, screen);
}

void MenuGameStart::MenuAction(SDL_Event events, SDL_Renderer* screen)
{

    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            m_optionBound.left_ = 101;
            m_optionBound.top_ = SCR_HEIGHT - 127;
            m_optionBound.width_ = 94;
            m_optionBound.height_ = SCR_HEIGHT - 1;
            m_optionBound.color_ = ColorData(255, 0, 0);
            m_option = 1;

            break;
        }
        case SDLK_LEFT:
        {
            m_optionBound.left_ = 1;
            m_optionBound.top_ = SCR_HEIGHT - 127;
            m_optionBound.width_ = 94;
            m_optionBound.height_ = SCR_HEIGHT - 1;
            m_optionBound.color_ = ColorData(255, 0, 0);
            m_option = 0;
            break;
        }
        }
    }


    switch (events.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                // khi bấm chuột vào vùng button, thì trả về index của button
                m_Select = i;
                break;
            }
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                 // không bấm chuột mà move chuột vào vùng button
                // thay đổi hình ảnh chữ trắng, sang chữ đổ tạo hiệu ứng trắng đỏ
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\start_red.bmp", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit_red.bmp", screen);
                }
            }
            else
            {
                // khi ra ngoài vùng button, thì lại load ảnh trắng
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\start.bmp", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit.bmp", screen);
                }
            }
        }
        break;
    }
    default: break;
    }
}

///////////////////////////////////////////////////////////////////////
MenuGamePause::MenuGamePause()
{
    m_Select = -1;
}

MenuGamePause::~MenuGamePause()
{
    FreeData();
}


void MenuGamePause::SetImgOptionList(const VT(BaseObject*)& img_list)
{
    FreeData();
    m_ImgOptionList = img_list;
}

void MenuGamePause::FreeData()
{
    if (m_ImgOptionList.empty())
    {
        for (auto x : m_ImgOptionList)
        {
            if (x != NULL)
            {
                x->Free();
            }
        }
        m_ImgOptionList.clear();
    }
}

void MenuGamePause::InitFrameGeo()
{
    int x1 = H_WIDTH - 120;
    int y1 = H_HEIGHT - 60;
    m_Rect.left_ = x1;
    m_Rect.top_ = y1;
    m_Rect.width_ = 200;
    m_Rect.height_ = 100;

    m_Rect.color_ = TBase::GetBaseColor(GRAY_COLOR);

    int x2 = m_Rect.left_ + 1;
    int y2 = m_Rect.top_ + 1;
    int w2 = m_Rect.width_ - 1;
    int h2 = m_Rect.height_ - 1;

    m_RectOutLine.left_ = x2;
    m_RectOutLine.top_ = y2;
    m_RectOutLine.width_ = w2;
    m_RectOutLine.height_ = h2;

    m_RectOutLine.color_ = TBase::GetBaseColor(WHITE_COLOR);

    int xPos = m_Rect.left_ + 70;
    int yPos = m_Rect.top_ + 10;

    for (size_t i = 0; i < m_ImgOptionList.size(); i++)
    {
        m_ImgOptionList[i]->SetRect(xPos, yPos);
        yPos += 50;
    }
}

void MenuGamePause::Render(SDL_Renderer* screen)
{
    Gemometric::RenderRectange(m_Rect, screen);
    Gemometric::RenderOutline(m_RectOutLine, screen);

    MenuGame::Render(screen);
    for (auto x : m_ImgOptionList)
    {
        x->Render(screen);
    }
}

void MenuGamePause::MenuAction(SDL_Event events, SDL_Renderer* screen)
{
    switch (events.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                m_Select = i;
                break;
            }
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                if (i == 0)
                {
                     m_ImgOptionList[i]->LoadImg("image\\resume_red.bmp", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit_red.bmp", screen);
                }
            }
            else
            {
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\resume.bmp", screen);
                }
                else
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit.bmp", screen);
                }
            }
        }
        break;
    }
    default: break;
    }
}

///////////////////////////////////////////////////////////////

MenuGameEnd::MenuGameEnd()
{
    m_Select = -1;
    m_Font = NULL;
    m_State = GA_NONE;
    m_MarkCurrent = 0;
}

MenuGameEnd::~MenuGameEnd()
{

}

void MenuGameEnd::SetImgOptionList(const VT(BaseObject*)& img_list)
{
    FreeData();
    m_ImgOptionList = img_list;
}


void MenuGameEnd::FreeData()
{
    if (m_ImgOptionList.empty())
    {
        for (auto x : m_ImgOptionList)
        {
            if (x != NULL)
            {
                x->Free();
            }
        }
        m_ImgOptionList.clear();
    }

    for (auto t : m_TextList)
    {
        t->Free();
    }
    m_TextList.clear();
}

void MenuGameEnd::MakeMarkText()
{
    int x_pos = 1000;
    int y_pos = 100;

    TBase::InsertSort(m_markList);

    bool bRed = false;
    for (int j = m_markList.size() - 1; j >= 0; j--)
    {
        TextObject* pText = new TextObject();
        int val = m_markList[j];

        if (val == m_MarkCurrent)
        {
            if (bRed == false)
            {
                pText->SetColor(TextObject::RED_TEXT);
                bRed = true;
            }
            else
            {
                pText->SetColor(TextObject::WHITE_TEXT);
            }
        }
        else
        {
            pText->SetColor(TextObject::WHITE_TEXT);
        }
        std::string sData = "Mark: ";
        sData += std::to_string(m_markList[j]);
        pText->SetText(sData);
        pText->SetPos(x_pos, y_pos);
        y_pos += 50;

        m_TextList.push_back(pText);
    }
}

void MenuGameEnd::RemoveTextList()
{
    for (int i = 0; i < m_TextList.size(); i++)
    {
        TextObject* pText = m_TextList[i];
        if (pText != NULL)
        {
            delete pText;
            pText = NULL;
        }
    }

    m_TextList.clear();
}

void MenuGameEnd::Render(SDL_Renderer* screen)
{
    MenuGame::Render(screen);
    for (auto x : m_TextList)
    {
        x->LoadFromRenderedText(m_Font, screen);
        x->RenderText(screen);
    }
      
    for (int i = 0; i < m_ImgOptionList.size(); i++)
    {
        BaseObject* pObj = m_ImgOptionList.at(i);
        if (pObj != NULL)
        {
            if (i == 1)
            {
                if (m_State == GA_WIN)
                {
                    pObj->Render(screen);
                }
                else if (m_State == GA_LOSE)
                {
                    ;// pObj->SetTextureIndex(2);
                    pObj->Render(screen);
                }
                else
                {
                    ;// pObj->SetTextureIndex(2);
                    pObj->Render(screen);
                }
            }
            else
            {
                pObj->Render(screen);
            }
        }
    }
}

void MenuGameEnd::MenuAction(SDL_Event events, SDL_Renderer* screen)
{
    switch (events.type)
    {
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                m_Select = i;
            }
        }
        break;
    }
    case SDL_MOUSEMOTION:
    {
        int x = events.button.x;
        int y = events.button.y;
        for (size_t i = 0; i < m_ImgOptionList.size(); ++i)
        {
            SDL_Rect item_rect = m_ImgOptionList[i]->GetRect();
            bool bRet = OnRect(x, y, item_rect);
            if (bRet)
            {
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit_red.bmp", screen);
                }
            }
            else
            {
                if (i == 0)
                {
                    m_ImgOptionList[i]->LoadImg("image\\exit.bmp", screen);
                }
            }
        }
    }
    default: break;
    }
}

void MenuGameEnd::SetTextContent(std::string sData, int idx)
{
    if (idx >= 0 && idx < (int)m_TextList.size())
    {
        TextObject* pText = m_TextList[idx];
        if (pText)
        {
            pText->SetText(sData);
        }
    }
}