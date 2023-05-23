// IRonGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameMain.h"

#undef main
////////////////////////////////
// Hàm chính của 1 chương trình c++
//
///////////////////////////////
int main()
{
    // Gọi hàm Init của lớp main để xây dựng màn hình windows cho game
    bool bInit = GameMain::GetInstance()->Init();

    // Gọi hàm init data để khởi tạo các thông số ban đầu cho các đối tượng game
    bool bInitData = GameMain::GetInstance()->InitData();
    if (bInit && bInitData)
    {
        // Nếu Hàm init và hàm initdata không có lỗi gì xảy ra
        // Thực hiện gọi chương trình loop để chạy game
        GameMain::GetInstance()->Loop();
    }

    // Khi kết thúc game thì thực hiện giải phóng vùng nhớ
    GameMain::GetInstance()->Close();
    return 0;
}

