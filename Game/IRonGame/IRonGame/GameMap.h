
#ifndef GAME_MAP_H_
#define GAME_MAP_H_

//////////////////////////////////////////////////////////////////////////
/*
Lớp đại diện chung cho xử lý bản đồ game
nó bao gồm biến quản lý danh sách dữ liệu map
xử lý các hành vi đọc map
*/
//////////////////////////////////////////////////////////////////////////
#include "g_define.h"
#include "MapData.h"
#include "TBase.h"


// Khi xây dựng bản đồ, có các đối tượng chỉ là ngoại cảnh, như cây cối, núi non
// các đối tượng này chỉ làm đẹp game, ko tham gia vào quá trình xử lý va chạm
// Do đó chúng được lưu vào biến skipMap
const std::string SkipMap[] = { "sr1", "sr2", "sr3", "tr1", "tr2", "TR11",
                                "TR12","TR13", "TR14", "sg2"};

class GameMap
{
public:
  GameMap();
  ~GameMap();

  static GameMap* GetInstance()
  {
      if (instance_ == NULL)
          instance_ = new GameMap();
      return instance_;
  }

  void DestroyInst(); // giải phóng đối tượng map
  void SetPath(std::string path) { m_path = path; }

  // hàm loadMap
  void LoadMap();
  void ReadMap(std::string file, int layer); // dọc dữ liệu từ file map theo layer

  void DrawMap(SDL_Renderer* des, int layer);  // vẽ toàn bộ bản đồ map
  void LoadMapTiles(SDL_Renderer* screen, int layer); // đọc các file ảnh cho từng ô tile, dữ liệu đó sẽ dùng trong hàm DraMap

  void SetMap(Map* gMap) {game_map_ = gMap;}
  void ResetMap(SDL_Renderer* screen); // xóa dữ liệu máp

  Map* GetMap() const { return game_map_; }
  std::string GetPathMapName();

  int GetHNum() { return map_num_h; }
  int GetWNum() { return  map_num_w; }

  bool CheckSkipMap(const std::string& tile);  // kiểm tra ô tile có phải đối tượng bỏ qua hay ko
  int GetValueProduct(const std::string& tile); // kiểm tra ô tile có phải coin, kim cương hay ko và trả về giá trị của nó
public:
  Map* game_map_;   // đối đại diện cho danh sách các khối map game
private:
    static GameMap* instance_;
    int map_num_h;  // số lượng ô tile map chiều dọc
    int map_num_w;  // số lượng ô til map chiều ngang
    std::string m_path;  // tên bản đồ map
};

#endif