
#ifndef MARK_INFO_H_
#define MARK_INFO_H_

#include "TBase.h"
#include <vector>

class MarkInfo
{
public:

    MarkInfo();
    ~MarkInfo();
    void SetMark(int mark) { m_mark = mark; mark_list.push_back(mark); }
    void LoadMark();
    void SaveMark();
    std::vector<int> GetMarkList() { return mark_list; }
private:
    std::vector<int> mark_list;
    int m_mark;
};


#endif
