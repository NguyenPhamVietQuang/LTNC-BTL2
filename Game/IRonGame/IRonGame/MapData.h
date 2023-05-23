
#ifndef MAP_DATA_H_
#define MAP_DATA_H_

#include "BaseObject.h"


//////////////////////////////////////////////////////////////////////////
// BlockMap là lớp đối tượng đại diện cho 1 ô map
// BlockMap được sử dụng lưu trữ toàn bộ danh sách các ô map đọc từ file map.
// Và BlockMap được dùng trong lớp Map
// Phân biệt 3 lớp đối tượng như sau: 
// +BlockMap là một ô map
// +Map là lớp lưu danh sách các ổ map, cho các layer, chỉ lưu trữ
// +GameMap chưa đối tượng Map, và có hành vi đọc file để lấy dữ liệu cho map
/////////////////////////////////////////////////////////////////////////
class BlockMap
{
public:
    BlockMap();
    ~BlockMap();

    void Update();
    void Render(SDL_Renderer* screen);
    void UpdateImage(SDL_Renderer* screen, std::string path);
    void RemoveTile();

    std::string getType() { return m_type; }
    BaseObject* GetTile() const { return m_tile; }

    int getXIdx() { return x_index_; }
    int getYIdx() { return y_index_; }
    int getXVal() { return xVal_; }
    int getYVal() { return yVal_; }
    int getXpMap() { return xp_map_; }
    int getYpMap() { return yp_map_; }
    int GetLayer() { return layer_; }

    void setTile(BaseObject* pTile) { m_tile = pTile; };
    void setXIdx(int x) { x_index_ = x; }
    void setYIdx(int y) { y_index_ = y; }
    void setType(std::string type) { m_type = type; }
    void setXVal(int xv) { xVal_ = xv; }
    void setYVal(int yv) { yVal_ = yv; }
    void setXpMap(int xp) { xp_map_ = xp; }
    void setYpMap(int yp) { yp_map_ = yp; }
    void SetLayer(int lay) { layer_ = lay; }

private:
    std::string m_type; // 1 ô map sẽ có mã tile đọc từ file, ví dụ G1, TR1, BRICK1, COIN
    BaseObject* m_tile; // dữ liệu ảnh của ô map đó
    int x_index_;       // index của ô map, ví dụ là ô ở dòng 20, cột 10
    int y_index_;
    int xp_map_; // vị trí của ô map
    int yp_map_;
    int xVal_;   // dùng khi muốn làm ô map có thể có hành vi, ví dụ ô máp nhảy lên, ô map vỡ vụn...
    int yVal_;
    int layer_;  // layer của ô tile
};

//////////////////////////////////////////////////////////////////////////
// lưu dữ liệu các ô tile map của các layer từ file
//
//////////////////////////////////////////////////////////////////////////
class Map
{
public:
    Map();
    ~Map();

    int getStartX() const { return start_x_; }
    int getStartY() const { return start_y_; }
    int getMaxX() const { return max_x_; }
    int getMaxY() const { return max_y_; }

    VT(VT(BlockMap*)) GetTile1() { return m_BlockList1; }
    VT(VT(BlockMap*)) GetTile2() { return m_BlockList2; }

    void SetStartX(int xs) { start_x_ = xs; }
    void SetStartY(int ys) { start_y_ = ys; }
    void SetMaxX(int mx) { max_x_ = mx; }
    void SetMaxY(int my) { max_y_ = my; }
    void UpdateXMapInfo(int xp);
    void UpdateYMapInfo(int yp);
    void SetFileMap(std::string filemap) { file_name_ = filemap; }//std::string file_name_;
    void AddList1(VT(BlockMap*) list) { m_BlockList1.push_back(list); }
    void AddList2(VT(BlockMap*) list) { m_BlockList2.push_back(list); }
    void RemoveList(int layer);
private:
    int start_x_;  // vị trí start_x của map
    int start_y_;
    int max_x_;  // tổng chiều dài map
    int max_y_;
    std::string file_name_;
    VT(VT(BlockMap*)) m_BlockList1;
    VT(VT(BlockMap*)) m_BlockList2;
};

#endif