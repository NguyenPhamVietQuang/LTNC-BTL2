

#ifndef MUSIC_H_
#define MUSIC_H_

#include "g_define.h"
#include "TBase.h"

//////////////////////////////////////////////////////////////////////////
// Lớp quản lý việc tạo âm thanh cho game
//
//////////////////////////////////////////////////////////////////////////
static Mix_Music* g_music = NULL;
static Mix_Chunk* gChunkBullet = NULL;
static Mix_Chunk* gChunkMainDeath = NULL;
static Mix_Chunk* gChunkCoin = NULL;
static Mix_Chunk* gChunkGameOver = NULL;
static Mix_Chunk* gChunkPause = NULL;
static Mix_Chunk* gChunkBoom = NULL;
static Mix_Chunk* gChunkLaser = NULL;

static char gSMainBullet[] = { "sound\\iron_shot.wav"};
static char gSCoinUp[] = {"sound\\iron_coin.wav"};
static char gSGamePause[] = { "sound\\iron_pause.wav" };
static char gSMainDeath[] = { "sound\\iron_blood.wav" };
static char gSGameOver[] = {"sound\\iron_game_over.wav"};
static char gSExpBoom[] = { "sound\\iron_explo.wav" };
static char gSExpLaser[] = { "sound\\gun_laser.wav" };

class Music
{
public:
    Music();
    ~Music();

    // các loại âm thanh
    enum SoundType
    {
        BULLET_SOUND  = 0,
        EXP_SOUND,
        BLOOD_SOUND,
        COIN_INCREASING,
        GAMEOVER_SOUND,
        GAME_PAUSE,
        LASER_SOUND,
    };

    enum MusicState
    {
        MS_PLAY = 0,
        MS_STOP = 1,
        MS_PAUSE = 2,
    };

    static Music* GetInstance();
    bool Init();
    int PlaySoundGame(int soundType);
    int PlayMusic();
    void StopMusic();
    void PauseMusic();
    int GetMusicState() const { music_state_; };
    void SetMusicState(int state) { music_state_ = state; }

private:
    static Music* instance_;
    int music_state_;
};

#endif

