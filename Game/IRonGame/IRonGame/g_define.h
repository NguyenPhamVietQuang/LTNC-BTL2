
#ifndef G_DEFINE_H_
#define G_DEFINE_H_

#include <sdl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>"
#include <SDL_ttf.h>

#include <cmath>
#include <windows.h>
#include <vector>
#include <string>

#define SCR_WIDTH 1280
#define SCR_HEIGHT 640
#define TILE_SIZE 64
#define  RENDER_DRAW_COLOR 0X00

const int FRAMES_PER_SECOND = 30;

const int COLOR_KEY_R = 125;
const int COLOR_KEY_G = 125;
const int COLOR_KEY_B = 125;

#define EPXILON 2

#define VT(T) std::vector<T>

static const std::string IMAGE_EXT = ".bmp";



static const char TL_MAP_INFO[] = { "map_info" };
static const char TL_W_NUM[] = { "tl_w_num" };
static const char TL_H_NUM[] = { "tl_h_num" };
static const char TL_SIZE[] = { "tl_size" };
static const char TL_PATH[] = { "tl_path" };
static const char TL_LAY [] = { "tl_lay_num" };
static const char TL_DATA[] = { "data" };

static char g_MenuFont[] = { "font\\g_font.ttf" };
static char g_PlayerCoin[] = { "image\\player_coin.bmp" };

static char kImgBullet[] = { "image//iron_bullet.bmp" };
static char kImgBulletLaser[] = { "image//bul_laser.bmp" };

static char kImgBossFire[] = { "image//boss_fire.bmp" };

typedef struct Input
{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
} Input;

#define GRAVITY_SPEED       1.0
#define MAX_FALL_SPEED      15

const int FRAME_H = 50;

const int SCREEN_BPP = 32;
const int SPEED_SCREEN = 2;

const int H_WIDTH = (int)(0.5 * SCR_WIDTH);
const int H_HEIGHT = (int)(0.5 * SCR_HEIGHT);


enum GameStatus
{
    GAME_START = 100,
    GAME_PLAYING,
    GAME_PAUSE,
    GAME_END,
};

enum ColorBase
{
    RED_COLOR = 0,
    WHITE_COLOR = 1,
    BLACK_COLOR = 2,
    GREEN_COLOR = 3,
    BLUE_COLOR = 4,
    ORANGE_COLOR = 5,
    GRAY_COLOR = 6,
};

class ColorData
{
public:
    ColorData() { ; }
    ColorData(Uint8 r, Uint8 g, Uint8 b)
    {
        red_ = r;
        green_ = g;
        blue_ = b;
    }

    Uint8 red_;
    Uint8 green_;
    Uint8 blue_;

};
#endif 
