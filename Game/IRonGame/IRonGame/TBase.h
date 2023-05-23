
//Copyright: TGoup

#ifndef TBASE_H_
#define TBASE_H_

#include "g_define.h"
#include "GameMap.h"
//#include "Geometric.h"

namespace TBase
{
    ColorData GetBaseColor(int type);
    std::string GetModulePathExe();
    int GetMax(const int& a, const int& b);
    int GetMin(const int& a, const int& b);
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    //bool CheckCollisionEx(const SDL_Rect& rt1, const SDL_Rect& rt2, TPoint& colPt);
    bool CheckInsideMapX(UINT x1, UINT x2);
    bool CheckInsideMapY(UINT y1, UINT y2);
    bool CheckInsideMap(UINT x, UINT y);
    std::vector<std::string> SplitBySpace(const std::string& sData);
    void InsertSort(std::vector<int>& list);
}

#endif