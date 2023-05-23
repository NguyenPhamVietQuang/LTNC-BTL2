
#include "stdafx.h"
#include <sstream> 
#include "TBase.h"


// Lớp thực hiện các hàm xử lý chung, phụ trợ cho game
int TBase::GetMax(const int& a, const int& b)
{
    return (a > b) ? a : b;
}

int TBase::GetMin(const int& a, const int& b)
{
    return (a < b) ? a : b;
}


std::string TBase::GetModulePathExe()
{
    std::string exePath;
    TCHAR chPath[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL)
    {
        GetModuleFileName(hModule, chPath, (sizeof(chPath)));

        std::wstring sTemp(&chPath[0]); //convert to wstring
        exePath = std::string(sTemp.begin(), sTemp.end());

        exePath = exePath.substr(0, exePath.find_last_of("\\/"));
    }

    return exePath;
}

bool TBase::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // Case 1: size object 1 < size object 2
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (top_a == top_b && bottom_a == bottom_b)
        {
            return true;
        }
    }

    // Case 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (top_a == top_b && bottom_a == bottom_b)
        {
            return true;
        }
    }

    // Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }

    return false;
}

bool TBase::CheckInsideMapX(UINT x1, UINT x2)
{
    int wNum = GameMap::GetInstance()->GetWNum();
    if (x1 >= 0 && x1 < wNum && x2 >= 0 && x2 < wNum)
    {
        return true;
    }
    return false;
}

bool TBase::CheckInsideMapY(UINT y1, UINT y2)
{
    int hNum = GameMap::GetInstance()->GetHNum();
    int wNum = GameMap::GetInstance()->GetWNum();
    if (y1 >= 0 && y1 < wNum && y2 >= 0 && y2 < hNum)
    {
        return true;
    }
    return false;
}

bool TBase::CheckInsideMap(UINT x, UINT y)
{
    int hNum = GameMap::GetInstance()->GetHNum();
    int wNum = GameMap::GetInstance()->GetWNum();
    if (x >= 0 && x < wNum && y >= 0 && y < hNum)
        return true;
    return false;
}

std::vector<std::string> TBase::SplitBySpace(const std::string& sData)
{
    std::vector<std::string> retList;
    std::stringstream ss(sData);
    std::string sRet;
    while (std::getline(ss, sRet, ' '))
    {
        retList.push_back(sRet);
    }

    return retList;
}

ColorData TBase::GetBaseColor(int type)
{
    ColorData color;
    if (type == RED_COLOR)
    {
        color = { 255, 0, 0 };
    }
    else if (type == WHITE_COLOR)
    {
        color = { 255, 255, 255 };
    }
    else if (type == GREEN_COLOR)
    {
        color = { 0, 255, 0 };
    }
    else if (type == BLUE_COLOR)
    {
        color = { 0, 0, 255 };
    }
    else if (type == ORANGE_COLOR)
    {
        color = { 20, 200, 100 };
    }
    else if (type == GRAY_COLOR)
    {
        color = { 36, 36, 36 };
    }
    else
    {
        color = { 0, 0, 0 };
    }

    return color;
}


void TBase::InsertSort(std::vector<int>& list)
{
    int length = list.size();
    int i, key, j;
    for (i = 1; i < length; i++)
    {
        key = list[i];
        j = i - 1;

        // Move elements of arr[0..i-1], 
        // that are greater than key, to one
        // position ahead of their
        // current position
        while (j >= 0 && list[j] > key)
        {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }


}