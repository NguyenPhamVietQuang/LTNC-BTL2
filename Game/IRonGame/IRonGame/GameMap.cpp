
#include "stdafx.h"
#include <fstream>

#include "GameMap.h"


GameMap* GameMap::instance_ = NULL;

GameMap::GameMap()
{
    game_map_ = new Map();
    m_path = "image\\map";
    map_num_h = 0;
    map_num_w = 0;
}

GameMap::~GameMap()
{
    //hàm hủy
}

void GameMap::DestroyInst()
{
    if (game_map_ != NULL)
    {
        delete game_map_;
        game_map_ = NULL;
    }

    if (instance_ != NULL)
    {
        delete instance_;
        instance_ = NULL;
    }
}

std::string GameMap::GetPathMapName()
{
    return m_path;//m_path = "image\\map";dòng 13
}

void GameMap::LoadMap()
{
    // map thiết kế là map 2 lớp layer
    // do đó xử lý đọc 2 file map
    std::string mapPath = GetPathMapName() + "\\map.tmp";
    std::string mapPath2 = GetPathMapName() + "\\map_2.tmp";;

    ReadMap(mapPath, 0);
    ReadMap(mapPath2, 1);
}

void GameMap::ReadMap(std::string file, int layer)
{
    const char *wName = file.c_str();//dữ liệu file đầu vào chuyển thành con trỏ kiểu char theo yêu cầu hàm Load,open,... 
    std::fstream fsFile;
    fsFile.open(wName, std::ios::in);//mở file

    int rIndex = 0;//chỉ số hàng
    int cIndex = 0;

    // File map là file dạng txt, với 400 cột và 10 hàng
    // thực hiện đọc dữ liệu file để tìm ra các mã tile.
    if (fsFile.is_open() == true)//chỉ số khác 0
    {
        bool bStartData = false;
        while (!fsFile.eof())//Duyệt qua từng dòng của tệp cho đến khi kết thúc tệp
        {
            std::string strData;
            std::getline(fsFile, strData);
            if (bStartData == false)
            {
                int index = strData.find(TL_DATA);//tìm tứ khóa "data" trong hai bản đồ map_2.tmp và map.tmp
                if (index >= 0)//Nếu không tìm thấy, find sẽ trả về giá trị std::string::npos thường có giá trị là -1 
                {
                    bStartData = true;//nếu có tìm thấy nghĩa là file đã đúng thì bắt đầu quét file khác và trả bStartData về true
                    continue;
                }
            }
            else//nếu qua lượt quét vẫn là false
            {
                if (strData.empty() == false)//thực hiện khi chuỗi này không rỗng
                {
                    // mỗi dòng line trong file text sẽ có 400 mã tile
                    // dùng hàm SplitBySpace để tách ra từng mã
                    std::vector<std::string> tile_list = TBase::SplitBySpace(strData);//tách ra bởi dấu cách rồi lưu trong tile_list
                    VT(BlockMap*) xTemp;//vector kiểu con trỏ BlockMap
                    for (int i = 0; i < tile_list.size(); ++i)
                    {
                        // Với mỗi mã tìm được, sẽ tạo một đối tượng blockmap
                        // dại diện cho 1 ô tile map
                        std::string strTile = tile_list.at(i);
                        BlockMap* pBlock = new BlockMap();//Tạo một đối tượng mới BlockMap và gán cho con trỏ pBlock.
                        int cx = rIndex*TILE_SIZE;
                        int cy = cIndex*TILE_SIZE;

                        // block map có tên layer, loại tile, vị trí hiển thị
                        pBlock->SetLayer(layer);//xét là layer 0 hay 1 ?
                        pBlock->setType(strTile);//xét kiểu std::string m_type; // 1 ô map sẽ có mã tile đọc từ file, ví dụ G1, TR1, BRICK1, COIN 
                        pBlock->setYIdx(cy);
                        pBlock->setXIdx(cx);//void setXIdx(int x) { x_index_ = x; } int x_index_;  index của ô map, ví dụ là ô ở dòng 20, cột 10
                        xTemp.push_back(pBlock);//sau khi cập nhật xong data cho con trỏ kiểu BlockMap là bBlock thì cập nhật vào vector chứa con trỏ là xTemp chỉ tới đối tượng BlockMap
                        rIndex++;
                        //tổng quan pBlock sẽ truy cập vào các hàm trên có class BlockMap và trả về kiểu dữ liệu yêu cầu
                    }

                    if (layer == 0)
                    {
                        game_map_->AddList1(xTemp);//void AddList1(VT(BlockMap*) list) { m_BlockList1.push_back(list); }
                       
                    }
                    else
                    {
                        game_map_->AddList2(xTemp);
                    }

                    map_num_w = rIndex;
                    rIndex = 0;
                    cIndex++;
                    //mã cập nhật giá trị map_num_w để theo dõi chiều rộng của bản đồ (số lượng ô tile theo chiều ngang) và đặt lại giá trị rIndex về 0. Tiếp theo, mã tăng giá trị cIndex để xử lý hàng tiếp theo của bản đồ.
                }
            }
        }
    }

    map_num_h = cIndex;
    //Gán giá trị của cIndex cho map_num_h để lưu trữ chiều cao của bản đồ dưới dạng số lượng ô tile theo chiều dọc.


    // sau khi load toàn bộ số lượng tile map trong file map
    // lưu giá trị chiều dài, rộng toàn bộ bản đồ game
    game_map_->SetMaxX(map_num_w*TILE_SIZE); //void SetMaxX(int mx) { max_x_ = mx; } int max_x_
    game_map_->SetMaxY(map_num_h*TILE_SIZE);

    // vi trí ban đầu của map với màn hình là 0, 0
    // khi nhân vật chạy, màn hình giữ nguyên, máp bị cuốn chiếu
    // giá trị start_x, start_y sẽ được tăng dần.
    game_map_->SetStartX(0);//void SetStartX(int xs) { start_x_ = xs; }
    game_map_->SetStartY(0);
    fsFile.close();//đóng file
    if (layer == 0)
    {
        game_map_->SetFileMap(file);//void SetFileMap(std::string filemap) { file_name_ = filemap; }//std::string file_name_;
    }//Đoạn mã này kiểm tra nếu layer bằng 0, tức là đang xử lý lớp đầu tiên của bản đồ (background). Nếu đúng, hàm SetFileMap được gọi để lưu trữ tên tệp bản đồ (filemap) vào biến file_name_ của đối tượng game_map_.
}

