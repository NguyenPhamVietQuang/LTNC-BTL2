
#include "stdafx.h"
#include "BossObject.h"

BossObject::BossObject()
{
    x_plus_ = 0;
    m_Visible = false;
    type_ = TH_BOS;

    m_BossSate = BS_WALK;

    m_BloodBos = 50;
    is_alive_ = true;
}


BossObject::~BossObject()
{
    FreeBullet();
}

//////////////////////////////////////////////////////////////////////////
// khởi tạo boss
//////////////////////////////////////////////////////////////////////////
void BossObject::Init(bool bClip, int xTile, int yTile, SDL_Renderer* screen)
{
    // clip là tham số cho phép xử lý độ trễ thời gian giữa các khung hình nhân vật.
    // thương các nhân vật có 8 khung hình, và nó load từ 0 đến 8 rất nhanh
    // do đó hiệu ứng game quá nhanh nó gây rối mắt.
    // bClip cho phép sử dụng kỹ thuật tạo độ trễ cho các khung hình
    this->set_is_clip(bClip);
    // set vị trí xuất hiện boss
    this->set_xpos(xTile * TILE_SIZE);
    this->set_ypos(yTile * TILE_SIZE);
    // load ảnh boss
    this->LoadImg(sBossWalk, screen);
    // tạo đạn cho boss
    this->AddBullet(screen);
}

// hàm giảm máu của boss
void BossObject::MinusBlood() 
{   
    m_BloodBos--; 
    if (m_BloodBos <= 0)
    {
        m_BloodBos = 0;
    }
}

// Xử lý hành vi của boss
void BossObject::DoAction()
{
    // khi boss ở trạng thái sống.
    if (is_alive_ == true)
    {
        // thực hiện hành vi rơi tư do xuống mặt đất
        // giá trị y_val sẽ được tăng dần với một giá trị rơi
        y_val_ += GRAVITY_SPEED;
        if (y_val_ >= MAX_FALL_SPEED) // chi cho phép y_val tăng đén một giới hạn max_fall_speed
        {
            // điều này tạo hiệu ứng rơi nhanh dần, và chạm giới hạn sẽ rơi đều
            y_val_ = MAX_FALL_SPEED;
        }

        // trong quá trình boss rơi sẽ liên tục kiểm tra va chạm với bản đồ game
        CheckToMap();
    }
}

//////////////////////////////////////////////////////////////////////////
// Tương tự như phân tích ở player, đọc comment ở phần player
//////////////////////////////////////////////////////////////////////////
void BossObject::CheckToMap()
{
    if (on_ground_ == true)
    {
        if (m_BossSate == BS_WALK)
        {
            if (x_val_ > 0)
            {
                DoRight();
            }
            else if (x_val_ < 0)
            {
                DoLeft();
            }
            x_pos_ += x_val_;
        }
    }

    if (y_val_ > 0)
    {
        DoDown();
    }

    y_pos_ += y_val_;


    if (y_pos_ > pMap_->getMaxY())
    {
        is_alive_ = false;
    }
}

void BossObject::DoLeft()
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

    int curTileY = yPos / TILE_SIZE;
    int prevTileX = xPosPrev / TILE_SIZE;

    int pX = xPosPrev - prevTileX*TILE_SIZE;
    int pY = yPos - curTileY*TILE_SIZE;

    bool IsInside = TBase::CheckInsideMap(prevTileX, curTileY);
    if (IsInside)
    {
        BlockMap* pBlock = tile_list[curTileY][prevTileX];
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                std::string sLType = pBlock->getType();
                bool bSkip = pMap->CheckSkipMap(sLType);
                if (bSkip == false)
                {
                    x_pos_ = prevTileX*TILE_SIZE + width_frame_;
                    x_val_ = 0;
                }
            }
        }
    }
    else
    {
        x_val_ = 0;
    }
}


void BossObject::DoRight()
{
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;
    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int nextTX = (x_pos_ + x_val_ + width_frame_) / TILE_SIZE;
    int curTY = (y_pos_ + height_frame_ - EPXILON) / TILE_SIZE;

    bool bInside = TBase::CheckInsideMap(nextTX, curTY);
    if (bInside)
    {
        BlockMap* pBlock = tile_list[curTY][nextTX];
        if (pBlock != NULL)
        {
            if (pBlock->GetTile() != NULL)
            {
                std::string sLType = pBlock->getType();
                bool bSkip = pMap->CheckSkipMap(sLType);
                if (bSkip == false)
                {
                    x_pos_ = nextTX*TILE_SIZE - width_frame_;
                    x_val_ = 0;
                }
            }
        }
    }
    else
    {
        x_val_ = 0;
    }
}


