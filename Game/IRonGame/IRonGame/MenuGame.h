

#ifndef MENU_GAME_H_
#define MENU_GAME_H_

#include "g_define.h"
#include "BaseObject.h"
#include "Geometric.h"
#include "TextObject.h"

//////////////////////////////////////////////////////////////////////////
// Các lớp quản lý Màn hình menu game
// 
//////////////////////////////////////////////////////////////////////////
class MenuGame
{
public:
	MenuGame(bool is_bkgn = true);
	~MenuGame();

	enum MenuTyle
	{
		MENU_START = 200,
		MENU_PAUSE,
		MENU_END,
	};

	virtual void Render(SDL_Renderer* screen);
	virtual void MenuAction(SDL_Event events, SDL_Renderer* screen);
	virtual void LoadBkgn(const char* file, SDL_Renderer* screen);

	void Free();
	bool OnRect(int x, int y, SDL_Rect& rect);
	void SetIsBkgn(bool is_bkgn) { m_isDrawBkgn = is_bkgn; }
	void SetIsSelect(int idx) { m_Select = idx; };
	int GetSelect() { return m_Select; }
protected:
	int m_Type;
	BaseObject m_Background;
	bool m_isDrawBkgn;
	int m_Select;
};

//////////////////////////////////////////////////////////////////////////
// kế thừa từ menugame
//
//
//////////////////////////////////////////////////////////////////////////
class MenuGameStart : public MenuGame
{
public:
	MenuGameStart();
	~MenuGameStart();
	void SetImgOptionList(const VT(BaseObject*)& img_list);
	void FreeData();
	void Render(SDL_Renderer* screen);
	void MenuAction(SDL_Event events, SDL_Renderer* screen);
    void InitOption(SDL_Renderer* screen);
    int GetOption() { return m_option; }
private:
	VT(BaseObject*) m_ImgOptionList;
    int m_option;
    BaseObject  m_Img1;
    BaseObject m_Img2;
    GeoFormat m_optionBound;
};


//////////////////////////////////////////////////////////////////////////
// kế thừa từ menugame
// ko dùng background
//
//////////////////////////////////////////////////////////////////////////
class MenuGamePause : public MenuGame
{
public:
	MenuGamePause();
	~MenuGamePause();
	void SetImgOptionList(const VT(BaseObject*)& img_list);
	void FreeData();
	void Render(SDL_Renderer* screen);
	void MenuAction(SDL_Event events, SDL_Renderer* screen);
	void InitFrameGeo();
private:
	VT(BaseObject*) m_ImgOptionList;
	GeoFormat m_Rect;
	GeoFormat m_RectOutLine;
};

//////////////////////////////////////////////////////////////////////////
// kế thừa từ menugame
// Có dùng background
//
//////////////////////////////////////////////////////////////////////////
class MenuGameEnd : public MenuGame
{
public:

	enum EndLessonState
	{
		GA_NONE = 200,
		GA_WIN,
		GA_LOSE,
		GA_END_GAME
	};

	MenuGameEnd();
	~MenuGameEnd();

	void SetImgOptionList(const VT(BaseObject*)& img_list);
	void FreeData();
	void Render(SDL_Renderer* screen);
	void MenuAction(SDL_Event events, SDL_Renderer* screen);
	void SetTextContent(std::string sData, int idx);
	void AddTextObj(TextObject* obj) { m_TextList.push_back(obj); }
    void RemoveTextList();
	void SetFont(TTF_Font* font) { m_Font = font; }
	void SetGameState(Uint16 state) { m_State = state; }
	int GetGameState() {return m_State;}
    void SetMarkList(std::vector<int> mList) { m_markList = mList; }
    void MakeMarkText();
    void SetMarkCurrent(int mark) { m_MarkCurrent = mark; };
private:
	VT(BaseObject*) m_ImgOptionList;
	VT(TextObject*) m_TextList;
	TTF_Font* m_Font;
	Uint16 m_State;
    std::vector<int> m_markList;
    int m_MarkCurrent;
};
#endif

