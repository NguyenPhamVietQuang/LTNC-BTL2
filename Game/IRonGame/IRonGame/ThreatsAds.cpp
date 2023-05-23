
#include "stdafx.h"
#include "ThreatsAds.h"
#include "GBMonster.h"
#include "Geometric.h"

#define NO_BOOM 

//////////////////////////////////////////////////////////////////////////
// Lớp quản lý toàn bộ danh sách các đối tượng kẻ địch
//
//////////////////////////////////////////////////////////////////////////

ThreatsAds* ThreatsAds::instance_ = NULL;

ThreatsAds::ThreatsAds()
{
    is_boom_cool_ = false;
}


ThreatsAds::~ThreatsAds()
{

}

void ThreatsAds::DestroyInstance()
{
    Free();
    if (instance_ != NULL)
    {
        delete instance_;
        instance_ = 0;
    }
}

void ThreatsAds::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    for (int i = 0; i < pThreatsNormal_.size(); i++)
    {
        ThreatsObject* obj_threat = pThreatsNormal_.at(i);
        if (obj_threat != NULL)
        {
            obj_threat->HandleInputAction(events, screen);
        }
    }
}

void ThreatsAds::ReStart(SDL_Renderer* screen)
{
    Free();
    BuildMonster(screen);
}

// Hàm xây dựng danh sách kẻ địch
void ThreatsAds::BuildMonster(SDL_Renderer* screen)
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap != NULL)
    {
        // Tạo danh sách monster
        BuildMonster1(screen);
    }
}

void ThreatsAds::BuildMonster1(SDL_Renderer* screen)
{
    MakeGBMonster(screen); // loại monster di chuyển
    MakeGBThreatShot(screen); // loại monster bắn đạn
    MakeGBFire(screen);  // loại lửa
}

void ThreatsAds::MakeGBMonster(SDL_Renderer* screen)
{
    UINT gbNum = sizeof(gbZoombieList) / sizeof(gbZoombieList[0]);
    if (gbNum > 0)
    {
        GBMonster* pGBList = new GBMonster[gbNum];
        for (int i = 0; i < gbNum; i++)
        {
            GBMonster* pGBMonster = (pGBList + i);
            if (pGBMonster != NULL)
            {
                pGBMonster->Init(GBMonster::GB_MOVING, true, gbZoombieList[i], 2, screen);
                pThreatsNormal_.push_back(pGBMonster);
            }
        }
    }
}

void ThreatsAds::MakeGBThreatShot(SDL_Renderer* screen)
{
    UINT gbNum = sizeof(gbThreatShotList) / sizeof(gbThreatShotList[0]);
    if (gbNum > 0)
    {
        GBMonster* pGBList = new GBMonster[gbNum];
        for (int i = 0; i < gbNum; i++)
        {
            GBMonster* pGBThreatShot = (pGBList + i);
            if (pGBThreatShot != NULL)
            {
                pGBThreatShot->Init(GBMonster::GB_SHOT, true, gbThreatShotList[i], 3, screen);
                pGBThreatShot->AddBullet(screen);
                pThreatsNormal_.push_back(pGBThreatShot);
            }
        }
    }
}

