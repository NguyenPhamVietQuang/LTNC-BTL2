
#ifndef GOOMBAS_H_
#define GOOMBAS_H_

#include "TBase.h"
#include "ThreatsObject.h"
#include "Music.h"

////////////////////////////////////////////////////////////////////////////
// lớp monster kế thừa từ lớp theat cha
// theat hiểu là các mối hiểm họa nói chung
// monster là con của threat, thì hiểu đơn giản là quái vật được coi là hiểm họa
//////////////////////////////////////////////////////////////////////////

// có 3 loại monster được định nghĩa.
static char sThreadMoving[] = { "image//iron_moving.bmp" };  // loại di chuyển trong phạm vi
static char sThreadShot[] = { "image//iron_shot.bmp" };  // loại đứng yên và bắn
static char sThreadFire[] = { "image//iron_fire.bmp" };  // loại đứng yên (lửa, hầm chông...)

class GBMonster : public ThreatsObject
{
public:
    GBMonster();
    ~GBMonster();
    void Init(int type, bool bClip, int xTile, int yTile, SDL_Renderer* screen);
    void Update();
    void Show(SDL_Renderer* des);
    void SetIsType(int bType) { gb_type_ = bType; }
    int GetIsType() const { return gb_type_; }
    void DoAction();
    void CheckToMap();
    void DoLeft();
    void DoRight();
    void DoDown();
    void AddBullet(SDL_Renderer* screen);
    void DoBullet(SDL_Renderer* screen, bool is_pause = false);
    VT(BulletObject*) GetBullet() { return m_BulletList; }
    void FreeBullet();
    enum GMType
    {
        GB_MOVING = 0,
        GB_SHOT = 1,
        GB_FIRE = 2,
    };
private:
    int gb_type_; // loại monster
    VT(BulletObject*) m_BulletList;  // đạn cho moster bắn đạn
};

#endif
