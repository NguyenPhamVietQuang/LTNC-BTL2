
#include "stdafx.h"
#include "PlayerObject.h"
#include "BulletObject.h"
#include "Music.h"
#include "ExplosionObject.h"

PlayerObject::PlayerObject()//đóng gói các trạng thái của player
{
    on_ground_ = false;
    is_falling_ = false;
    is_death_ = false;

    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE;//không bước
    m_CoinCount = 0;
    alive_time_ = 0;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.up_ = 0;
    input_type_.down_ = 0;
    input_type_.jump_ = 0;
    m_bAttack = false;

    for (int i = 0; i < PLAYER_FRAMES; i++)
    {
        m_DelayFrame[i] = 50;
    }

    passed_time_ = 0;
    m_bMinusBlood = false;
    y_val_jump_ = 0;

    m_PlayerOption = 0;

    m_count_jump = 0;
}


PlayerObject::~PlayerObject()
{
    Free();
    FreeBullet();
}


// lấy thông tin vị trí player, và kích thước 1 frame
SDL_Rect PlayerObject::GetRectFrame()
{
  SDL_Rect rect;
  rect.x = m_Rect.x;
  rect.y = m_Rect.y;
  rect.w = m_Rect.w/ PLAYER_FRAMES;
  rect.h = m_Rect.h;
  return rect;
}