void ThreatsAds::MakeGBFire(SDL_Renderer* screen)
{
    UINT gbNum = sizeof(gbThreatFireList) / sizeof(gbThreatFireList[0]);
    if (gbNum > 0)
    {
        GBMonster* pGBList = new GBMonster[gbNum];
        for (int i = 0; i < gbNum; i++)
        {
            GBMonster* pGBFire= (pGBList + i);
            if (pGBFire != NULL)
            {
                pGBFire->Init(GBMonster::GB_FIRE, true, gbThreatFireList[i], 3, screen);
                pThreatsNormal_.push_back(pGBFire);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Hiển thị monster lên màn hình
//
//////////////////////////////////////////////////////////////////////////
void ThreatsAds::Render(SDL_Renderer* screen, bool is_pause /*false*/)
{
    for (int i = 0; i < pThreatsNormal_.size(); i++)
    {
        ThreatsObject* obj_threat = pThreatsNormal_.at(i);
        if (obj_threat->GetType() == ThreatsObject::TH_GB_MONSTER)
        {
            GBMonster* pObject = static_cast<GBMonster*>(obj_threat);
            if (pObject->get_is_alive() == true)
            {
                // Bắn đạn cho monster
                pObject->DoBullet(screen, is_pause);

                if (is_pause == false)
                {
                    // di chuyển trên bản đồ cho monster
                    pObject->DoAction();
                }
                pObject->Show(screen);
            }
            else
            {
                // xóa monster khi bị giết
                obj_threat->Free();
                pThreatsNormal_.erase(pThreatsNormal_.begin() + i);
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
//   xử lý va chạm giữa player và kẻ địch
//   xử lý va chạm đạn player và kẻ địch
//////////////////////////////////////////////////////////////////////////

bool ThreatsAds::CheckCollision(const SDL_Rect& rect_object, const bool& isDel /*true*/, bool skip_fire /*= false*/)
{
    bool bRet = false;
    for (int i = 0; i < pThreatsNormal_.size(); i++)
    {
        ThreatsObject* pThreat = pThreatsNormal_.at(i);
        if (pThreat != NULL)
        {
            if (skip_fire == true) // thường khi va chạm với đạn player
            {
                if (pThreat->GetType() == ThreatsObject::TH_GB_MONSTER)
                {
                    GBMonster* pMonster = (GBMonster*)pThreat;
                    if (pMonster != NULL)
                    {
                        // chỉ xử lý va chạm với loại không phải lửa
                        if (pMonster->GetIsType() != GBMonster::GB_FIRE)
                        {
                            SDL_Rect rect = pThreat->GetRectFrame();
                            TPoint pCol;
                            bool bColRect = TBase::CheckCollision(rect_object, rect);
                            if (bColRect)
                            {
                                bRet = true;
                                if (isDel == true)
                                {
                                    // cho phép xóa monster khi bị giết
                                    pThreatsNormal_.erase(pThreatsNormal_.begin() + i);
                                }
                            }
                        }

                    }
                }
            }
            else
            {
                // va chạm với chính player
                SDL_Rect rect = pThreat->GetRectFrame();
                TPoint pCol;
                bool bColRect = TBase::CheckCollision(rect_object, rect);
                if (bColRect)
                {
                    bRet = true;
                    if (isDel == true)
                    {
                        pThreatsNormal_.erase(pThreatsNormal_.begin() + i);
                    }
                }
            }
        }
    }

    if (bRet == true)
    {
        return bRet;
    }
    // Check Collision
    return bRet;
}

//////////////////////////////////////////////////////////////////////////
// Check va chạm giữa đạn kẻ địch và player
//
//////////////////////////////////////////////////////////////////////////
bool ThreatsAds::CheckCollisionBulThreat(const SDL_Rect& rect_object)
{
    bool bFindCol = false;
    for (int i = 0; i < pThreatsNormal_.size(); i++)
    {
        ThreatsObject* pThreat = pThreatsNormal_.at(i);
        if (pThreat)
        {
            if (pThreat->GetType() == ThreatsObject::TH_GB_MONSTER)
            {
                GBMonster* pMonster = (GBMonster*)pThreat;
                if (pMonster != NULL)
                {
                    if (pMonster->GetIsType() == GBMonster::GB_SHOT)
                    {
                        std::vector<BulletObject*> bullet_list = pMonster->GetBullet();
                        for (int bul = 0; bul < bullet_list.size(); bul++)
                        {
                            BulletObject* pBull = bullet_list[bul];
                            if (pBull != NULL)
                            {
                                bool bCol = TBase::CheckCollision(rect_object, pBull->GetRect());
                                if (bCol == true)
                                {
                                    bFindCol = true;
                                    pBull->set_is_move(false);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (bFindCol == true)
        {
            break;
        }
    }

    return bFindCol;
}

// giải phòng toàn bộ layer khi stop game
void ThreatsAds::Free()
{
    for (int i = 0; i < pThreatsNormal_.size(); i++)
    {
        ThreatsObject* pObj = pThreatsNormal_[i];
        pObj->Free();
        pThreatsNormal_.erase(pThreatsNormal_.begin() + i);
    }

    pThreatsNormal_.clear();
}