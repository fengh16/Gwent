#ifndef DATACONTROL_H
#define DATACONTROL_H

#include "iobase.h"
#include <vector>
#include <string>
#include "Cards.h"

using namespace std;

extern vector<Card*> GetListFromData(string src) throw(string);
extern string SaveDataFromList(vector<Card*> src);

#endif