//////////////////////////////////////////////////////////////////////////
// Sau khi đã đọc được mã tile của từng ô trong bản đồ map
// thì sử dụng mã đó tìm file ảnh có tên tương ứng với mã để load dữ liệu
// Hàm này thực hiện nhiệm vụ đó
//////////////////////////////////////////////////////////////////////////
void GameMap::LoadMapTiles(SDL_Renderer* screen, int layer)
{
    char* filename = NULL;
    VT(VT(BlockMap*)) dataMapList;
    if (layer == 0)
    {
        dataMapList = game_map_->GetTile1(); //VT(VT(BlockMap*)) GetTile1() { return m_BlockList1; } VT(VT(BlockMap*)) m_BlockList1;
    }
    else
    {
        dataMapList = game_map_->GetTile2();
    }
    //VT(VT(BlockMap*)) dataMapList một vector chứa các vector con, và mỗi vector con chứa các con trỏ đến đối tượng BlockMap.
    std::string path_img = GetPathMapName() + std::string("\\");//GetPathMapName() return m_path(string)
    int nRow = dataMapList.size();

    // Đọc theo từng hàng
    for (int y = 0; y < nRow; ++y)
    {

        VT(BlockMap*) dataMaps = dataMapList[y];//Ở mỗi lần lặp của vòng lặp ngoài (với biến lặp y), dataMaps được gán bằng phần tử thứ y của dataMapList.vector con của dataMapList
        int nCol = dataMaps.size();

        // đọc theo từng cột
        for (int x = 0; x < nCol; ++x)
        {
            BlockMap* pBlock = dataMaps[x];//ta hiểu ở đây là phần tử kiểu con trỏ BlockMap của ma trận vector dataMapList
            if (pBlock != NULL)
            {
                // tìm được mã tile
                std::string type = pBlock->getType();//std::string getType() { return m_type; }
                //  file ảnh  = mã tile + đuôi ảnh (png, bitmap..)
                std::string tile_path = path_img + type + IMAGE_EXT;//".bmp";
                filename = const_cast<char*>(tile_path.c_str());//?

                // mỗi tile ảnh là một baseobject
                BaseObject* pTile = new BaseObject();//ta tạo mỗi file ảnh với những dữ liệu nguyên thủy của 1 baseobj

                // load ảnh và lưu dữ liệu ảnh vào blockMap
                bool ret = pTile->LoadImg(filename, screen);//load ảnh với dữ liệu của ảnh ở trong baseobject
                if (ret)
                {
                    pBlock->setTile(pTile);//void setTile(BaseObject* pTile) { m_tile = pTile; };BaseObject* m_tile;
                    //m_tile sẽ chứa thông tin về hình ảnh (image) của ô bản đồ đó hay pBlock sẽ nhận dữ liệu từ BaseObject
                }
            }
        }
    }
}

void GameMap::ResetMap(SDL_Renderer* screen)
{
    game_map_->RemoveList(0);
    game_map_->RemoveList(1);

    LoadMap();
    LoadMapTiles(screen, 0);
    LoadMapTiles(screen, 1);
}

