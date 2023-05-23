
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "g_define.h"

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();

    virtual bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

    void Free();
    void SetRect(const int& x, const int& y) { m_Rect.x = x, m_Rect.y = y; }; //hàm đặt ra 1 tấm ảnh với x,y độ dài và đọ rộng
    void SetIsFlip(const bool& flip) { m_Flip = flip; }
    bool GetFlip() const { return m_Flip; }
    SDL_Rect GetRect() const { return m_Rect; }//hàm hiển thị 1 tấm ảnh
    SDL_Texture* GetObject() { return m_ObjectTexture; }
protected:
    SDL_Texture* m_ObjectTexture; // Lưu dữ liệu đọc ảnh dạng texture
    SDL_Surface* m_Surface;       // dữ liệu đọc ảnh surface
    SDL_Rect m_Rect;              // vị trí hiển thị 1 tấm ảnh (x,y độ dài, độ rộng)
    bool m_Flip;                  // biến quản lý đổi ảnh soi gương trái phải (khi nhân vật quay trái quay phải, thì ảnh đổi lại)
};

#endif 