// xử lý sự kiện bàn phím
void PlayerObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (is_death_ == true || is_falling_ == true)
        return;

    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT: // di phải
        {
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            input_type_.left_ = 0;
            UpdateImagePlayer(screen);
            break;
        }
        case SDLK_LEFT: // đi trái
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0;
            UpdateImagePlayer(screen);
            break;
        }
        case SDLK_DOWN: // ngồi xuống (đang ko dùng)
        {
            input_type_.up_ = 0;
            input_type_.down_ = 1;
            break;
        }
        case SDLK_UP: // đi lên( ko dùng)
        {
            input_type_.up_ = 1;
            input_type_.down_ = 0;
            break;
        }
        case SDLK_RETURN:
        {
            break;
        }
        default:
            break;
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        // khi bấm key up, thì đưa các trạng thái di chuyển ở trên về = 0
        // nhân vật đứng im
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            input_type_.right_ = 0;
            break;
        case SDLK_LEFT:
            input_type_.left_ = 0;
            break;
        case SDLK_DOWN: 
        {
            input_type_.down_ = 0;
            break;
        }
        case SDLK_UP:
        {
            input_type_.up_ = 0;
            break;
        }
        default:
            break;
        }
    }
    else if (events.type == SDL_MOUSEBUTTONDOWN)
    {
        // khi click chuột trái thì bắn đạn
        if (events.button.button == SDL_BUTTON_LEFT)
        {
           CreateBullet(screen);
        }
        else if (events.button.button == SDL_BUTTON_RIGHT)
        {
            // kích chuột phải thì nhảy lên
            if (on_ground_ == true || m_count_jump < 2  && on_ground_ == false)
            {
                m_count_jump++;
                on_ground_ = false;
                input_type_.jump_ = 1;
                input_type_.down_ = 0;
            }
        }
    }
    else if (events.type == SDL_MOUSEBUTTONUP)
    {
        if (events.button.button == SDL_BUTTON_RIGHT)
        {
            input_type_.jump_ = 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Tạo đạn cho player
//////////////////////////////////////////////////////////////////////////
void PlayerObject::CreateBullet(SDL_Renderer* screen)
{
    m_bAttack = true;
    BulletObject* p_bullet = new BulletObject();//tạo ra một con trỏ kiểu bulletobj
    bool bRet = p_bullet->Init(kImgBullet, screen);//static char kImgBullet[] = { "image//iron_bullet.bmp" };
    if (bRet == true)
    {
        Music::GetInstance()->PlaySoundGame(Music::BULLET_SOUND);//hàm âm thanh
        INT xBul = x_pos_;//x_pos_ = 0;trong class gán băng vị trí nhân vật người chơi
        if (status_ == WALK_LEFT)//status_ = WALK_NONE;WALK_LEFT = 2,đi sang trai đi hai bc phải 1 bước
        {
            // quay bên trái thì bắn sang bên trái
            p_bullet->set_dir_bullet(BulletObject::DIR_LEFT);//void set_dir_bullet(const int& dir_type) { (int)bullet_dir_ = dir_type; }
            //DIR_RIGHT = 20,DIR_LEFT = 21,
            xBul -= p_bullet->GetRect().w;
            xBul -= x_val_;
            // Cập nhật vị trí x của đạn xBul bằng cách trừ chiều rộng của đạn và vận tốc x của người chơi.
        }
        else
        {
            // quay bên phải bắn sang ben phải
            p_bullet->set_dir_bullet(BulletObject::DIR_RIGHT);//nếu hướng bắn bên phải bắn 20 viên còn trái bắn 21 viên
            xBul += width_frame_;
            xBul += x_val_;
            //cộng chiều rộng của khung người chơi và vận tốc x của người chơi.
        }

        // vị trí xuất hiện của đạn hay là căn sao cho đạn xuất hiện giữa nòng súng
        if (m_PlayerOption == 0)
        {
            INT yBul = y_pos_ + height_frame_*0.5;
            p_bullet->set_xy_pos(xBul, yBul);
        }
        else
        {
            INT yBul = y_pos_ + height_frame_*0.3;
            p_bullet->set_xy_pos(xBul, yBul);
        }

        // tốc độ đạn = 25
        p_bullet->set_x_val(25);//void set_x_val(const int& x_val) { x_val_ = x_val; }
        m_BulletList.push_back(p_bullet);
    }
}

//////////////////////////////////////////////////////////////////////////
// xử lý bắn đạn
//////////////////////////////////////////////////////////////////////////
void PlayerObject::HandleBullet(SDL_Renderer* des, bool is_pause /* = false*/)
{
    for (int i = 0; i < m_BulletList.size(); i++)
    {
        BulletObject* bullet = m_BulletList.at(i);
        if (bullet != NULL)
        {
            if (bullet->get_is_move())//xem đạn đã bắn chưa trong một khoảng nhất định
            {
                bullet->Show(des);

                // pham vi bắn đạn là 500
                int xBoder1 = x_pos_ - 500;
                int xBoder2 = x_pos_ + width_frame_ + 500;
                if (is_pause == false) // nếu dang pause thì ko di chuyển đạn
                {
                    bullet->HandelMove(xBoder1, xBoder2);//hàm để bullet di chuyển

                    // kiểm tra dạn va chạm với bản đồ
                    bool ret = bullet->CheckToMap();
                    if (ret == true)
                    {
                        // nếu va chạm thì tạo hiệu ứng vụ nô và âm thanh
                        Music::GetInstance()->PlaySoundGame(Music::EXP_SOUND);
                        bullet->set_is_move(false);

                        ExplosionObject* pExp = new ExplosionObject(4);
                        bool ret = pExp->LoadImg("image\\iron_ex.bmp", des);
                        if (ret)
                        {
                            SDL_Rect rc_pos = bullet->GetExpPosition();
                            pExp->SetXP(rc_pos);
                            ExpAds::GetInstance()->Add(pExp);
                        }
                        continue;
                    }
                }
            }
            else
            {
                // nếu đạn đi quá phạm vi thì sẽ xóa đạn
                m_bAttack = false;
                RemoveBullet(i);
            }
        }
    }
}

// giải phóng viên đạn
void PlayerObject::RemoveBullet(const int& idx)
{
    if (m_BulletList.size() > 0 && idx < m_BulletList.size())
    {
        BulletObject* bullet = m_BulletList.at(idx);
        m_BulletList.erase(m_BulletList.begin() + idx);

        if (bullet)
        {
            delete bullet;
            bullet = NULL;
        }
    }
}

// giải phòng toàn bộ đạn
void PlayerObject::FreeBullet()
{
    for (int idx = 0; idx < m_BulletList.size(); idx++)
    {
        BulletObject* bullet = m_BulletList.at(idx);

        if (bullet != NULL)
        {
            delete bullet;
            bullet = NULL;
        }
    }

    m_BulletList.clear();
}

std::vector<BulletObject*> PlayerObject::GetBullets() const
{
    return m_BulletList;
}


void PlayerObject::SetBullets(VT(BulletObject*) bullet_list)
{
    m_BulletList = bullet_list;
}

// load ảnh nhân vật
bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret == true)
    {
        // tính toán kích thước 1 frame
        width_frame_ = m_Rect.w / PLAYER_FRAMES;
        height_frame_ = m_Rect.h;
        SetClip();
    }

    return ret;
}


