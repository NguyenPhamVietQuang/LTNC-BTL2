

#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "g_define.h"
#include "TBase.h"
#include "BaseObject.h"



class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };

    bool Init(std::string path, SDL_Renderer* screen);
    void HandelMove(const int& x_border1, const int& x_border2);
    void set_x_Scope(const int& xScope) { x_scope_ = xScope; }
    void set_x_val(const int& x_val) { x_val_ = x_val; }
    void set_y_val(const int& y_val) { y_val_ = y_val; }
    void set_xy_pos(const int& xp, const int& yp) 
    { 
        x_pos_ = xp, y_pos_ = yp;
        UpdateRect();
    }

    void UpdateRect();
    int get_x_pos() const { return x_pos_; }
    int get_y_pos() const { return y_pos_; }
    int get_x_val() const { return x_val_; }
    int get_y_val() const { return y_val_; }
    void set_is_move(const bool& is_move) { is_move_ = is_move; }
    bool get_is_move()const { return is_move_; }
    void set_dir_bullet(const int& dir_type) { bullet_dir_ = dir_type; }
    void SetFlip(bool bFlip) { is_flip = bFlip; }
    bool CheckToMap();
    SDL_Rect GetExpPosition();
    BulletObject* Clone();
    void Show(SDL_Renderer* des);
private:
    int x_scope_;
    int x_up_;
    int x_pos_;
    int y_pos_;
    int x_val_;
    int y_val_;
    bool is_move_;
    int bullet_dir_;
    bool is_flip;
};

#endif