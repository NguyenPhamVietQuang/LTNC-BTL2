
#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include "g_define.h"

#define K_EPXILON_2  0.01
#define K_EPXILON_3  0.001
#define K_EPXILON_4  0.0001
#define K_EPXILON_5  0.00001
#define K_EPXILON_6  0.000001

//////////////////////////////////////////////////////////////////////////
// Lớp vẽ hình học trong game
// sử dụng vẽ các khung hình trong game
//////////////////////////////////////////////////////////////////////////
class GeoFormat
{
public:
    GeoFormat() 
    { 
        left_ = 0;
        top_ = 0;
        width_ = 0;
        height_ = 0;
        color_ = ColorData(255, 255, 255);
    }

    GeoFormat(int left, int top, int width, int height, const ColorData& color)
    {
        left_ = left; 
        top_ = top;
        width_ = width;
        height_ = height;
        color_ = color;
    }

    int left_;
    int top_;
    int width_;
    int height_;
    ColorData color_;
};

class TPoint
{
public:
    TPoint();
    TPoint(int xx, int yy);
    TPoint(const TPoint& pt);
    ~TPoint();
    int x;
    int y;

    TPoint& operator =(const TPoint& pt)
    {
        this->x = pt.x;
        this->y = pt.y;
        return *this;
    }

    float GetDistance(const TPoint& pt);
};


class Gemometric
{
public:
   static void RenderRectange(const GeoFormat& geo_data, SDL_Renderer* screen);
   static void RenderOutline(const GeoFormat& geo_data,  SDL_Renderer* screen);
};


#endif