
#ifndef PLAYER_OBJECT_H_
#define PLAYER_OBJECT_H_

#include "TBase.h"
#include "g_define.h"
#include "BaseObject.h"
#include "MapData.h"
#include "GameMap.h"
#include "BulletObject.h"

//////////////////////////////////////////////////////////////////////////
// PlayerObject: Lơp nhân vật chơi game
// 
//////////////////////////////////////////////////////////////////////////

#define PLAYER_SPEED        6    // tốc độ di chuyển
#define PLAYER_HIGHT_VAL    18   // tốc độ nhảy của player
#define PLAYER_FRAMES       8    // số lượng khung hình


// các trạng thái của player
static char sPlayerMove[] = {"image\\iron_walk.bmp"};
static char sPlayerIdle[] = { "image\\iron_idle.bmp" };
static char sPlayerIdleShot[] = { "image\\iron_idle_shot.bmp" };
static char sPlayerRunShot[] = { "image\\iron_run_shot.bmp" };
static char sPlayerSoldIde[] = { "image\\sold_idle.bmp" };
static char sPlayerSoldWalk[] = { "image\\sold_walk.bmp" };

// kế thừa từ baseObject
class PlayerObject : public BaseObject
{
public:
    PlayerObject();
    ~PlayerObject();

    // trạng thái khi di chuyển hoặc đứng im
    // quay trái hay quay phải
    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };

    SDL_Rect GetRectFrame();
    BlockMap* GetBlockMap(int y, int x);
    BlockMap* GetBlockMap2(int y, int x);

    bool LoadImg(std::string path, SDL_Renderer* screen);
    bool get_is_death() const { return is_death_; }
    bool get_is_falling() const { return is_falling_; }

    int get_alive_time() const { return alive_time_; }
    int get_x_pos()       const { return x_pos_; }
    int get_y_pos()        const { return y_pos_; }
    int GetMoneyCount()    const { return m_CoinCount; };
    int get_frame_width()  const { return width_frame_; }
    int get_frame_height() const { return height_frame_; }

    void SetClip();
    void Show(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);

    void DoAction(SDL_Renderer* des);
    void CheckToMap(SDL_Renderer* des);
    void DoRight();
    void DoLeft();
    void FreeFalling();
    void DoJump();

    void HandleBullet(SDL_Renderer* des, bool is_pause = false);
    void RemoveBullet(const int& idx);
    void FreeBullet();
    void SetBullets(VT(BulletObject*) bullet_list);
    VT(BulletObject*) GetBullets() const;
    void CreateBullet(SDL_Renderer* screen);

    void setXYPos(const int& xp, const int& yp) { x_pos_ = xp; y_pos_ = yp; }
    void set_is_falling(const bool& is_falling) { is_falling_ = is_falling; }
    void set_is_death(const bool& is_death) { is_death_ = is_death;}

    void set_off_minus() { m_bMinusBlood = false; }
    bool get_minus() { return m_bMinusBlood; }
    void set_alive_time(const int& live_time) { alive_time_ = live_time; }
    void SetCoinCount(const int& mCoin) { m_CoinCount = mCoin; }
    void DoUpCoin();

    void ResetAlive();
    void ReStart();
    void SetOptionPlayer(int type) { m_PlayerOption = type; }
protected:
    void UpdateImagePlayer(SDL_Renderer* des);
private:
    // kỹ thuật độ trễ thời gian cho khung hình frame
    UINT m_DelayFrame[PLAYER_FRAMES];
    unsigned long passed_time_;

    // mảng khung frame
    SDL_Rect m_FrameClip[PLAYER_FRAMES];

    // quản lý trạng thái hành vi của player
    // đi trái, đi phải, nhảy.
    Input input_type_;

    // giá trị tăng khi di chuyển trái phải
    float x_val_;
    float y_val_;

    // vị trí của player trên màn hình
    float x_pos_;
    float y_pos_;

    // giá trị nhảy
    float y_val_jump_;

    // trạng thái nhảy hay không
    bool is_jump_;

    // trạng thái trên mặt đất hay không
    bool on_ground_;

    int m_count_jump;

    // trạng thái rơi xuống hố hay không
    bool is_falling_;

    // trạng thái chết hay ko
    bool is_death_;

    int frame_;

    // biến trạng thái quay trái quay phải
    int status_;

    // kích thước 1 frame
    int width_frame_;
    int height_frame_;

private:
    bool m_bAttack;  // đang tấn công hay ko
    VT(BulletObject*) m_BulletList;  // mảng lưu các viên đạn
    int m_CoinCount;  // số lượng tiền ăn được
    int alive_time_;  // thời gian chờ hồi sinh
    bool m_bMinusBlood;  // bị dính dạn, cờ này bật lên để báo giảm máu 1 đơn vị.
    int m_PlayerOption;
};


#endif