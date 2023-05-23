
#pragma once

#include "BaseObject.h"
#include "g_define.h"

//////////////////////////////////////////////////////////////////////////
// Lớp quản lý vụ nổ
//////////////////////////////////////////////////////////////////////////
class ExplosionObject : public BaseObject
{
public:
    ExplosionObject();
    ~ExplosionObject();
    ExplosionObject(int frameNum);
public:
    virtual bool LoadImg(std::string path, SDL_Renderer* screen);

    int get_frame() const { return frame_; }
    int get_frame_width() const { return frame_width_; }
    int get_frame_height() const { return frame_height_; }

    void set_clips();
    void set_frame(const int& fr) { frame_ = fr; }
    void SetXP(SDL_Rect rect_pos);
    void ImpRender(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void SetActive(bool bActive) { m_isActive = bActive; }
    bool GetActive() const { return m_isActive; }
private:
    int m_Frame;  // số lượng frame cho vụ nổ (4)
    VT(SDL_Rect) frame_clip_;  // mảng chứa thông tin frame
    int frame_width_;   // chiều rộng 1 frame
    int frame_height_;  // chiều cao 1 frame
    unsigned int frame_; // frame hiện tại khi xử lý 
    bool m_isActive;     // frame hoạt động hay ngưng
};

//////////////////////////////////////////////////////////////////////////
// Lớp lưu danh sách các vụ nổ được tạo ra
// khi va chạm player, đạn, theat, boss, các vụ nổ được tạo ra, nhưng ko hiển thị ngay
// vì chúng đang nằm trong vòng lặp của đan, boss, player
// do đó chung cần lưu tạm vào một nơi.
//////////////////////////////////////////////////////////////////////////
class ExpAds
{
public:
    ExpAds();
    ~ExpAds();

    static ExpAds* GetInstance()
    {
        if (instance_ == NULL)
            instance_ = new ExpAds();
        return instance_;
    }

    void Render(SDL_Renderer* screen);
    void Add(ExplosionObject* pObj);
    bool IsEmpty();
    void ResetExp() { m_ExpList.clear(); }
private:
    std::vector<ExplosionObject*> m_ExpList;  // mảng vector lưu danh sách các vụ nổ
    static ExpAds* instance_;
};
