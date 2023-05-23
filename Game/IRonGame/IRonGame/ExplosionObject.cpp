#include "stdafx.h"
#include "ExplosionObject.h"
#include "Music.h"

ExplosionObject::ExplosionObject()
{
    m_isActive = true;
    frame_ = -1;
    m_Frame = 4;
    frame_width_ = 0;
    frame_height_ = 0;

    for (int i = 0; i < m_Frame; i++)
    {
        SDL_Rect rt = { 0, 0, 0, 0 };
        frame_clip_.push_back(rt);
    }
}

ExplosionObject::ExplosionObject(int frameNum)
{
    m_isActive = true;
    frame_ = -1;
    m_Frame = frameNum;
    frame_width_ = 0;
    frame_height_ = 0;

    for (int i = 0; i < m_Frame; i++)
    {
        SDL_Rect rt = { 0, 0, 0, 0 };
        frame_clip_.push_back(rt);
    }
}



ExplosionObject::~ExplosionObject()
{

}

//////////////////////////////////////////////////////////////////////////
// load ảnh vụ nổ
//////////////////////////////////////////////////////////////////////////
bool ExplosionObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        // tính toán kích thước 1 frame đơn
        // ví dụ 240/4 = 60
        frame_width_ = m_Rect.w / m_Frame;
        frame_height_ = m_Rect.h;
    }

    // xử lý xây dựng mảng clip thông tin cho từng frame
    set_clips();
    return ret;
}

void ExplosionObject::set_clips()
{
    if (frame_width_ > 0 && frame_height_ > 0)
    {
        // với 4 frame cho 1 tấm ảnh dài ví dụ 240x60
        for (int i = 0; i < m_Frame; i++)
        {
            // frame 0: 0,0,60,60
            // frame 1: 60,0, 60, 60...
            frame_clip_[i].x = frame_width_*i;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = frame_width_;
            frame_clip_[i].h = frame_height_;
        }
    }
}

// hiển thị vụ nổ cho 1 frame
void ExplosionObject::Show(SDL_Renderer* screen)
{
    SDL_Rect* currentClip = &frame_clip_[frame_];
    BaseObject::Render(screen, currentClip);
}

// tính toán vị trí xuất hiện của frame
void ExplosionObject::SetXP(SDL_Rect rect_pos)
{
    int x_pos = (int)(rect_pos.x - frame_width_*0.5);
    int y_pos = (int)(rect_pos.y - frame_height_*0.5);

    SetRect(x_pos, y_pos);
}

void ExplosionObject::ImpRender(SDL_Renderer* screen)
{
    // render toàn bộ frame, và frame sẽ chạy lần lượt từ 0 - numFame
    frame_++;
    if ((int)frame_ >= m_Frame)
    {
        // khi frame chạy = m_Frame
        // thì vụ nổ kết thúc, nó sẽ ko hiển thị nữa. và biến active sẽ quản lý vấn đề đó

        frame_ = 0;
        m_isActive = false;
    }

    if (m_isActive)
    {
        Show(screen);
    }
}

///////////////////////////////////////////////////////////////////////////////
ExpAds* ExpAds::instance_ = NULL;
ExpAds::ExpAds()
{

}

ExpAds::~ExpAds()
{

}

// Hiển thị danh sách toàn bộ các vụ nổ
void ExpAds::Render(SDL_Renderer* screen)
{
    for (size_t i = 0; i < m_ExpList.size(); i++)
    {
        ExplosionObject* pObj = m_ExpList.at(i);
        if (pObj != NULL)
        {
            pObj->ImpRender(screen);
        }
    }

    for (size_t i = 0; i < m_ExpList.size(); i++)
    {
        ExplosionObject* pObj = m_ExpList.at(i);
        if (pObj != NULL)
        {
            if (pObj->GetActive() == false)
            {
                // khi một vụ nổ kết thúc, thông tin về nó sẽ được loại khỏi danh sách
                pObj->Free();
                m_ExpList.erase(m_ExpList.begin() + i);
                i--;
            }
        }
    }
}

// lưu thêm vụ nổ vào danh sách
void ExpAds::Add(ExplosionObject* pObj)
{
    Music::GetInstance()->PlaySoundGame(Music::EXP_SOUND);
    if (pObj != NULL)
    {
        m_ExpList.push_back(pObj);
    }
}

bool ExpAds::IsEmpty()
{
    return m_ExpList.empty();
}