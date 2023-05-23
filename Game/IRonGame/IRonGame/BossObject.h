

#ifndef BOSS_OBJECT_H_
#define BOSS_OBJECT_H_

#include <vector>
#include "TBase.h"
#include "BulletObject.h"
#include "ThreatsObject.h"
#include "Music.h"

static char sBossAttak[] = { "image//boss_attack.bmp" };
static char sBossWalk[] = { "image//boss_walk.bmp" };
static char sBossShot[] = { "image//boss_shot.bmp" };

class BossObject : public ThreatsObject
{
public:

    // Trạng thái của boss
    enum BossState
    {
        BS_WALK = 400,  // di bộ trái phải
        BS_ATTACK = 401, // tấn công cận chiến (khi player đến sát)
        BS_SHOT = 402,  // bắn súng, khi player tiến gần
    };

    BossObject();
    ~BossObject();
    void Init(bool bClip, int xTile, int yTile, SDL_Renderer* screen);
    void Update(SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void DoAction();
    void CheckToMap();
    void DoLeft();
    void DoRight();
    void DoDown();
    void AddBullet(SDL_Renderer* screen);
    void DoBullet(SDL_Renderer* screen);
    void SetVisible(bool visible) { m_Visible = visible; }
    bool GetVisible() { return m_Visible; }
    void SetState(int state) { m_BossSate = state; }
    int GetState() { return m_BossSate; }
    void SetPosPlayer(SDL_Rect& rect_player) { m_PosPlayer = rect_player; }
    bool CheckCollisionBullet(SDL_Rect& rect);
    void MinusBlood();
    int GetBlood() { return m_BloodBos; }
    void FreeBullet();
private:
    int m_BloodBos; // máu của boss
    VT(BulletObject*) m_BulletList;  // danh sách đạn của boss
    bool m_Visible;  // cho phép ẩn hiện 
    int m_BossSate;  // trạng thái của bó
    SDL_Rect m_PosPlayer; // vị trí của nhân vật game
};


#endif