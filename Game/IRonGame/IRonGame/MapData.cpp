
#include "stdafx.h"
#include "MapData.h"

BlockMap::BlockMap()
{
    m_tile = NULL;
    m_type = "";

    x_index_ = 0;
    y_index_ = 0;
    xVal_ = 0;
    yVal_ = 0;
    xp_map_ = 0;
    yp_map_ = 0;
}

BlockMap::~BlockMap()
{
    if (m_tile != NULL)
    {
        delete m_tile;
        m_tile = NULL;
    }
}

void BlockMap::Update()
{
    SDL_Rect rect = m_tile->GetRect();
    if (yVal_ > 0)
    {
        rect.y -= yVal_;
        if (rect.y < yp_map_ - 64)
        {
            yVal_ = -yVal_;
        }
        m_tile->SetRect(xp_map_, rect.y);
    }
    else if (yVal_ < 0)
    {
        if (rect.y < yp_map_)
        {
            rect.y += abs(yVal_);
        }
        else
        {
            rect.y = yp_map_;
            yVal_ = 0;
        }
        m_tile->SetRect(xp_map_, rect.y);
    }
    else
    {
        m_tile->SetRect(xp_map_, yp_map_);
    }
}

void BlockMap::Render(SDL_Renderer* screen)
{
    if (m_tile && m_type != "")
    {
        m_tile->Render(screen);
    }
}

void BlockMap::RemoveTile()
{
    m_type = "";
    if (m_tile != NULL)
    {
        m_tile->Free();
        m_tile = NULL;
    }
}

void BlockMap::UpdateImage(SDL_Renderer* screen, std::string path)
{
    std::string sFile = path + std::string("\\") +  m_type + IMAGE_EXT;
    m_tile->LoadImg(sFile, screen);
}

//////////////////////////////////////////////////////////////////////////
Map::Map()
{

}

Map::~Map()
{
    RemoveList(0);
    RemoveList(1);
}

void Map::UpdateYMapInfo(int yp)
{
    // tuong tu nhu vay voi y
    if (max_y_ < SCR_HEIGHT)
    {
        // Todo
    }
    else
    {
        int sub = yp - start_y_;
        int d4 = (int)(SCR_HEIGHT*0.5);
        int d6 = (int)(SCR_HEIGHT*0.6);
        if (sub < d4)
        {
            start_y_ = start_y_ - (d4 - sub);
        }
        else if (sub > d6)
        {
            start_y_ = start_y_ + (sub - d6);
        }

        if (start_y_ + SCR_HEIGHT >= max_y_)
        {
            start_y_ = max_y_ - SCR_HEIGHT;
        }
        else if (start_y_ < 0)
        {
            start_y_ = 0;
        }
    }
}

void Map::UpdateXMapInfo(int xp)
{
    if (max_x_ < SCR_WIDTH)
    {
        //Todo
    }
    else
    {
        // Khi nhan vat di chuyen tu vi tri dau tien, den gan giua man hinh
        // thi man hinh chua can phai cuon chieu
        // Bat dau nhan vat vuot qua 1/2 man hinh, thi bat dau map dc cuon chieu
        // Ex: xp = 2 + screen_width/2, map bat dau cuon chieu 1 luong = 2
        int d5 = (int)SCR_WIDTH*0.5;
        int d2 = TILE_SIZE;

        int sub = xp - start_x_;
        if (sub > d5)
        {
            start_x_ = start_x_ + (sub - d5);
        }
        else if (sub < 64)
        {
            start_x_ = start_x_ - (TILE_SIZE - sub);
        }

        if (start_x_ + SCR_WIDTH >= max_x_)
        {
            // khi nhan vat di chuyen den gan cuoi ban do
            // cung la luc start_x se tang dan.
            // khi start_x + screen_width bat dau >= full map
            // thi luc nay map ko can cuon chieu nua
            // start_x dat gioi han = max_x - width man hinh
            start_x_ = max_x_ - SCR_WIDTH;
        }
        else if (start_x_ < 0)
        {
            start_x_ = 0;
        }
    }
}

void Map::RemoveList(int layer)
{
    if (layer == 0)
    {
        for (int i = 0; i < m_BlockList1.size(); i++)
        {
            VT(BlockMap*) temp = m_BlockList1[i];
            VT(BlockMap*)::iterator it;
            for (it = temp.begin(); it != temp.end(); it++)
            {
                delete (*it);
                *it = NULL;
            }

            temp.clear();
        }

        m_BlockList1.clear();
    }
    else
    {
        for (int i = 0; i < m_BlockList2.size(); i++)
        {
            VT(BlockMap*) temp = m_BlockList2[i];
            VT(BlockMap*)::iterator it;
            for (it = temp.begin(); it != temp.end(); it++)
            {
                delete (*it);
                *it = NULL;
            }

            temp.clear();
        }

        m_BlockList2.clear();
    }
}