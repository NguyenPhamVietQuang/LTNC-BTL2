#pragma once

#include "g_define.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "Geometric.h"
#include "TextObject.h"
#include "PlayerBlood.h"
#include "PlayerParam.h"
#include "MenuGame.h"
#include "MarkInfo.h"

///////////////////////////////////////////////////////////////////////////
// GameMain là một lớp đối tượng chính đại diện cho 1 chương trình game
// Các thành phần, đối tượng của game sẽ là biến thành viên của GameMain
//////////////////////////////////////////////////////////////////////////////

class GameMain
{
public:
    GameMain();
    ~GameMain();

    // Trạng thái của một chương trình game
    enum GameState
    {
        G_PLAYING = 300,
        G_START = 301,
        G_PAUSE = 302,
        G_OVER = 303,
        G_END = 304,
    };

    // GameMain được định nghĩa theo mô hình design pattern: Singleton
    // Vì game chỉ có 1 chương trình duy nhất.
    // Do đó sử singleton để tạo ra một đối tượng gamemain duy nhất.
    static GameMain* GetInstance()
    {
        if (m_Instance == NULL)
        {
            m_Instance = new GameMain();
        }
        return m_Instance;
    }

    bool Init();
    bool InitData();
    void Loop();
    void Close();
    bool LoadBackground();
    void InitStartGame();
    void InitPauseGame();
    void InitEndGame();
private:
    static GameMain* m_Instance;
    int m_GameState;

    int m_MapX;    // vị trí mép trái màn hình so với bản đồ game(khi nhân vật di chuyển, màn hình bị cuốn chiếu)
    int m_MapY;    // vị trí mép trên màn hình so với bản đô game

    // Screen
    SDL_Window* m_Window;       // biến quản lý màn hình windows của game
    SDL_Renderer* m_Screen;     // biến đại diện cho màn hình chương trình game
    BaseObject  m_Background;   // Đối tượng ảnh background màn hình
    SDL_Event   m_Event;        // Biến quản lý sự kiện khi click chuọt
    TTF_Font*   m_Font;         // biến quản lý font chữ 
    TTF_Font*   m_Font50;       // biến quản lý font chữ size 50
    TTF_Font*   m_Font20;       // biến quản lý font chữ size 20

    PlayerObject m_player;      // Đối tượng game Player(người chơi chính)

    //Nhóm Biến thực hiện nhiệm vụ vẽ khung thông tin cho game
    GeoFormat m_Rect;
    GeoFormat m_RectOutLine;
    TextObject m_TTime;
    TextObject m_Title;

    PlayerBlood m_PlayerBlood; // Đối tượng máu(mạng) của nhân vật game
    PlayerCoin m_player_coin;  // Số lượng tiền, vàng ăn được

    // Khi bắn 1 chết 1 con quái vật, thì điểm số được tăng 1 và lưu và m_Mark
    unsigned int m_Mark;

    // m_MarkObj sử dụng m_Mark để hiển thị thông tin lên khung frame thông tin
    TextObject m_MarkObj;
    // menu
    MenuGameStart m_StartMenu; // thực hiện tạo màn hình start game
    MenuGamePause m_PauseMenu; // thực hiện tạo màn hình pause game, dùng phím space
    MenuGameEnd m_EndMenuOver; // màn hình endgame khi thua cuộc
    MenuGameEnd m_EndMenuWin;  // màn hình endgame khi chiến thắng

    MarkInfo m_MarkLog;
};

