//
// Created by ofek on 6/1/22.
//

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

#define SIZE 150
using namespace std;

vector<BQ *> queues;

BQ* editorsBQ;

UBQ* sportsUBQ = new UBQ(10);
UBQ* newsUBQ = new UBQ(10);
UBQ* weatherUBQ = new UBQ(10);


#endif //UNTITLED_EX3_H
