
#include "stdafx.h"
#include "PlayerBlood.h"


PlayerBlood::PlayerBlood()
{
	m_Count = 6;
}


PlayerBlood::~PlayerBlood()
{

}

//////////////////////////////////////////////////////////////////////////
// Init máu
//////////////////////////////////////////////////////////////////////////
void PlayerBlood::Init(SDL_Renderer* screen)
{
    VT(std::string) path_list = { "image\\blood\\blo_1.png",
                                    "image\\blood\\blo_2.png",
                                    "image\\blood\\blo_3.png",
                                    "image\\blood\\blo_4.png",
                                    "image\\blood\\blo_5.png" };

    int xp = 10;
    m_Count = path_list.size();
    for (int i = 0; i < m_Count; i++)
    {
        BaseObject* pObj = new BaseObject();
        bool ret = pObj->LoadImg(path_list[i], screen);
        xp = xp + pObj->GetRect().w + 2;
        int yp = (FRAME_H - 5) - pObj->GetRect().h;

        pObj->SetRect(xp, yp);
        m_BloodList.push_back(pObj);
    }
}

// hiển thị máu
void PlayerBlood::Show(SDL_Renderer* screen)
{
	for (auto b : m_BloodList)
	{
		b->Render(screen);
	}
}

// giảm máu 1 đơn vị
void PlayerBlood::MinusUpdate()
{
	int size = m_BloodList.size();

	if (size > 0)
	{
		BaseObject* pObj = m_BloodList.at(size - 1);
		pObj->Free();
		m_BloodList.pop_back();
        m_Count--;
	}

    if (m_Count < 0)
    {
        m_Count = 0;
    }
}

bool PlayerBlood::IsEmpty()
{
	if (m_BloodList.size() == 0)
	{
		return true;
	}

	return false;
}

void PlayerBlood::FreeData()
{
	if (IsEmpty() == false)
	{
		for (auto x : m_BloodList)
		{
			x->Free();
		}

		m_BloodList.clear();
	}
}


void PlayerBlood::ReStart(SDL_Renderer* screen)
{
    FreeData();
    Init(screen);
}