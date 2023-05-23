#include "stdafx.h"
#include "GameMain.h"
#include "ImpTimer.h"
#include "GameMap.h"
#include "Music.h"
#include "ExplosionObject.h"
#include "ThreatsAds.h"
#include "BossObject.h"

GameMain* GameMain::m_Instance = NULL;

GameMain::GameMain()
{
    m_GameState = GameStatus::GAME_START;
}


GameMain::~GameMain()
{
}

//////////////////////////////////////////////////////////////////////////
// Khởi tạo môi trường lập trình game sdl
//////////////////////////////////////////////////////////////////////////
bool GameMain::Init()
{
    bool success = true;

    // Khởi tạo cơ bản
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
    {
        return success;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    //Tạo windows
    m_Window = SDL_CreateWindow("IRon-Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    if (m_Window == NULL)
    {
        success = false;
    }
    else
    {
        // Tạo đối tượng m_screen đại diện cho màn hình game
        m_Screen = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
        if (m_Screen == NULL)
        {
            success = false;
        }
        else
        {
            // xây dựng màn hình cơ bản với màu đen.
            SDL_SetRenderDrawColor(m_Screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

            // khởi tạo chế độ load ảnh PNG
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
            {
                success = false;
            }
        }

        // Khởi tạo chế độ load font, chữ.
        if (TTF_Init() == -1)
        {
            success = false;
        }

        // Khởi tạo chế độ load âm thanh
        if (Music::GetInstance()->Init() == false)
        {
            success = false;
        }

        // Đọc font chữ từ file font có sẵn
        m_Font20 = TTF_OpenFont(g_MenuFont, 20);
        m_Font = TTF_OpenFont(g_MenuFont, 30);
        m_Font50 = TTF_OpenFont(g_MenuFont, 50);
        if (m_Font == NULL || m_Font50 == NULL  || m_Font20 == NULL)
        {
            return false;
        }
    }

    return success;
}

//////////////////////////////////////////////////////////////////////////
// Khởi tạo thông số cơ bản cho các đối tượng game
//////////////////////////////////////////////////////////////////////////
bool GameMain::InitData()
{
    // Load hình ảnh background
    bool ret = LoadBackground();
    if (ret == false)
        return false;

    // Load bản đồ game
    GameMap* game_map = GameMap::GetInstance();
    std::string path = "image\\map";
    game_map->SetPath(path);
    // đọc dữ liệu từ file map.tmp
    game_map->LoadMap();

    // Sử dụng bản đồ 2 lơp layer, nên đọc 2 lần
    // 1 layer sử dụng cho map bản đồ di chuyển
    game_map->LoadMapTiles(m_Screen, 0);

    // 1 layer sử dụng cho các đối tượng như tiền vàng, kim cương
    game_map->LoadMapTiles(m_Screen, 1);

    // tính toán tổng chiều dài, chiều rộng của của map
    // map có 400 tile ngang và 10 tile dọc
    m_MapY = game_map->GetMap()->getMaxY();  // =400*64 = 25600
    m_MapX = game_map->GetMap()->getMaxX();  // = 10*64 = 640

    // Load đối tượng ảnh ở vị trí 100, 100
    m_player.LoadImg(sPlayerIdle, m_Screen);
    m_player.setXYPos(100, 100);


    // xay dựng khung frame thông tin trên đỉnh màn hình game
    // Khung có vị trí tứ 0 đến độ dài màn hình screen_width (ko phải toàn bộ chiều dài bản đồ)
    m_Rect.left_ = 0;
    m_Rect.top_ = 0;
    m_Rect.width_ = min(SCR_WIDTH, m_MapX);
    m_Rect.height_ = FRAME_H;
    m_Rect.color_ = ColorData(36, 36, 36);

    // Khung viền màu trắng lùi 1 đơn vị so với khung xám
    m_RectOutLine.left_ = 1;
    m_RectOutLine.top_ = 1;
    m_RectOutLine.width_ = m_Rect.width_ - 1;
    m_RectOutLine.height_ = FRAME_H - 2;
    m_RectOutLine.color_ = ColorData(255, 255, 255);

    // khởi tạo màu cho đối tượng quản lý hiển thị thời gian game
    m_TTime.SetColor(TextObject::WHITE_TEXT);

    // khởi tạo màu cho đối tượng quản lý hiển thị tên game
    m_Title.SetColor(TextObject::WHITE_TEXT);
    std::string str_val = "G-IRON";
    m_Title.SetText(str_val);
    m_Title.LoadFromRenderedText(m_Font50, m_Screen);
    int xPos = SCR_WIDTH*0.5;
    m_Title.SetPos(xPos - 50, 2);


    // khởi tạo màu cho đối tượng quản lý hiển điểm của game
    m_MarkObj.SetColor(TextObject::WHITE_TEXT);
    m_MarkObj.SetPos(SCR_WIDTH*0.5 + 200, 15);

    // load ảnh và khởi tạo máu của player
    m_PlayerBlood.Init(m_Screen);

    // load ảnh và khởi tạo số tiền ăn được của player
    m_player_coin.Init(m_Screen);

    // Khởi tạo các màn hình start, pause, endgame
    InitStartGame();
    InitPauseGame();
    InitEndGame();

    m_MarkLog.LoadMark();
    return true;
}

bool GameMain::LoadBackground()
{
    std::string path = "image\\bkgn.bmp";
    bool ret = m_Background.LoadImg(path, m_Screen);
    if (ret == false)
    {
        return false;
    }

    return true;
}

void GameMain::InitStartGame()
{
    // load ảnh background cho màn hình start game
    m_StartMenu.LoadBkgn("image\\iron_start_bkgn.bmp", m_Screen);

    // Màn hình start game có 2 button player và exist
    VT(BaseObject*) menu_list;
    BaseObject* playObj = new BaseObject();
    BaseObject* ExitObj = new BaseObject();

    int x_pos = H_WIDTH - 200;
    int y_pos = H_HEIGHT + 180;

    bool b1a = playObj->LoadImg("image\\start.bmp", m_Screen);
    if (b1a)
    {
        playObj->SetRect(x_pos, y_pos);
        menu_list.push_back(playObj);
    }

    bool b3a = ExitObj->LoadImg("image\\exit.bmp", m_Screen);
    if (b3a)
    {
        y_pos += 50;
        ExitObj->SetRect(x_pos, y_pos);
        menu_list.push_back(ExitObj);
    }

    m_StartMenu.SetImgOptionList(menu_list);
    m_StartMenu.InitOption(m_Screen);
}


void GameMain::InitPauseGame()
{
    // Màn hình pause game không sử dụng background
    // Pause game sử dụng một khung frame nhỏ
    m_PauseMenu.SetIsBkgn(false);
    VT(BaseObject*) pause_list;

    // pause game có 2 button reusume và quit
    BaseObject* resumeObj = new BaseObject();
    BaseObject* quitObj = new BaseObject();

    bool ret1 = resumeObj->LoadImg("image\\resume.bmp", m_Screen);
    if (ret1)
    {
        pause_list.push_back(resumeObj);
    }

    bool ret3 = quitObj->LoadImg("image\\exit.bmp", m_Screen);
    if (ret3)
    {
        pause_list.push_back(quitObj);
    }

    m_PauseMenu.SetImgOptionList(pause_list);
    m_PauseMenu.InitFrameGeo();
}

void GameMain::InitEndGame()
{
    // Màn hình engame có 2 loai: over và win
    m_EndMenuOver.LoadBkgn("image\\game_over.bmp", m_Screen);
    m_EndMenuWin.LoadBkgn("image\\game_win.bmp", m_Screen);

    VT(BaseObject*) menu_list;
    BaseObject* ExitObj = new BaseObject();

    // Chỉ cần 1 button exit cho endgame
    int x_pos = H_WIDTH - 200;
    int y_pos = H_HEIGHT + 180;

    bool bRet = ExitObj->LoadImg("image\\exit.bmp", m_Screen);
    if (bRet)
    {
        y_pos += 50;
        ExitObj->SetRect(x_pos, y_pos);
        menu_list.push_back(ExitObj);
    }

    m_EndMenuOver.SetFont(m_Font50);
    m_EndMenuOver.SetFont(m_Font50);

    m_EndMenuOver.SetImgOptionList(menu_list);
    m_EndMenuWin.SetImgOptionList(menu_list);
}

///////////////////////////////////////////////////////////////////////////
// Hàm Loop:
// Vòng lặp chơi game cho đến khi dừng chương trình
//////////////////////////////////////////////////////////////////////////
void GameMain::Loop()
{
    // thời gian đếm ngược bắt đầu từ 300s
    // khi về 0, thì sẽ thua
    INT time_down = 300;

    ImpTimer fps;

    bool quit = false;

    // ExpAds là mảng quản lý toàn bộ đối tượng kẻ địch sinh ra ngoại trừ nhân vật Boss
    ExpAds* pExpAds = ExpAds::GetInstance();

    ThreatsAds* pThreatsAd = ThreatsAds::GetInstance();
    pThreatsAd->BuildMonster(m_Screen);

    // Nhân vật bossObj
    // chỉ xuất hiện ở cuối bài chơi, khi play tiến đến sát
    BossObject bossObj;
    //bossObj.Init(true, 15, 0, m_Screen);
    bossObj.Init(true, 370, 0, m_Screen);


    bool isWin = false;
    unsigned int frame_count = 0; // đếm khung thời gian để tính toán 1s
    while (!quit)
    {
        fps.start();

        // Vong lặp bắt sự kiện chuột phím
        while (SDL_PollEvent(&m_Event) != 0)
        {
            if (m_Event.type == SDL_QUIT)
            {
                // nếu bấm dấu x góc phải windows, thì game stop
                quit = true;
            }

            if (m_Event.type == SDL_KEYDOWN)
            {
                // Nếu bấm space thì game pause
                if (m_Event.key.keysym.sym == SDLK_SPACE)
                {
                    m_GameState = GAME_PAUSE;
                    Music::GetInstance()->PlaySoundGame(Music::GAME_PAUSE);
                }
            }

            if (m_GameState == GameStatus::GAME_START)
            {
                // Khi game ở màn hình start, thì chỉ màn hình start sẽ xử lý các sự kiện
                m_StartMenu.MenuAction(m_Event, m_Screen);
            }
            else if (m_GameState == GameStatus::GAME_PLAYING)
            {
                // Khi game ở trạng thái playing
                // thì các đối tượng game như player, thread... được xử lý sự kiện
                m_player.HandleInputAction(m_Event, m_Screen);
                pThreatsAd->HandleInputAction(m_Event, m_Screen);
            }
            else if (m_GameState == GameStatus::GAME_PAUSE)
            {
                // Khi game ở màn hình pause, thì chỉ màn hình pause sẽ xử lý các sự kiện
                m_PauseMenu.MenuAction(m_Event, m_Screen);
            }
            else if (m_GameState == GameStatus::GAME_END)
            {
                // Khi game ở màn hình end, thì chỉ màn hình end sẽ xử lý các sự kiện
                m_EndMenuOver.MenuAction(m_Event, m_Screen);
                m_EndMenuWin.MenuAction(m_Event, m_Screen);
            }
        }

        // Chạy bản nhạc xuyên suốt quá trình chơi game
        Music::GetInstance()->PlayMusic();

        // thiết lập chế độ màu màn hình ban đầu.
        SDL_SetRenderDrawColor(m_Screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(m_Screen);

        // Kiểm tra trạng thái game ban đầu là gì
        // Thường là start game
        if (m_GameState == GameStatus::GAME_START)
        {
            // Xử lý start game
            m_StartMenu.Render(m_Screen);
            int iOption = m_StartMenu.GetOption();
            int is = m_StartMenu.GetSelect();
            if (is == 0) // bấm vào play
            {
                fps.start();
                // chuyển game sang trạng thái playing
                m_GameState = GAME_PLAYING;

                // đưa kết quả phím bấm của start_menu về -1(vì lúc này màn hình start sẽ biến mất)
                m_StartMenu.SetIsSelect(-1);
                m_player.SetOptionPlayer(iOption);

            }
            else if (is == 1) // bấm vào exit game
            {
                quit = true;
                m_StartMenu.SetIsSelect(-1);
                continue;
            }
        }
        else if (m_GameState == GameStatus::GAME_PLAYING)
        {
            // Hiển thị ảnh Background
            m_Background.Render(m_Screen, NULL);

            // Hiển thị map với 2 lớp layer
            GameMap::GetInstance()->DrawMap(m_Screen, 1);
            GameMap::GetInstance()->DrawMap(m_Screen, 0);

            // vẽ khung frame thông tin game
            Gemometric::RenderRectange(m_Rect, m_Screen);
            Gemometric::RenderOutline(m_RectOutLine, m_Screen);

            // hiển thị máu nhân vật game
            m_PlayerBlood.Show(m_Screen);

            // hiển thị tiêu đề game.
            m_Title.RenderText(m_Screen);

            //Hiển thị thời gian đếm ngược
            std::string str_val = std::to_string(time_down);
            std::string str_time = "Time: " + str_val;
            m_TTime.SetText(str_time);
            m_TTime.LoadFromRenderedText(m_Font, m_Screen);
            int xPos = min(SCR_WIDTH, m_MapX);
            m_TTime.SetPos(xPos - 200, 15);
            m_TTime.RenderText(m_Screen);

            // Hiển thị tiền
            int coinCount = m_player.GetMoneyCount();
            m_player_coin.SetCount(coinCount);
            m_player_coin.MakeText();
            m_player_coin.Show(m_Screen);

            // Hiển thị boss với điều kiện player vượt qua vị trí 325*64 trên bản đồ
            int xPosPlayer = m_player.get_x_pos();
            if (xPosPlayer >= 325 * TILE_SIZE)
            {
                bossObj.SetVisible(true);
            }

            // lấy thông tin kích thước vị trí của player
            SDL_Rect player_rect = m_player.GetRectFrame();

            // Lưu thông tin vị trí player vào boss để boss tính toán hành vi với player
            // vi dụ cách quá xa, thì chưa làm gì
            // đến gần 1 khoảng ví dụ a, thì boss sẽ bắn đạn
            // đến rất gần, thì boss sẽ tấn công cận chiến
            bossObj.SetPosPlayer(player_rect);

            // gọi hàm thực hiện di chyển boss
            bossObj.DoAction();

            // hàm xử lý bắn đạn của boss
            bossObj.DoBullet(m_Screen);

            // hiển thị boss
            bossObj.Show(m_Screen);

            // hiển thị player
            m_player.Show(m_Screen);

            // di chuyển, check rơi, nhảy player
            m_player.DoAction(m_Screen);

            // bắn đạn của player
            m_player.HandleBullet(m_Screen);

            // hiển thị kẻ địch
            pThreatsAd->Render(m_Screen);


            // kiểm tra trạng thái máu của player
            // khi có tín hiệu máu bị giảm ( ví dụ bị dính đạn, va chạm kẻ địch)
#if 1
            bool is_minus = m_player.get_minus();
            if (is_minus == true)
            {
                // tắt tin hiệu đi
                m_player.set_off_minus();

                // giảm cột máu 1 đơn vị
                m_PlayerBlood.MinusUpdate();
            }
#endif
            // Xử lý va chạm giữa đạn của player và kẻ địch
            if (m_player.get_is_death() == false)
            {
                // chỉ xử lý khi player đang ở trạng thái sống.
                // khi player mất 1 cột máu, nó có 60ms để hồi sinh, đó là khoảng thời gian chưa sống lại
                // nên ko check va chạm lúc đó.

                std::vector<BulletObject*> bullet_arr = m_player.GetBullets();
                for (int am = 0; am < bullet_arr.size(); am++)
                {
                    BulletObject* p_bullet = bullet_arr.at(am);

                    // lấy danh sách viên đạn của player, chỉ check với viên đạn đang di chuyển
                    if (p_bullet && p_bullet->get_is_move() == true)
                    {

                        // kiểm tra va chạm giữa đạn của player và đối tượng kẻ địch
                        // biến true thư nhất là khi xảy ra va chạm, cho phép xóa kẻ địch
                        // biến true thứ 2. là nếu kẻ địch là đốm lửa, thì bỏ qua, ko xử lý va chạm với đốm lửa
                        // nghia la khi gạp lửa, thì player buộc phải nhảy qua. (ko bắn được lửa)
                        bool bRet = pThreatsAd->CheckCollision(p_bullet->GetRect(), true, true);
                        if (bRet)
                        {

                            // khi xảy ra va chạm, thì thực hiện tạo một hiệu ứng vụ nổ
                            ExplosionObject* pExp = new ExplosionObject(4);
                            bool ret = pExp->LoadImg("image//iron_ex.bmp", m_Screen);
                            if (ret)
                            {
                                // vị trí vụ nổ là vị trí của viên đạn khi xảy ra va chạm
                                SDL_Rect rc_pos = p_bullet->GetRect();
                                pExp->SetXP(rc_pos);
                                // lưu vụ nổ vào biến quản lý vụ ổ
                                pExpAds->Add(pExp);
                            }

                            // tăng điểm lên 1 đơn vị (khi bắn được 1 kẻ địch)
                            m_Mark++;
                            // xóa viên đạn đã bắn ra khi giết được kẻ địch
                            m_player.RemoveBullet(am);
                            continue; // viên đạn đã hết vài trò, nên sẽ thực hiện chuyển sang viên đạn kế tiếp
                        }

                        // nếu viện đạn không va chạm với kẻ đich nào, thì sẽ kiểm tra với boss
                        // tương tự kiểm tra va chạm với đối tượng boss
                        if (bossObj.GetVisible() == true)
                        {
                            // chỉ kiểm tra va chạm với boss khi nó xuất hiện
                            bool bShotBoss = TBase::CheckCollision(p_bullet->GetRect(), bossObj.GetRectFrame());
                            if (bShotBoss == true)
                            {
                                // BOSS có chỉ số máu = 50
                                // mỗi lần va chạm, viên đạn sẽ làm máu của boss giảm đi 1 đơn vị
                                bossObj.MinusBlood();

                                // viên đạn cũng sẽ bị xóa
                                p_bullet->set_is_move(false);

                                // tạo một vụ nổ xảy ra.
                                // Như vậy muốn chiến thắng boss phải bắn trúng boss 50 phát
                                ExplosionObject* pExp = new ExplosionObject(4);
                                bool ret = pExp->LoadImg("image//iron_ex.bmp", m_Screen);
                                if (ret)
                                {
                                    SDL_Rect rc_pos = p_bullet->GetRect();
                                    pExp->SetXP(rc_pos);
                                    pExpAds->Add(pExp);
                                }
                            }
                        }
                    }
                }
            }

            // trong suốt quá trinh chơi, mỗi khi player bị va chạm hay dính đạn
            // chỉ số máu sẽ bị giảm
            // Nếu chỉ số máu không còn, hoặc thời gian hết, thì game két thức
            if (m_PlayerBlood.IsEmpty() == true || time_down <= 0)
            {
                // đưa time = 0 để tránh nó hiệ thị giá trị âm
                time_down = 0;
                // chuyển trạng thái game sang màn hình end
                m_GameState = GAME_END;
                int coin_sum = m_player_coin.GetCount();
                m_MarkLog.SetMark(coin_sum);
                m_MarkLog.SaveMark();
                std::vector<int> mark_list = m_MarkLog.GetMarkList();
                m_EndMenuOver.SetMarkCurrent(coin_sum);
                m_EndMenuOver.SetMarkList(mark_list);
                m_EndMenuOver.MakeMarkText();

                // game over nghĩa là win = false
                isWin = false;
            }

            // Trái lại nêu máu của boss giảm về 0 
            if (bossObj.GetBlood() <= 0)
            {
                // boss sẽ bị chết
                bossObj.set_is_alive(false);
                // chương trình cũng kết thúc
                m_GameState = GAME_END;
                int coin_sum = m_player_coin.GetCount();

                m_MarkLog.SetMark(coin_sum);
                m_MarkLog.SaveMark();
                std::vector<int> mark_list = m_MarkLog.GetMarkList();
                m_EndMenuOver.SetMarkCurrent(coin_sum);
                m_EndMenuWin.SetMarkList(mark_list);
                m_EndMenuWin.MakeMarkText();
                // win = true
                isWin = true;
                continue;
            }
#if 0
            // an ko hien thi point
            //Hiển thị điểm bắn kẻ địch
            std::string val_str_mark = std::to_string(m_Mark);
            std::string strMark("Point: ");
            strMark += val_str_mark;

            m_MarkObj.SetText(strMark);
            m_MarkObj.LoadFromRenderedText(m_Font, m_Screen);
            m_MarkObj.RenderText(m_Screen);
#endif

            // Kiểm tra va chạm giữa player và kẻ địch, player và đạn kẻ địch
            if (m_player.get_is_death() == false)
            {
                // player vs kẻ địch
                //bool bRet1 = pThreatsAd->CheckCollision(m_player.GetRectFrame(), false);

                // player vs đạn kẻ địch
                bool bRet2 = pThreatsAd->CheckCollisionBulThreat(m_player.GetRectFrame());

                //Check collision with Boss
                SDL_Rect boss_rect = bossObj.GetRectFrame();

                // kiểm tra player vs với boss
                bool bRet3 = false;
                if (bossObj.get_is_alive() == true && bossObj.GetVisible() == true)
                {
                    TBase::CheckCollision(player_rect, boss_rect);
                }
                // kiểm tra player vs đạn của boss
                bool bRet4 = bossObj.CheckCollisionBullet(player_rect);


                // Khi 1 trong 4 trường hợp trên xảy ra
                // player đều bị mất 1 máu
#if 1
                if (/*bRet1*/  bRet2 || bRet3 || bRet4)
                {
                    if (m_player.get_is_death() == false)
                    {
                        // chạy âm thanh mất máu
                        Music::GetInstance()->PlaySoundGame(Music::BLOOD_SOUND);

                        // sét trạng thái đã bị chết
                        m_player.set_is_death(true);
                        // sét thời gian chờ hồi sinh là 100ms
                        m_player.set_alive_time(100);
                        // giảm máu đi 1 đơn vị
                        m_PlayerBlood.MinusUpdate();

                        // tạo một vụ nổ
                        ExplosionObject* pExp = new ExplosionObject(4);
                        bool ret = pExp->LoadImg("image//iron_ex.bmp", m_Screen);
                        if (ret)
                        {
                            SDL_Rect rc_pos = m_player.GetRectFrame();
                            rc_pos.x += rc_pos.w * 0.5;
                            rc_pos.y += rc_pos.h * 0.5;

                            pExp->SetXP(rc_pos);
                            pExpAds->Add(pExp);
                        }
                    }
                }
#endif
            }
            // toàn bộ các vụ nổ tao ra và lưu lại sẽ được thực hiện hiển thị tại đây
            pExpAds->Render(m_Screen);


            // Đếm frame count.
            // khi fram_count = FRAMES_PER_SCOND nghĩa là thời gian 1s (thực) đã trôi qua
            frame_count++;
            if (frame_count == FRAMES_PER_SECOND)
            {
                // và time_down đếm ngược 1s
                time_down--;
                frame_count = 0;
            }
        }
        else if (m_GameState == GAME_PAUSE)
        {
            // Khi ấn phím space thì game bị pasue lại

            // toàn bộ các đối tượng game bị dừng lại

            // Cơ chế của pause sẽ giống với playing.
            // Đó là giữ lại toàn bộ các xử lý hiển thị, còn các xử lý hành vi thì bị chặn
            // ví dụ như bắn đạn, di chyển, chiểm tra va cham....

            // Background
            m_Background.Render(m_Screen, NULL);

            // Map
            GameMap::GetInstance()->DrawMap(m_Screen, 1);
            GameMap::GetInstance()->DrawMap(m_Screen, 0);

            Gemometric::RenderRectange(m_Rect, m_Screen);
            Gemometric::RenderOutline(m_RectOutLine, m_Screen);

            m_PlayerBlood.Show(m_Screen);

            // Title
            m_Title.RenderText(m_Screen);

            // Time
            std::string str_val = std::to_string(time_down);
            std::string str_time = "Time: " + str_val;
            m_TTime.SetText(str_time);
            m_TTime.LoadFromRenderedText(m_Font, m_Screen);
            int xPos = min(SCR_WIDTH, m_MapX);
            m_TTime.SetPos(xPos - 200, 15);
            m_TTime.RenderText(m_Screen);

            int coinCount = m_player.GetMoneyCount();
            m_player_coin.SetCount(coinCount);
            m_player_coin.MakeText();
            m_player_coin.Show(m_Screen);

            bossObj.DoBullet(m_Screen);
            bossObj.Show(m_Screen);

            m_player.Show(m_Screen);
            m_player.HandleBullet(m_Screen, true);

            pThreatsAd->Render(m_Screen, true);

            //Show mark value to screen
            std::string val_str_mark = std::to_string(m_Mark);
            std::string strMark("Point: ");
            strMark += val_str_mark;

            m_MarkObj.SetText(strMark);
            m_MarkObj.LoadFromRenderedText(m_Font, m_Screen);
            m_MarkObj.RenderText(m_Screen);

            pExpAds->Render(m_Screen);

            m_PauseMenu.Render(m_Screen);

            // khi pasue bấm vào button resume hoặc exit
            int is = m_PauseMenu.GetSelect();
            if (is == 0)
            {
                // với resume thì thay đổi trạng thái game = playing để hoạt động bình thường
                m_GameState = GAME_PLAYING;
                m_PauseMenu.SetIsSelect(-1);
            }
            else if (is == 1)
            {
                // kết thúc game
                m_PauseMenu.SetIsSelect(-1);

                m_GameState = GAME_START;
                m_player.ReStart();


                GameMap::GetInstance()->GetMap()->SetStartX(0);
                GameMap::GetInstance()->GetMap()->SetStartY(0);

                pThreatsAd->ReStart(m_Screen);
                time_down = 300;
                m_player_coin.SetCount(0);
                m_PlayerBlood.ReStart(m_Screen);

                GameMap::GetInstance()->ResetMap(m_Screen);
                pExpAds->ResetExp();
                m_MarkLog.LoadMark();
                continue;
            }
        }
        else if (m_GameState == GAME_END)
        {
            // Khi trạng thái win/game over được thiết lập thì màn hình end game được hiển thị
            if (isWin == true)
            {
                m_EndMenuWin.Render(m_Screen);

                int is = m_EndMenuWin.GetSelect();
                if (is == 0)
                {
                    m_EndMenuWin.SetIsSelect(-1);
                    m_EndMenuWin.RemoveTextList();
                    m_GameState = GAME_START;
                    m_player.ReStart();

                    GameMap::GetInstance()->GetMap()->SetStartX(0);
                    GameMap::GetInstance()->GetMap()->SetStartY(0);

                    pThreatsAd->ReStart(m_Screen);
                    time_down = 300;
                    m_player_coin.SetCount(0);
                    m_PlayerBlood.ReStart(m_Screen);

                    GameMap::GetInstance()->ResetMap(m_Screen);
                    pExpAds->ResetExp();
                    m_MarkLog.LoadMark();
                    continue;
                }
            }
            else
            {
                m_EndMenuOver.Render(m_Screen);

                int is = m_EndMenuOver.GetSelect();
                if (is == 0)
                {
                    m_EndMenuOver.RemoveTextList();
                    m_EndMenuOver.SetIsSelect(-1);
                    m_GameState = GAME_START;
                    m_player.ReStart();

                    GameMap::GetInstance()->GetMap()->SetStartX(0);
                    GameMap::GetInstance()->GetMap()->SetStartY(0);

                    pThreatsAd->ReStart(m_Screen);
                    time_down = 300;
                    m_player_coin.SetCount(0);
                    m_PlayerBlood.ReStart(m_Screen);

                    GameMap::GetInstance()->ResetMap(m_Screen);
                    m_EndMenuOver.SetIsSelect(-1);
                    pExpAds->ResetExp();
                    m_MarkLog.LoadMark();
                    continue;
                }
            }
        }

        // Apply dữ liệu hiển thị vào màn hình screen
        SDL_RenderPresent(m_Screen);

        // cân bằng thời gian cho mỗi vòng lặp game
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    fps.stop();
}

// giải phóng vùng nhớ các đối tượng game
void GameMain::Close()
{
    ThreatsAds::GetInstance()->Free();
    m_Background.Free();

    SDL_DestroyRenderer(m_Screen);
    m_Screen = NULL;

    SDL_DestroyWindow(m_Window);
    m_Window = NULL;

    IMG_Quit();
    SDL_Quit();
}