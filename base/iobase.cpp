#include "iobase.h"

string getstring(const long long n)
{
    stringstream newstr;
    newstr << n;
    return newstr.str();
}

long long getint(const string s)
{
    stringstream ss;
    ss << s;

    long long i;
    ss >> i;

    return i;
}

// From http://www.cnblogs.com/dfcao/p/cpp-FAQ-split.html
// Will ignore the last seperator and the last
// For example: "aaabcdaaa" seperate by "aaa" is { "", "bcd" }, "abc" seperate by "aaa" is { "abc" }
vector<string> split(const std::string& s, const std::string& c)
{
    std::vector<std::string> v;
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
    return v;
}

string GetTag(string src, string tag) throw(string)
{
    try
    {
        string reSrc;
        reSrc = split(src, "<" + tag + ">")[1];
        reSrc = split(reSrc, "</" + tag)[0];
        return reSrc;
    }
    catch (...)
    {
        throw("NoSuchTag");
    }
}

vector<string> GetTagAll(string src, string tag)
{
    vector<string> v, temp;
    string reSrc;
    temp = split(src, "<" + tag + ">");
    for (unsigned int i = 1; i < temp.size(); i++)
    {
        reSrc = split(temp[i], "</" + tag + ">")[0];
        v.push_back(reSrc);
    }
    return v;
}

#define WORKDIR "psw.fhp"

bool IOCheckPassword(string username, string password) throw(string)
{
    ifstream i(WORKDIR);
    string str, str0;
    str = "";
    while (getline(i, str0))
    {
        str += str0;
    }
    i.close();

    if (str == "")
    {
        throw("ErrorNoPasswordFile");
    }

    vector<string> infos = GetTagAll(str, "info");
    for (auto info : infos)
    {
        if (username == GetTag(info, "user") && password == GetTag(info, "psw"))
        {
            return true;
        }

    }
    return false;
}

string IOLoadInfo(string username, string password)
{
    ifstream i(WORKDIR);
    string str, str0;
    str = "";
    while (getline(i, str0))
    {
        str += str0;
    }
    i.close();

    if (str == "")
    {
        throw("ErrorNoPasswordFile");
    }

    vector<string> infos = GetTagAll(str, "info");
    for (auto info : infos)
    {
        if (username == GetTag(info, "user") && password == GetTag(info, "psw"))
        {
            return GetTag(info, "userinfo");
        }
    }
    return "None";
}

// Update the info in the password file
// 更新文件里面的记录
void IOUpdateDataAll(string username, string password, string newinfo)
{
    ifstream i(WORKDIR);
    string str, str0;
    str = "";
    while (getline(i, str0))
    {
        str += str0;
    }
    i.close();

    if (str == "")
    {
        throw("ErrorNoPasswordFile");
    }

    string target = "";

    vector<string> infos = GetTagAll(str, "info");
    for (auto info : infos)
    {
        if (username == GetTag(info, "user") && password == GetTag(info, "psw"))
        {
            target += newinfo;
        }
        else
        {
            target += "<info>";
            target += info;
            target += "</info>";
        }
    }

    ofstream out(WORKDIR);
    out << target;
    out.close();
}

int IOGetRandomBetweenZeroAndNum(int i)
{
    int tar;
    do
    {
        tar = rand() % i;
    }
    while(tar <= 0);
    return tar;
}
