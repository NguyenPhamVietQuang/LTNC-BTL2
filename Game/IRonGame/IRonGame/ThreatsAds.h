#pragma once
#include "TBase.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"


static INT gbZoombieList[] = { 8, 18, 30, 51, 65,
                               80, 133, 148, 162, 225, 300, 333, 360 };

static INT gbThreatShotList[] = { 36, 44, 71, 88,
                                100, 116, 168, 207, 239,
                                260, 288, 308, 342, 350, 375 };

static INT gbThreatFireList[] = {157, 191, 230, 270, 313, 335, 343, 365 };

static INT gbCrowListX[] = { 25, 100, 155};
static INT gbCrowListY[] = { 2, 3, 5 };

class ThreatsAds
{
public:
    ThreatsAds();
    ~ThreatsAds();

    static ThreatsAds* GetInstance()
    {
        if (instance_ == NULL)
            instance_ = new ThreatsAds();
        return instance_;
    }
    void DestroyInstance();

    bool CheckCollision(const SDL_Rect& rect_obj, const bool& isdel = true, bool skip_fire = false);
    bool CheckCollisionBulThreat(const SDL_Rect& rect_obj);
    bool GetBoolCol() const { return is_boom_cool_; }

    void BuildMonster(SDL_Renderer* screen);
    void BuildMonster1(SDL_Renderer* screen);

    void ReStart(SDL_Renderer* screen);

    void Render(SDL_Renderer* screen, bool is_pause = false);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void Free();
    void MakeGBMonster(SDL_Renderer* screen);
    void MakeGBThreatShot(SDL_Renderer* screen);
    void MakeGBFire(SDL_Renderer* screen);
private:
    static ThreatsAds* instance_;
    std::vector <ThreatsObject*> pThreatsNormal_;
    ExplosionObject* pEx_;
    bool is_boom_cool_;
};

