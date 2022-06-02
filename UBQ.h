#ifndef UNTITLED_UBQ_H
#define UNTITLED_UBQ_H
#include "BQ.h"

using namespace std;


class UBQ {
    vector<string> queue;
    int _size;
    int semid;
    union semun semarg;
    struct sembuf sops[1];

public:
    UBQ();
    int enqueue(const string& s);

    string dequeue();

};


#endif //UNTITLED_UBQ_H
