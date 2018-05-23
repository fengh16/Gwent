#ifndef IOBASE_H
#define IOBASE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "DataControl.h"
#include <time.h>
#include <stdlib.h>

using namespace std;

string getstring(const long long n);

long long getint(const string s);

// From http://www.cnblogs.com/dfcao/p/cpp-FAQ-split.html
// Will ignore the last seperator and the last
// For example: "aaabcdaaa" seperate by "aaa" is { "", "bcd" }, "abc" seperate by "aaa" is { "abc" }
vector<string> split(const std::string& s, const std::string& c);

string GetTag(string src, string tag) throw(string);

vector<string> GetTagAll(string src, string tag);

bool IOCheckPassword(string username, string password) throw(string);

string IOLoadInfo(string username, string password);

void IOUpdateDataAll(string username, string password, string newinfo);

int IOGetRandomBetweenZeroAndNum(int i);

#endif // IOBASE_H