void BossObject::DoDown()
{
    on_ground_ = 0;
    GameMap* pMap = GameMap::GetInstance();
    if (pMap == NULL || pMap->GetMap() == NULL)
        return;
    Map* data_map = pMap->GetMap();
    VT(VT(BlockMap*)) tile_list = data_map->GetTile1();
    if (tile_list.empty() == true)
        return;

    int curTileX1 = x_pos_ / TILE_SIZE;
    int curTileX2 = (x_pos_ + width_frame_) / TILE_SIZE;

    int yPosNext = y_pos_ + height_frame_ + y_val_;
    int nextTileY = yPosNext / TILE_SIZE;

    bool bInside1 = TBase::CheckInsideMap(curTileX1, nextTileY);
    bool bInside2 = TBase::CheckInsideMap(curTileX2, nextTileY);
    if (bInside1 && bInside2)
    {
        BlockMap* pBlock1 = tile_list[nextTileY][curTileX1];
        if (pBlock1 != NULL && pBlock1->GetTile() != NULL)
        {
            std::string tp1 = pBlock1->getType();
            bool bSkip1 = pMap->CheckSkipMap(tp1);
            if (bSkip1 == false)
            {
                on_ground_ = true;
                y_pos_ = nextTileY*TILE_SIZE - height_frame_;
                y_val_ = 0;
            }
        }

        if (nextTileY >= 0 && nextTileY < tile_list.size())
        {
            if (curTileX2 >= 0 && curTileX2 < tile_list[nextTileY].size())
            {
                BlockMap* pBlock2 = tile_list[nextTileY][curTileX2];
                if (pBlock2 != NULL && pBlock2->GetTile() != NULL)
                {
                    std::string tp2 = pBlock2->getType();
                    bool bSkip2 = pMap->CheckSkipMap(tp2);
                    if (bSkip2 == false)
                    {
                        on_ground_ = true;
                        y_pos_ = nextTileY*TILE_SIZE - height_frame_;
                        y_val_ = 0;
                    }
                }
            }
        }
    }
}