// tính toán mảng dữ liệu frame
void PlayerObject::SetClip()
{
  if (width_frame_ > 0 && height_frame_ > 0)
  {
      for (int i = 0; i < PLAYER_FRAMES; i++)
      {
          m_FrameClip[i].x = width_frame_*i;
          m_FrameClip[i].y = 0;
          m_FrameClip[i].w = width_frame_;
          m_FrameClip[i].h = height_frame_;
      }
  }
}

// Hiển thị nhân vật player trên màn hình
void PlayerObject::Show(SDL_Renderer* des)
{
    Map* data_map = GameMap::GetInstance()->GetMap();

    // Khi map bị cuốn chiếu, thì giá trị mxStart > 0
    int mxStart = data_map->getStartX();
    int myStart = data_map->getStartY();

    UpdateImagePlayer(des);

    // x_pos_ là giá trị thực của player trên bản đồ game dài 25600
    // nhưng rect.x là giá trị theo màn hình máy tính
    // do đó giá trị màn hình máy tính phải = giá trị thực của bản đồ, trừ đi giá trị mép bản đồ đã bị cuốn đi

    m_Rect.x = x_pos_ - mxStart;
    m_Rect.y = y_pos_ - myStart;


    if (is_death_== false && is_falling_ == false)
    {
#if 0 
        // nếu không dùng kỹ thuật trễ khung hình
        if (input_type_.left_ == 1 || input_type_.right_ == 1)
        {
            frame_++;
        }
        else
        {
            frame_ = 0;
        }

        if (frame_ >= PLAYER_FRAMES)
        {
            frame_ = 0;
        }
#else
        // kỹ thuật trễ khung frame của nhân vật game
        if (SDL_GetTicks() - m_DelayFrame[frame_] > passed_time_)
        {
            passed_time_ = SDL_GetTicks();
            ++frame_;
            if (frame_ > PLAYER_FRAMES - 1)
            {
                frame_ = 0;
            }
        }

#endif
        // hiển thị nhân vật theo từng frame
        SDL_Rect* currentClip = &m_FrameClip[frame_];
        BaseObject::Render(des, currentClip);
    }
}

//////////////////////////////////////////////////////////////////////////
// Hàm thực hiện hành vi của player
//
//
//////////////////////////////////////////////////////////////////////////
void PlayerObject::DoAction(SDL_Renderer* des)
{
    Map* data_map = GameMap::GetInstance()->GetMap();
    if (is_falling_ == false && is_death_ == false)
    {
        // khi player được bám vào di chuyển trái (bàn phím là mũi tên trái)
        if (input_type_.left_ == 1)
        {
            // giá trị x_val bị giảm 1 lương speed
            x_val_ = -PLAYER_SPEED;
        }
        else if (input_type_.right_ == 1)
        {
            // với bên phải thì tăng 1 lượng speed
            x_val_ = PLAYER_SPEED;
        }
        else if (input_type_.left_ == 0 && input_type_.right_ == 0)
        {
            // khi ko di chyển, thì giá trị tăng = 0
            x_val_ = 0;
        }

        if (input_type_.jump_ == 1)
        {
            // nếu trạng thái nhảy thì giá trị jum được giảm 1 lượng
            y_val_jump_ = -PLAYER_HIGHT_VAL;
            input_type_.jump_ = 0;
        }


        // Kiểm tra hành vi
        // nếu giá trị y_val_jum nhỏ hơn 0, nghĩa là cần thực hiện nhảy
        if (y_val_jump_ < 0)
        {
            // gọi hàm nhảy
            DoJump();
        }
        else
        {
            // nếu không nhảy thì phải gọi hàm rơi tự do
            FreeFalling();
            y_pos_ += y_val_;
        }

        // Ngoài ra kiểm tra liên tục va chạm với bản đồ.
        CheckToMap(des);

        // khi nhân vật di chuyển, thì bản đồ bị cuôn chiếu
        // do đó cần tính toán giá trị start_x, start_y của bản đồ dựa trên vị trí x_pos, y_pos của player
        data_map->UpdateXMapInfo(x_pos_);
        data_map->UpdateYMapInfo(y_pos_);
    }
    else
    {
        // nếu nhân vật bị chết, thì
        // thời gian hồi sinh được thiết lập, ví dụ = 60
        if (alive_time_ > 0)
        {
            // bắt đầu giảm thời gian hồi sinh
            alive_time_ -= 1;
        }
        else
        {
            // khi thời gian chời hồi sinh = 0
            alive_time_ = 0;

            // bắt đầu hồi sinh player
            is_falling_ = false;
            is_death_ = false;

            ResetAlive();
        }
    }
}

