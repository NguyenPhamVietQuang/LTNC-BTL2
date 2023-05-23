
#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "TBase.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include <vector>

#define WIDHT_THREAT 44;
#define HEIGHT_THREAT 50;

#define NUM_FRAME 8

//////////////////////////////////////////////////////////////////////////
// Lớp chung đại diện cho kẻ địch
//
//
//////////////////////////////////////////////////////////////////////////
class ThreatsObject : public BaseObject
{
public:
    ThreatsObject();
    ~ThreatsObject();

    enum TypeThreats
    {
        TH_UNDEF        = 0,
        TH_GB_MONSTER,
        TH_BOS,
    };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void set_x_val(int xp) { x_val_ = xp; }
    void set_y_val(int yp) { y_val_ = yp; }
    void set_xpos(const int& xps) { x_pos_ = xps; }
    void set_ypos(const int& yps) { y_pos_ = yps; }
    void set_clips();
    void set_is_alive(bool is_alive) { is_alive_ = is_alive; }
    void set_is_clip(bool isclip) { is_clip_ = isclip; }

    int get_x_pos() { return x_pos_; }
    int get_y_pos() { return y_pos_; }
    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }
    bool get_is_alive() { return is_alive_; }
    int GetType() const { return type_; }
    SDL_Rect GetRectFrame();
    //Overridde
public:
    virtual void Show(SDL_Renderer* des);
    virtual void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    virtual void DoAction();
protected:
    Map* pMap_;
    Input input_type_;
    SDL_Rect frame_clip_[NUM_FRAME];

    bool is_clip_;
    bool is_alive_;

    int on_ground_;
    int width_frame_;
    int height_frame_;
    int frame_;
    int v_dir_;
    int type_;

    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;

    float x_plus_;
    unsigned long passed_time_;
    unsigned int m_DelayTime[NUM_FRAME];
};

#endif //THREATS_OBJECT_H_