void BossObject::Update(SDL_Renderer* screen)
{
    int rx = x_pos_ - pMap_->getStartX();
    if (on_ground_ == 1)
    {
        if (m_BossSate == BS_WALK)
        {
            if (v_dir_ == 1)
            {
                x_val_ = -3;
            }
            else if (v_dir_ == -1)
            {
                x_val_ = 3;
            }

            x_plus_ += 3;
            if (x_plus_ >= 333)
            {
                v_dir_ *= -1;
                if (v_dir_ == -1)
                {
                    this->SetIsFlip(true);
                }
                else
                {
                    this->SetIsFlip(false);
                }
                x_plus_ = 0;
            }
        }
        else
        {
            if (rx > (m_PosPlayer.x + m_PosPlayer.w*0.5))
            {
                if (this->GetFlip() != false)
                {
                    this->SetIsFlip(false);
                    for (auto bb : m_BulletList)
                    {
                        bb->set_dir_bullet(BulletObject::DIR_LEFT);
                        bb->set_is_move(false);
                    }
                }
            }
            else
            {
                if (this->GetFlip() != true)
                {
                    this->SetIsFlip(true);
                    for (auto bb : m_BulletList)
                    {
                        bb->set_dir_bullet(BulletObject::DIR_RIGHT);
                        bb->set_is_move(false);
                    }
                }
            }
        }


        int dis = abs(rx - m_PosPlayer.x);
        if (dis <= 384)
        {
            if (dis <= 128)
            {
                if (m_BossSate != BS_ATTACK)
                {
                    m_BossSate = BS_ATTACK;
                    int h_frame_old = height_frame_;
                    LoadImg(sBossAttak, screen);

                    int sub_h = height_frame_ - h_frame_old;
                    y_pos_ -= sub_h;
                }
            }
            else
            {
                if (m_BossSate != BS_SHOT)
                {
                    m_BossSate = BS_SHOT;
                    int h_frame_old = height_frame_;
                    LoadImg(sBossShot, screen);
                    int sub_h = height_frame_ - h_frame_old;
                    y_pos_ -= sub_h;
                }
            }
        }
        else
        {
            if (m_BossSate != BS_WALK)
            {
                int h_frame_old = height_frame_;
                m_BossSate = BS_WALK;
                LoadImg(sBossWalk, screen);
                int sub_h = height_frame_ - h_frame_old;
                y_pos_ -= sub_h;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Hiển thị boss
//////////////////////////////////////////////////////////////////////////
void BossObject::Show(SDL_Renderer* des)
{
    if (m_Visible == true && is_alive_ == true)
    {
        Update(des);
        ThreatsObject::Show(des);
    }
}

//////////////////////////////////////////////////////////////////////////
// Thêm đạn
//////////////////////////////////////////////////////////////////////////
void BossObject::AddBullet(SDL_Renderer* screen)
{
    if (is_alive_ == true)
    {
        BulletObject* pBul = NULL;
        pBul = new BulletObject();
        // hương đạn sẽ là từ phải sang trái
        pBul->set_dir_bullet(BulletObject::DIR_LEFT);
        // đạn là dạng laser lửa
        pBul->Init(kImgBossFire, screen);
        // tốc độ là 18 nhanh hơn player
        pBul->set_x_val(18);
        m_BulletList.push_back(pBul);
    }
}

//////////////////////////////////////////////////////////////////////////
// xử lý bắn đạn
//////////////////////////////////////////////////////////////////////////
void BossObject::DoBullet(SDL_Renderer* screen)
{
    // nếu boss ko hiển thị hay đã chết, thì ko xử lý bắn đạn
    if (is_alive_ == false || m_Visible == false)
        return;

    // Nếu boss không ở trạng thái bắn đạn thì cũng ko xử lý bắn đạn
    if (m_BossSate != BS_SHOT)
        return;

    // Khi boss rơi xuống mặt đất, thì mới bắn đạn
    if (on_ground_ == true)
    {
        if (m_BulletList.size() > 0)
        {
            // phát âm thanh bắn đạn laser
            Music::GetInstance()->PlaySoundGame(Music::LASER_SOUND);
        }

        for (size_t iBul = 0; iBul < m_BulletList.size(); iBul++)
        {
            BulletObject* pBul = m_BulletList.at(iBul);
            if (pBul != NULL)
            {
                // nếu viện đạn ở trạng thái di chuyển
                if (pBul->get_is_move() == true)
                {
                    // thực hiện bắn đạn với phạm vi từ vị trí xuất phát + 500 đơn vị(hơn 8 tile)
                    int xLimit1 = x_pos_ - 500; // dùng cho bắn về bên trái
                    int xLimit2 = x_pos_ + 500; // bắn về bên phải (nếu player nhảy vọt qua đầu boss, boss sẽ quay lại)

                    // gọi hàm xử lý đạn bắn và hiển thị đạn bắn
                    pBul->HandelMove(xLimit1, xLimit2);
                    pBul->Show(screen);
                }
                else
                {
                    // khi viên đạn boss bắn quá phạm vi cho phép, hoặc va chạm với player
                    // nó được sét is_move = false
                    // lúc này viên đạn sẽ được phục hồi

                    // trước khi phục hồi nó cần biết boss đang quay bên phải hay bên trái
                    int rx = x_pos_ - pMap_->getStartX();
                    if (rx > m_PosPlayer.x)
                    {
                        // nếu player trươc boss (chiều từ trái -> phải) thì đạn phát ra từ vị trí xpos
                        int xBul = x_pos_;
                        int yBul = y_pos_ + height_frame_*0.6;
                        pBul->set_xy_pos(xBul, yBul);
                    }
                    else
                    {
                        // nhưng nếu player nhảy qua đầu boss đứng sau boss
                        // boss sẽ quay đầu lại, và như vậy viên đạn sẽ phát ra vị trí x_pos + width_frame
                        int xBul = x_pos_ + width_frame_;
                        int yBul = y_pos_ + height_frame_*0.6;
                        pBul->set_xy_pos(xBul, yBul);
                    }

                    // hồi sinh viên đạn

                    pBul->set_is_move(true);
                }
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// xử lý va chạm player và đạn của boss
//////////////////////////////////////////////////////////////////////////

bool BossObject::CheckCollisionBullet(SDL_Rect& rect)
{
    if (is_alive_ == false || m_Visible == false)
        return false;

    bool bFind = false;
    if (m_BossSate == BossObject::BS_SHOT) // chỉ xử lý khi boss ở trạng thái bắn đạn
    {
        for (size_t iBul = 0; iBul < m_BulletList.size(); iBul++)
        {
            BulletObject* pBul = m_BulletList.at(iBul);
            if (pBul != NULL)
            {
                if (pBul->get_is_move() == true)
                {
                    bFind = TBase::CheckCollision(pBul->GetRect(), rect);
                    if (bFind == true)
                    {
                        break;
                    }
                }
            }
        }
    }
    return bFind;
}

// giải phóng đạn
void BossObject::FreeBullet()
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