BlockMap* PlayerObject::GetBlockMap(int y, int x)
{
    BlockMap* pBlock = NULL;
    GameMap* pMap = GameMap::GetInstance();
    if (pMap != NULL && pMap->GetMap())
    {
        Map* data_map = pMap->GetMap();
        VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
        if (tile_list.empty() == false)
        {
            UINT yNum = tile_list.size();
            if (y >= 0 && y < yNum)
            {
                UINT xNum = tile_list[y].size();
                if (x >= 0 && x < xNum)
                {
                    pBlock = tile_list[y][x];
                }
            }
        }
    }

    return pBlock;
}

BlockMap* PlayerObject::GetBlockMap2(int y, int x)
{
    BlockMap* pBlock = NULL;
    GameMap* pMap = GameMap::GetInstance();
    if (pMap != NULL && pMap->GetMap())
    {
        Map* data_map = pMap->GetMap();
        VT(VT(BlockMap*)) tile_list = data_map->GetTile2();
        if (tile_list.empty() == false)
        {
            UINT yNum = tile_list.size();
            if (y >= 0 && y < yNum)
            {
                UINT xNum = tile_list[y].size();
                if (x >= 0 && x < xNum)
                {
                    pBlock = tile_list[y][x];
                }
            }
        }
    }

    return pBlock;
}

void PlayerObject::CheckToMap(SDL_Renderer* des)
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL || is_death_ == true)
        return;

    Map* data_map = pMap->GetMap();

    if (input_type_.right_ == 1) // MOVE RIGHT
    {
        DoRight();
    }
    else if (input_type_.left_ == 1) // MOVE LEFT
    {
        DoLeft();
    }

    // Nếu xử lý trải, phải không va chạm với tile
    // giá trị x_val_ sẽ khác 0
    // và x_pos, y_pos được thay đổi vị trí
    // và như vậy player di chuyển tiếp
    x_pos_ += x_val_;
    //y_pos_ += y_val_;

    // nếu di chuyển trái, và khi di mãi về đầu bản đồ
    if (x_pos_ < 0)
    {
        //thì sẽ chặn lại ko di chuyển được nữa
        x_pos_ = 0;
    }
    else if (x_pos_ + width_frame_ >= data_map->getMaxX())
    {
        // Hoặc di chuyển tới cuối bản đồ
        x_pos_ = data_map->getMaxX() - width_frame_;//- EPXILON;
    }

    // giải sử khi bạn đứng ở tile rất cao, và nhảy cái vượt quá màn hình theo chiều y
    // thì cũng chặn lại, ko cho vượt quá
    if (y_pos_ < 0)
    {
        y_pos_ = 0;
    }
    else if (y_pos_ + height_frame_ >= data_map->getMaxY() + 2*TILE_SIZE)
    {
        // Khi y_pos vượt quá chiều cao của map theo phương y
        // nghĩa là player bị rơi xuống vực
        // coi như player chết, máu bị giảm đi 1 đơn vị
        m_bMinusBlood = true;
        is_falling_ = true; // thiết lập trạng thái rơi = true
        alive_time_ = 100;  // thiết lập thời gian chờ hồi sinh là 100
        Music::GetInstance()->PlaySoundGame(Music::BLOOD_SOUND);
    }
}