//////////////////////////////////////////////////////////////////////////
// Hàm vẽ map
// Khi dữ liệu lưu ảnh đã đầy đủ (400 cột, 10 hàng)
// mã tile đã có, dữ liệu load ảnh tile cũng đã xong
// thì bắt đầu vẽ map đó lên màn hình
//////////////////////////////////////////////////////////////////////////
void GameMap::DrawMap(SDL_Renderer* des, int layer)
{
    VT(VT(BlockMap*)) blockList;
    if (layer == 0)
    {
        blockList = game_map_->GetTile1();
    }
    else
    {
        blockList = game_map_->GetTile2();
    }

    if (blockList.empty() == true && map_num_w > 0 && map_num_h > 0)
    {
        return;
    }

    // Do bản đồ game kéo dài từ 0 đến 400*TILE_SIZE (25600)
    // Trong khi màn hình chỉ có chiều dài = 1280
    // Do đó chỉ số lượng ảnh tile có vị trí phù hợp trong phạm vi màn hình mới được hiển thị
    // Vi trí màn hình sẽ bắt dầu từ: start_x đến start_x + 1280
    // Do chiều cao = 10*64 = 640, vẫn nhỏ hơn chiều cao màn hình nên có thể hiện thị ful về chiều cao
    // Khi mới khởi động start_x = 0, do đó hiển thị từ 0 đến 1280
    // Nhưng khi nhân vật di chuyển đến giữa bản đồ, thì các tile đầu bản đồ ko thể hiển thị được nữa.
    // do đó cần giá trị start_x để biết được bản đồ đã bị cuốn chiều là bao nhiêu
    // ví dụ start_x = 256, nghĩa là, bản đồ đã bị kéo 256.
    // 256/ 64 = 4, như vậy có nghĩa là bản đồ phải hiển thị từ các tile ở cột 4 trở đi.
    // xử lý cả x,y cho tổng quát(vì có thể làm map game kiểu dọc)

    int start_x = game_map_->getStartX();  // tìm ra trị start_x, start_y
    int start_y = game_map_->getStartY();

    int map_x = start_x / TILE_SIZE;  // tìm ra số tile bắt đầu hiển thị
    int map_y = start_y / TILE_SIZE;

    // ko phải lúc nào start_x chia cho tile_size cũng ra tròn số
    // do đó map_x, map_y là 
    int x1 = (start_x % TILE_SIZE);
    int y1 = (start_y % TILE_SIZE);

    // tìm min giữa scren_width mà chiều dài bản đồ game
    // vì có thể loại game chiều dài game chưa hết màn hình

    int limitX = min(SCR_WIDTH, map_num_w*TILE_SIZE);
    int limitY = min(SCR_HEIGHT, map_num_h*TILE_SIZE);

    // Tọa độ vị trí của các ô tile map sẽ chạy theo giá trị từ 0 đến chiều rộng màn hình 
    for (int y_pos = 0; y_pos < limitY; y_pos += TILE_SIZE)
    {
        for (int x_pos = 0; x_pos <= limitX; x_pos += TILE_SIZE)
        {
            // với mỗi vòng lặp, x_pos đều tằng 1 lượng tile_size
            // do đó map_x sẽ được tăng lên 1
            if (map_x < map_num_w && map_y >= 0)
            {
                // với mỗi giá trị map_x, map_y sẽ tìm tile tương ứng
                BlockMap* pBlock = blockList.at(map_y).at(map_x);
                // nếu block map tại vi trí đó có tile khác NULL (có hình ảnh)
                if (pBlock != NULL && pBlock->GetTile() != NULL)
                {
                    // thì thực hiện vẽ map tại vị trí x_pos, y_pos và trừ đi lượng số dư tính toán ở trên
                    pBlock->setXpMap(x_pos - x1);
                    pBlock->setYpMap(y_pos - y1);
                    pBlock->Update();
                    pBlock->Render(des);
                }
                map_x++;
            }
        }

        // sau mỗi hàng, thì map_x lại được tính lại ban đầu
        map_x = game_map_->getStartX() / TILE_SIZE;

        // hàng chuyển sang kế tiếp
        map_y++;
    }
}

//////////////////////////////////////////////////////////////////////////
// kiểm tra tile map có phải đối tượng ngoại cảnh ko
//
//////////////////////////////////////////////////////////////////////////
bool GameMap::CheckSkipMap(const std::string& tile)
{
    bool bRet = false;
    int nSize = sizeof(SkipMap) / sizeof(SkipMap[0]);
    for (int i = 0; i < nSize; ++i)
    {
        std::string sType = SkipMap[i];
        std::string sInput(tile);
        if (sInput == sType)
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

// kiểm tra tile có phải tiền, kim cương hay đồ ăn mà bạn tự định nghĩa ra
int GameMap::GetValueProduct(const std::string& tile)
{
    int val = 0;
    if (tile == "m1")
    {
        val = 20;
    }
    else if (tile == "dia1")
    {
        val = 50;
    }
    else if (tile == "dia2")
    {
        val = 100;
    }
    return val;
}