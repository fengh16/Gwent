#include "DataControl.h"
#include <stdlib.h>
#include "CONST.h"

vector<Card*> GetListFromData(string src) throw(string)
{
    // Split the data by card
    // 把不同卡片的数据分隔开，并且去掉限制的标签<Card></Card>
    vector<Card*> reVec;
    vector<string> splited = split(src, "</Card><Card>");
    splited[0] = split(splited[0], "<Card>")[0]; // 这个函数里面没有把最开头的空看做一个部分
    splited[splited.size() - 1] = split(splited[0], "</Card>")[0];

    for (auto str : splited)
    {
        int id = atoi(GetTag(str, "Type").c_str());
        if (id <= cCard_Base || id >= cCard_Num)
        {
            throw("Card_Id_OUT_OF_RANGE");
        }
        Card* p = GetNewCard(id);
        reVec.push_back(p);
    }
    return reVec;
}

string SaveDataFromList(vector<Card*> src)
{
    string reStr = "";
    for (auto p : src)
    {
        reStr += "<Card><Type>";
        reStr += getstring(p->GetIndex());
        reStr += "</Type></Card>";
    }
    return reStr;
}
