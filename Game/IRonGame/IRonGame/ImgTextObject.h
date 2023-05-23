
#ifndef IMG_TEXT_OBJ_H_
#define IMG_TEXT_OBJ_H_

#include "g_define.h"
#include "BaseObject.h"
#include "TextObject.h"

//////////////////////////////////////////////////////////////////////////
// ImgTextObject là lớp kế thừa từ BaseObject
// Nó có tính chất hiển thị một thông tin vừa có:
//  + Hình ảnh
//  + Text
//////////////////////////////////////////////////////////////////////////
class ImgTextObject : public BaseObject
{
public:
    ImgTextObject();
    ~ImgTextObject();

    void Show(SDL_Renderer* screen);
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void SetRectImg(int x, int y);
    void SetText(std::string text);
    void SetPos(int xp, int yp);
protected:
    TTF_Font* m_Font;
    TextObject m_Text;
};

#endif

