


#ifndef PLAYER_BLOOD_H_
#define PLAYER_BLOOD_H_
#include "g_define.h"
#include "BaseObject.h"

///////////////////////////
// Lớp máu cho player
//
////////////////////////////

class PlayerBlood
{
public:
	PlayerBlood();
	~PlayerBlood();
	int GetCount() { return m_Count; }

	void MinusUpdate(); 
	void Init(SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	bool IsEmpty();
	void FreeData();
    void ReStart(SDL_Renderer* screen);
private:
	int m_Count;
	VT(BaseObject*) m_BloodList;
};


#endif

