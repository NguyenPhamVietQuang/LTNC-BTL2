

#include "stdafx.h"
#include "Geometric.h"

TPoint::TPoint()
{
    x = 0;
    y = 0;
}

TPoint::TPoint(int xx, int yy)
{
    x = xx;
    y = yy;
}

TPoint::TPoint(const TPoint& pt)
{
    x = pt.x;
    y = pt.y;
}

TPoint::~TPoint()
{

}

float TPoint::GetDistance(const TPoint& pt)
{
    int val1 = (pt.x - this->x)*(pt.x - this->x);
    int val2 = (pt.y - this->y)*(pt.y - this->y);
    float ret = sqrt(val1 + val2);
    return ret;
}

/////////////////////////////////////////////////////////////////////////

void Gemometric::RenderRectange(const GeoFormat& geo_data, SDL_Renderer* screen)
{
    ColorData color = geo_data.color_;

    SDL_Rect fillRect = { geo_data.left_, geo_data.top_, geo_data.width_, geo_data.height_};
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF);        
    SDL_RenderFillRect(screen, &fillRect );
}

void Gemometric::RenderOutline(const GeoFormat& geo_data, SDL_Renderer* screen)
{
    ColorData color = geo_data.color_;
    SDL_Rect outlineRect = { geo_data.left_, geo_data.top_, geo_data.width_, geo_data.height_};
    SDL_SetRenderDrawColor(screen, color.red_, color.green_, color.blue_, 0xFF);
    SDL_RenderDrawRect(screen, &outlineRect );
}