//////////////////////////////////////////////////////////////////////////
// xử lý va chạm với map khi di chuyển trái
// Khác với nhảy và rơi, left right chỉ có 1 chiều.
// và khi va chạm với ô tile a, thì player cũng phải đứng trọn vẹn trong  1 layer
// (Trừ khi chúng ta cố tình vẽ tile a nằm 1 nửa tile x, 1 nửa tile y)
//////////////////////////////////////////////////////////////////////////
void PlayerObject::DoLeft()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int yPos = (y_pos_ + height_frame_ - EPXILON);
    int xPosPrev = (x_pos_ + x_val_);

    // tìm ô tile sẽ di chuyển đến (bên trái) với curTileY và prevTileX
    int curTileY = yPos / TILE_SIZE;
    int prevTileX = xPosPrev / TILE_SIZE;

    bool IsInside = TBase::CheckInsideMap(prevTileX, curTileY);
    if (IsInside) 
    {
        // đảm bảo bên trong map
        BlockMap* pBlock = GetBlockMap(curTileY, prevTileX);
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {

                // kiểm tra type của ô tile map
     
                std::string sLType = pBlock->getType();
                bool bSkip = pMap->CheckSkipMap(sLType);
                int vProduct = pMap->GetValueProduct(sLType);

                // nếu không phải ngoại cảnh, hay giá trị đồ ăn
                if (bSkip == false && vProduct == 0)
                {
                    // player sẽ bị cản lại tại ô tile bên trái
                    x_pos_ = prevTileX*TILE_SIZE + width_frame_;

                    // ko cho di chuyển trái tiếp
                    x_val_ = 0;
                }
            }
        }
        else
        {
            // Nếu ô máp trái ko có tile cản trở
            // kiểm tra với layer2 xem có đồ ăn gì hay ko
            pBlock = GetBlockMap2(curTileY, prevTileX);
            if (pBlock->GetTile() != NULL)
            {
                std::string tp = pBlock->getType();
                int vProduct = pMap->GetValueProduct(tp);
                if (vProduct > 0)
                {
                    // nếu có đồ ăn thi phát âm thanh đồ ăn
                    Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
                    // xóa đồ ăn 
                    pBlock->setTile(0);

                    // tăng giá trị tiền
                    m_CoinCount += vProduct;
                }
            }
        }
    }
    else
    {
        x_val_ = 0;
    }
}

//////////////////////////////////////////////////////////////////////////
// Hàm xử lý right cũng tương tự như hàm xử lý trái
//////////////////////////////////////////////////////////////////////////
void PlayerObject::DoRight()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int xPosNext = (x_pos_ + width_frame_ + x_val_);
    int yPosEnd = (y_pos_ + height_frame_ - EPXILON);

    int nextTileX = xPosNext / TILE_SIZE;
    int curTileY = yPosEnd / TILE_SIZE;

    bool IsInside = TBase::CheckInsideMap(nextTileX, curTileY);
    if (IsInside)
    {
        BlockMap* pBlock = GetBlockMap(curTileY, nextTileX);
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                std::string sLType = pBlock->getType();
                bool bSkip = pMap->CheckSkipMap(sLType);
                int vProduct = pMap->GetValueProduct(sLType);
                if (bSkip == false && vProduct == 0)
                {
                    x_pos_ = nextTileX*TILE_SIZE - width_frame_;
                    x_val_ = 0;
                }
            }
            else
            {
                pBlock = GetBlockMap2(curTileY, nextTileX);
                if (pBlock->GetTile() != NULL)
                {
                    std::string tp = pBlock->getType();
                    int vProduct = pMap->GetValueProduct(tp);
                    if (vProduct > 0)
                    {
                        Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
                        pBlock->setTile(0);
                        m_CoinCount += vProduct;
                    }
                }
            }
        }
    }
    else
    {
        x_val_ = 0;
    }
}

