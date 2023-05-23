
#include "stdafx.h"
#include "Music.h"


Music* Music::instance_ = NULL;

Music::Music()
{
    music_state_ = MS_STOP;
}


Music::~Music()
{

}

Music* Music::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new Music();
    }

    return instance_;
}

bool Music::Init()
{
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }

    // load các file audio
    gChunkBullet    = Mix_LoadWAV(gSMainBullet);
    gChunkMainDeath = Mix_LoadWAV(gSMainDeath);
    gChunkCoin      = Mix_LoadWAV(gSCoinUp);
    gChunkGameOver  = Mix_LoadWAV(gSGameOver);
    gChunkPause     = Mix_LoadWAV(gSGamePause);
    gChunkBoom      = Mix_LoadWAV(gSExpBoom);
    gChunkLaser     = Mix_LoadWAV(gSExpLaser);

    // load nhạc nền
    g_music = Mix_LoadMUS("sound//backsound.mid");

    if (gChunkBullet == NULL || gChunkMainDeath == NULL || 
        gChunkCoin == NULL || gChunkPause == NULL || gChunkBoom == NULL ||
        gChunkLaser == NULL)
    {
        return false;
    }

     if (g_music == NULL)
     {
         return false;
     }

    return true;
}

// chạy âm thanh theo loại cần phát
int Music::PlaySoundGame(int soundType)
{
    if (soundType == EXP_SOUND)
    {
        Mix_PlayChannel(-1, gChunkBoom, 0);
    }
    if (soundType == BULLET_SOUND)
    {
        Mix_PlayChannel(-1, gChunkBullet, 0);
    }
    else if (soundType == BLOOD_SOUND)
    {
        Mix_PlayChannel(-1, gChunkMainDeath, 0);
    }
    else if (soundType == COIN_INCREASING)
    {
        Mix_PlayChannel(-1, gChunkCoin, 0);
    }
    
    else if (soundType == GAMEOVER_SOUND)
    {
        Mix_PlayChannel(-1, gChunkGameOver, 0);
    }
    
    else if (soundType == GAME_PAUSE)
    {
        Mix_PlayChannel(-1, gChunkPause, 0);
    }
    else if (soundType == LASER_SOUND)
    {
        Mix_PlayChannel(-1, gChunkLaser, 0);
    }
    return 0;
}

int Music::PlayMusic()
{
#if 1
    if (music_state_ == MS_STOP)
    {
        if (Mix_PlayMusic(g_music, -1) == -1)
        {
            return -1;
        }
        music_state_ = MS_PLAY;
    }
#endif
    return 0;
}

// có thể stop âm nhạc khi cần
void Music::StopMusic()
{
    if (music_state_ != MS_STOP)
    {
        Mix_HaltMusic();
        music_state_ = MS_STOP;
    }
}

// hoặc khi ấn pasue cần pasue âm thanh
void Music::PauseMusic()
{
    if (music_state_ != MS_PAUSE)
    {
        Mix_PauseMusic();
        music_state_ = MS_PAUSE;
    }
    else
    {
        Mix_ResumeMusic();
        music_state_ = MS_PLAY;
    }
}