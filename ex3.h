#ifndef UNTITLED_EX3_H
#define UNTITLED_EX3_H

#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cctype>
#include "Producer.h"
#include "BQ.h"
#include <vector>
#include "mutex"
#include "UBQ.h"

using namespace std;

vector<BQ *> queues;

BQ* editorsBQ;

UBQ* sportsUBQ = new UBQ();
UBQ* newsUBQ = new UBQ();
UBQ* weatherUBQ = new UBQ();


#endif //UNTITLED_EX3_H
