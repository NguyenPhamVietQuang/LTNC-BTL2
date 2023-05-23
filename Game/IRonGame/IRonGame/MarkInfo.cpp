
#include "stdafx.h"
#include <fstream>
#include "MarkInfo.h"


MarkInfo::MarkInfo()
{
    m_mark = 0;
}


MarkInfo::~MarkInfo()
{
}



void MarkInfo::LoadMark()
{
    std::vector<int> total_mark_list;

    const char *wName = "image//mark_log.ini";
    std::fstream fsFile;
    fsFile.open(wName, std::ios::in);
    std::string sName = "mark=";
    if (fsFile.is_open() == true)
    {
        while (!fsFile.eof())
        {
            std::string strData;
            std::getline(fsFile, strData);
            if (strData.empty() == false)
            {
                int pos = strData.find(sName);
                if (pos >= 0)
                {
                    strData.replace(pos, 5, "");

                    if (strData.empty() == false)
                    {
                        int mark_value = stoi(strData);
                        total_mark_list.push_back(mark_value);
                    }
                }
            }
        }
        fsFile.close();
    }

    std::vector<int> rev_list;
    for (int i = (total_mark_list.size() - 1); i >= 0; i--)
    {
        rev_list.push_back(total_mark_list[i]);
    }

    int nSize = rev_list.size();
    if (nSize >= 5)
    {
        nSize = 5;
    }


    if (mark_list.empty() == false)
    {
        mark_list.clear();
    }

    for (int ii = 0; ii < 5; ii++)
    {
        int val = rev_list[ii];
        mark_list.push_back(val);
    }
}

void MarkInfo::SaveMark()
{
    std::ofstream outfile;
    outfile.open("image//mark_log.ini", std::ios_base::app);

    std::string sData = "mark=";
    std::string sMark = std::to_string(m_mark);
    sData += sMark;
    outfile << "\n";
    outfile << sData;
    outfile.close();
}