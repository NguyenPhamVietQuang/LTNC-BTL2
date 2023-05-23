
#include "stdafx.h"
#include "BulletObject.h"

BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    x_scope_ = 0;
    x_up_ = 0;
    m_Flip = false;
    is_move_ = true;
}

BulletObject::~BulletObject()
{

}

BulletObject* BulletObject::Clone()
{
    BulletObject* p_object_clone = new BulletObject();
    p_object_clone->bullet_dir_ = this->bullet_dir_;
    p_object_clone->is_move_ = this->is_move_;
    p_object_clone->x_val_ = this->x_val_;
    p_object_clone->y_val_ = this->y_val_;

    return p_object_clone;
}

bool BulletObject::Init(std::string path, SDL_Renderer* screen)
{
    bool bRet = LoadImg(path, screen);
    return bRet;
}

void BulletObject::HandelMove(const int& x_border1, const int& x_border2)
{
    Map* map_data = GameMap::GetInstance()->GetMap();

    if (bullet_dir_ == DIR_RIGHT)
    {
        x_pos_ += x_val_;
        if (x_pos_ > x_border2)
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == DIR_LEFT)
    {
        x_pos_ -= x_val_;
        if (x_pos_ < x_border1)
        {
            is_move_ = false;
        }
    }
}

bool BulletObject::CheckToMap()
{
    Map* map_data = GameMap::GetInstance()->GetMap();
    SDL_Rect blRect = GetRect();
    blRect.x = x_pos_ - map_data->getStartX();
    blRect.y = y_pos_ - map_data->getStartY();

    int x = (x_pos_) / TILE_SIZE;
    int y = (y_pos_) / TILE_SIZE;

    bool IsInside = true;
    IsInside = TBase::CheckInsideMapX(x, x);
    IsInside &= TBase::CheckInsideMapY(y, y);

    if (IsInside)
    {
        BlockMap* pBlock = map_data->GetTile1().at(y).at(x);
        if (pBlock->GetTile() != NULL)
        {
            std::string val = pBlock->getType();
            bool bSkip = GameMap::GetInstance()->CheckSkipMap(val);
            if (bSkip == false)
            {
                if (bullet_dir_ == DIR_LEFT)
                {
                    x_pos_ = (x + 1) * TILE_SIZE;
                }
                else if (bullet_dir_ == DIR_RIGHT)
                {
                    x_pos_ = x * TILE_SIZE;
                }

                is_move_ = false;
                m_Rect.x = x_pos_ - map_data->getStartX();
                return true;
            }
        }
    }

    return false;
}


SDL_Rect BulletObject::GetExpPosition()
{
    SDL_Rect rectExp = m_Rect;
    rectExp.y += m_Rect.h*0.5;

    return rectExp;
}

void BulletObject::Show(SDL_Renderer* des)
{
    if (bullet_dir_ == DIR_LEFT)
    {
        m_Flip = true;
    }

    Map* map_data = GameMap::GetInstance()->GetMap();
    m_Rect.x = x_pos_ - map_data->getStartX();
    m_Rect.y = y_pos_ - map_data->getStartY();
    BaseObject::Render(des);
}

void BulletObject::UpdateRect()
{
    Map* map_data = GameMap::GetInstance()->GetMap();
    // sau khi set lai x_pos va y_pos, can cap nhat lai vi tri cua m_rect
    // neu ko m_rect van la gia tri cua thoi diem trc do
    m_Rect.x = x_pos_ - map_data->getStartX();
    m_Rect.y = y_pos_ - map_data->getStartY();
}