//////////////////////////////////////////////////////////////////////////
// Hàm rơi tự do, có tính chất ngược với hàm jump
// Nhưng logic thì khá tương đồng
//////////////////////////////////////////////////////////////////////////
void PlayerObject::FreeFalling()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    y_val_ += GRAVITY_SPEED;
    if (y_val_ >= MAX_FALL_SPEED)
    {
        y_val_ = MAX_FALL_SPEED;
    }

    // Xác định 2 vị trí x1 và x2 để tìm 2 ô tile tương ứng với vị trí player đang rơi
    int curTileX1 = x_pos_ / TILE_SIZE;
    int curTileX2 = (x_pos_ + width_frame_ - EPXILON) / TILE_SIZE;

    // vì rơi nên sẽ tìm ô tile kế tiếp
    int yPosNext = y_pos_ + height_frame_ + y_val_;
    int nextTileY = yPosNext / TILE_SIZE;

    bool bInside1 = TBase::CheckInsideMap(curTileX1, nextTileY);
    bool bInside2 = TBase::CheckInsideMap(curTileX2, nextTileY);
    if (bInside1 && bInside2)
    {
        BlockMap* pBlock1 = GetBlockMap(nextTileY, curTileX1);
        if (pBlock1 != NULL)
        {
            if (pBlock1->GetTile() != NULL)
            {
                // tìm loại tile kế tiếp
                std::string tp1 = pBlock1->getType();
                bool bSkip1 = pMap->CheckSkipMap(tp1);
                int vProduct = pMap->GetValueProduct(tp1);

                // nếu không phải ngoại cảnh, hay giá trị thì sẽ xử lý dừng rơi
                if (bSkip1 == false && vProduct == 0)
                {
                    // dừng rơi nghĩa là đứng trên mặt đất
                    on_ground_ = true;
                    m_count_jump = 0;
                    // y_pos sẽ gán bằng vị trí của ô tile mà nó va chạm để đặt chân
                    y_pos_ = nextTileY*TILE_SIZE - height_frame_;
                    
                    // và ko cho rơi nữa
                    y_val_ = 0;
                }
            }
            else
            {
                // ngược lại nếu chưa rơi xuống mặt đất mà đã va chạm với tiền trên không trung
                pBlock1 = GetBlockMap2(nextTileY, curTileX1);
                if (pBlock1->GetTile() != NULL)
                {
                    std::string tp1 = pBlock1->getType();
                    int vProduct = pMap->GetValueProduct(tp1);
                    if (vProduct > 0)
                    {
                        // ăn tiền và phát âm thanh
                        Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
                        pBlock1->setTile(0); // xóa tiền khỏi vị trí đó
                        m_CoinCount += vProduct;  // cộng giá trị tiền
                    }
                }
            }
        }


        // Tương tự với ô tile thứ 2(nhắc lại player có thể đứng 1 nửa tile a, và 1 nửa tile b)
        // nên luôn phải kiểm tra 2 tile
        BlockMap* pBlock2 = GetBlockMap(nextTileY, curTileX2);
        if (pBlock2 != NULL)
        {
            if (pBlock2->GetTile() != NULL)
            {
                std::string tp2 = pBlock2->getType();
                bool bSkip2 = pMap->CheckSkipMap(tp2);
                int vProduct = pMap->GetValueProduct(tp2);

                if (bSkip2 == false && vProduct == 0)
                {
                    on_ground_ = true;
                    m_count_jump = 0;
                    y_pos_ = nextTileY*TILE_SIZE - height_frame_;
                    y_val_ = 0;
                }
            }
            else
            {
                pBlock2 = GetBlockMap2(nextTileY, curTileX2);
                if (pBlock2->GetTile() != NULL)
                {
                    std::string tp2 = pBlock2->getType();
                    int vProduct = pMap->GetValueProduct(tp2);
                    if (vProduct > 0)
                    {
                        Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
                        pBlock2->setTile(0);
                        m_CoinCount += vProduct;
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Thực hiện nhảy
//
//////////////////////////////////////////////////////////////////////////
void PlayerObject::DoJump()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;

    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;


    // Tại vị trí player đang thực hiện nhảy, x_pos, y_pos
    int curTileX1 = (x_pos_) / TILE_SIZE;  // tìm ra chỉ số của ô tile map thứ nhất

    // lấy giá trị x_pos + thêm chiều rộng đề tìm ra ô tile máp thứ 2
    int xPosCur = (x_pos_ + width_frame_ - EPXILON);
    int curTileX2 = xPosCur / TILE_SIZE;

    // Nếu curTileX1, và curTileX2 bằng nhau, nghĩa là player đứng nguyên trong 1 ô tile map
    // Nhưng nếu nó khác nhau, nghĩa là player đang đứng ở vị trí, 1 nửa ô tile này và 1 nửa ô tile kia.
    // Do đó, phải check 2 ô tile với chiều X

    // Khi nhảy lên, thì giá trị y_pos bị giảm đi 1 lượng (Phép công, nhưng chú ý :y_val_jum_ có giá trị âm)
    // cần tìm ổ tile theo chiều y sau khi giảm.
    // Mục đích là xem ô tile đó có phải tile va chạm hay ko, nếu có va chạm, thì player ko thể nhảy qua tile đó được.
    int prevTileY = (y_pos_ + y_val_jump_) / TILE_SIZE;
    
    // Khi đã có thông tin chỉ số ô tile map
    // kiểm ta đảm bảo ô tile đó nằm bên trong map hiện hành
    bool bInside1 = TBase::CheckInsideMap(curTileX1, prevTileY);
    bool bInside2 = TBase::CheckInsideMap(curTileX2, prevTileY);
    if (bInside1 && bInside2)
    {
        // Lấy dữ liệu ô tile map tại vị trí 1
        BlockMap* pBlock1 = GetBlockMap(prevTileY, curTileX1);
        if (pBlock1 != NULL)
        {
            if (pBlock1->GetTile() != NULL)
            {
                // Kiểm tra loại tile map
                std::string tp1 = pBlock1->getType();
                bool bSkip1 = pMap->CheckSkipMap(tp1);
                int vProduct = pMap->GetValueProduct(tp1);

                // Nếu ô tile này không phải đối tượng ngoại cảnh (bỏ qua), cũng ko phải đối tượng tiền.
                if (bSkip1 == false && vProduct == 0)
                {
                    // gán y_pos bằng vị trí va chạm với ô tile đó
                    y_pos_ = prevTileY*TILE_SIZE + height_frame_;
                    // ko cho phép nhảy thêm nữa
                    y_val_jump_ = 0;
                }
            }
            else
            {
                // Ngược lại nếu ô tile map đó không có gì
                // kiểm tra tiếp với layer2 của map
                // do layer 2 của map chỉ sử dụng cho đồ ăn
                pBlock1 = GetBlockMap2(prevTileY, curTileX1);
                if (pBlock1->GetTile() != NULL)
                {
                    // Nếu ô tile map đó tồn tại và thuộc nhóm đồ ăn
                    std::string tp1 = pBlock1->getType();
                    // tìm giá trị của đồ ăn
                    int vProduct = pMap->GetValueProduct(tp1);
                    if (vProduct > 0)
                    {
                        // nếu đúng đồ ăn, ví dụ tiền.
                        // phát âm thanh ăn tiền
                        Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
                        pBlock1->setTile(0); // xóa tiền khỏi ô tile máp đó
                        m_CoinCount += vProduct; // cộng giá trị tiền đã ăn
                    }
                }
            }
        }

        // Tương tự như vậy với ô tile ở block2 ví trị curtileX2
        BlockMap* pBlock2 = GetBlockMap(prevTileY, curTileX2);
        if (pBlock2 != NULL)
        {
            if (pBlock2->GetTile() != NULL)
            {
                std::string tp2 = pBlock2->getType();
                bool bSkip2 = pMap->CheckSkipMap(tp2);
                int vProduct = pMap->GetValueProduct(tp2);

                if (bSkip2 == false && vProduct == 0)
                {
                    y_pos_ = prevTileY*TILE_SIZE + height_frame_;
                    y_val_jump_ = 0;
                }
            }
            else
            {
                pBlock2 = GetBlockMap2(prevTileY, curTileX2);
                if (pBlock2->GetTile() != NULL)
                {
                    std::string tp2 = pBlock2->getType();
                    int vProduct = pMap->GetValueProduct(tp2);
                    if (vProduct > 0)
                    {
                        Music::GetInstance()->PlaySoundGame(Music::COIN_INCREASING);
                        pBlock2->setTile(0);
                        m_CoinCount += vProduct;
                    }
                }
            }
        }
    }

    // Nếu ko xảy a bất cứ va chạm nào, thì 
    // y_pos sẽ giảm 1 lượng giá trị y_val_jump
    if (y_val_jump_ < 0)
    {
        y_pos_ += y_val_jump_;
        y_val_jump_++;
    }
    else
    {
        y_val_jump_ = 0;
    }
}

void PlayerObject::DoUpCoin()
{
    m_CoinCount++;
}

//////////////////////////////////////////////////////////////////////////
// Update trạng thai player game
// Khi chơi, player có nhiều lần thay đổi hành vi
// đi trái, đi phải, đứng im, tấn công, bắn đạn..
//
//////////////////////////////////////////////////////////////////////////
void PlayerObject::UpdateImagePlayer(SDL_Renderer* des)
{
    if (m_PlayerOption == 0)
    {
        if (input_type_.left_ == 1 || input_type_.right_ == 1)
        {
            if (m_bAttack == true)
            {
                // nếu đi trái hoặc đi phải, và bấm chuột bắn đạn
                // Load ảnh vừa đi vừa bắn
                LoadImg(sPlayerRunShot, des);
            }
            else
            {
                // Ngược lại chỉ load ảnh đi
                LoadImg(sPlayerMove, des);
            }
        }
        else
        {
            // nếu ko di chuyển
            if (m_bAttack == false)
            {
                // nếu ko di chuyển và ko bấm chuột bắn đạn
                // load ảnh đứng im
                LoadImg(sPlayerIdle, des);
            }
            else
            {
                // ngược lại load ảnh đứng im bắn đạn
                LoadImg(sPlayerIdleShot, des);
            }
        }

        // Mọi trạng thái ở trên đều chỉ có 2 kiểu quay của player
        // Quay bên trái hay bên phải
        // mặc định các ảnh thiết kế đều quay bên phải
        if (status_ == WALK_LEFT)
        {
            // do đó khi quay trái (đi trái, jum trái, tấn công trái)
            // sét flip = true để anh lộn ngược lại
            m_Flip = true;
        }
        else if (status_ == WALK_RIGHT)
        {
            // nếu quay bên phải, thì ko thực hiện flip
            m_Flip = false;
        }
    }
    else // Solider
    {
        if (input_type_.left_ == 1 || input_type_.right_ == 1)
        {
            // load anh di chuyen
            LoadImg(sPlayerSoldWalk, des);
        }
        else
        {
            //load ảnh đứng
            LoadImg(sPlayerSoldIde, des);
        }

        // Mọi trạng thái ở trên đều chỉ có 2 kiểu quay của player
        // Quay bên trái hay bên phải
        // mặc định các ảnh thiết kế đều quay bên phải
        if (status_ == WALK_LEFT)
        {
            // do đó khi quay trái (đi trái, jum trái, tấn công trái)
            // sét flip = true để anh lộn ngược lại
            m_Flip = true;
        }
        else if (status_ == WALK_RIGHT)
        {
            // nếu quay bên phải, thì ko thực hiện flip
            m_Flip = false;
        }
    }
}


// Hồi sinh player
void PlayerObject::ResetAlive()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL) return;

    // xóa các hành vi trước đó
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;

    // tính toán lại vị trí hồi sinh
    // lùi sau vị trí chết 4 tile
    x_pos_ = x_pos_ - TILE_SIZE * 4;
    if (x_pos_ < 0)
    {
        x_pos_ = 0;
    }

    y_pos_ = 200;

    // tính toán lại vị trí vẽ player trên màn hình
    Map* data_map = GameMap::GetInstance()->GetMap();

    int mxStart = data_map->getStartX();
    int myStart = data_map->getStartY();

    m_Rect.x = x_pos_ - mxStart;
    m_Rect.y = y_pos_ - myStart;
}


void PlayerObject::ReStart()
{
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    x_pos_ = 100;
    y_pos_ = 100;
    m_CoinCount = 0;


    on_ground_ = false;
    is_falling_ = false;
    is_death_ = false;

    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_NONE;
    alive_time_ = 0;
    m_bAttack = false;
    m_bMinusBlood = false;
    y_val_jump_ = 0;
}