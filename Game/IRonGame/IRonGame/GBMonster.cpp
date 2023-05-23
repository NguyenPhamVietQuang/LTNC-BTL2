
#include "stdafx.h"
#include "GBMonster.h"

GBMonster::GBMonster()
{
    gb_type_ = GB_MOVING;
    type_ = TH_GB_MONSTER;
    x_plus_ = 0;
}


GBMonster::~GBMonster()
{
    FreeBullet();
}

//////////////////////////////////////////////////////////////////////////
// khởi tạo thông số ban đầu cho monster
//////////////////////////////////////////////////////////////////////////
void GBMonster::Init(int type, bool bClip, int xTile, int yTile,
                     SDL_Renderer* screen)
{
    // sử dụng kỹ thuật delay frame: (đọc lớp bossObject để hiểu rõ hơn)
    this->set_is_clip(bClip);

    // set vị trí
    this->set_xpos(xTile * TILE_SIZE);
    this->set_ypos(yTile * TILE_SIZE);

    // set loại
    this->gb_type_ = type;

    // load ảnh cho từng loại
    if (gb_type_ == GB_MOVING)
    {
        this->LoadImg(sThreadMoving, screen);
    }
    else if (gb_type_ == GB_SHOT)
    {
        this->LoadImg(sThreadShot, screen);
    }
    else if (gb_type_ == GB_FIRE)
    {
        this->LoadImg(sThreadFire, screen);
    }
}

//////////////////////////////////////////////////////////////////////////
// xử lý rơi theo trọng lực
//////////////////////////////////////////////////////////////////////////
void GBMonster::DoAction()
{
    if (is_alive_ == true)
    {
        y_val_ += GRAVITY_SPEED;
        if (y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }

        CheckToMap();
    }
}

//////////////////////////////////////////////////////////////////////////
// xử va chạm với map
//////////////////////////////////////////////////////////////////////////
void GBMonster::CheckToMap()
{
    if (on_ground_ == true)
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

void GBMonster::DoLeft()
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

void GBMonster::DoRight()
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

void GBMonster::DoDown()
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

//////////////////////////////////////////////////////////////////////////
// Hàm xử lý trạng thái biến đổi của monster
//
//////////////////////////////////////////////////////////////////////////
void GBMonster::Update()
{
    if (on_ground_ == 1)
    {
        // với loại monster di chuyển
        if (gb_type_ == GB_MOVING)
        {
            // di chuyển theo 2 chiều khác nhau
            if (v_dir_ == 1)
            {
                // ví dụ về bên trái
                x_val_ = -3;
            }
            else if (v_dir_ == -1)
            {
                // về bên phải
                x_val_ = 3;
            }

            // mỗi lần di chuyển, sẽ tính toán tổng lượng thay đổi
            x_plus_ += 3;

            // nếu tổng lượng thay đổi đó lớn hơn 192 thì sẽ đổi chiều
            // nghĩa là monster đi về bên trái một lượng giá trị = 192
            // thì nó sẽ quay ngược lại đi về bên phải
            if (x_plus_ >= 192)
            {
                v_dir_ *= -1;
                if (v_dir_ == -1)
                {
                    // và khi thay đổi chiều di chuyển
                    // thì thay đổi thông số flip của ảnh
                    // để anh được đảo chiều ngược lại khi hiển thị
                    this->SetIsFlip(true);
                }
                else
                {
                    this->SetIsFlip(false);
                }

                x_plus_ = 0;
            }

            // Dùng cho các monster di chuyển 1 chiều liên tục
            // khi vượt quá bản đồ, thì coi như đã chết
            // ví dụ game nấm mario, con goombasss là 1 kiểu như vậy
            if (x_pos_ + width_frame_ < 0 ||
                x_pos_ > pMap_->getMaxX())
            {
                is_alive_ = false;
            }
        }
    }
}

void GBMonster::Show(SDL_Renderer* des)
{
    Update();
    ThreatsObject::Show(des);
}

//////////////////////////////////////////////////////////////////////////
// Thêm bullet cho monster loại shot
//
//////////////////////////////////////////////////////////////////////////
void GBMonster::AddBullet(SDL_Renderer* screen)
{
    BulletObject* pBul = new BulletObject();
    //BulletObject* pBul = NULL;
    if (gb_type_ == GB_SHOT)
    {
        //pBul = new BulletObject();

        // hướng từ phải sang trái
        pBul->set_dir_bullet(BulletObject::DIR_LEFT);

        // loại laser
        pBul->Init(kImgBulletLaser, screen);

        // vị trí phát ra đạn
        int xBul = x_pos_;
        int yBul = y_pos_ + height_frame_*0.4;

        pBul->set_xy_pos(xBul, yBul);

        // tốc độ 8
        pBul->set_x_val(8);

        m_BulletList.push_back(pBul);
    }
}

void GBMonster::DoBullet(SDL_Renderer* screen, bool is_pause /*false*/)
{
    if (is_alive_ == false)
        return;

    // Thực hiện bắn đạn
    if (gb_type_ == GB_SHOT)
    {
        for (size_t iBul = 0; iBul < m_BulletList.size(); iBul++)
        {
            BulletObject* pBul = m_BulletList.at(iBul);
            if (pBul != NULL)
            {
                if (pBul->get_is_move() == true)
                {
                    // Khi đạn cho phép di chuyển, thì phạm vi đang bắn là 500 từ vị trí nó phát ra
                    int xLimit = x_pos_ - 500;

                    // nếu chế độ pause game được bật, thì ko xử lý bắn đạn
                    if (is_pause == false)
                    {
                        pBul->HandelMove(xLimit, SCR_HEIGHT);
                    }

                    // show đạn
                    pBul->Show(screen);
                }
                else
                {
                    // khi đạn đi hết phạm vi 500 hoặc va chạm với player
                    // nó tạm sét di chuyển = false

                    // sau đó nó được xét lại vị trí ban đầu
                    int xBul = x_pos_;
                    int yBul = y_pos_ + height_frame_*0.4;

                    pBul->set_xy_pos(xBul, yBul);


                    // và bắt đầu tái xuát hiện trở lại
                    // cách làm này mục đích tái sử dụng tài nguyên
                    // không muốn thêm xử lý xóa, rồi lại new
                    pBul->set_is_move(true);
                }
            }
        }
    }
    
}

// giải phóng đạn
void GBMonster::FreeBullet()
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