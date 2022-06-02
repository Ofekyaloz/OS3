#ifndef UNTITLED_BQ_H
#define UNTITLED_BQ_H
#include <string>
#include <vector>
#include <sys/sem.h>
using namespace std;

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};

class BQ {
    vector<string> queue;
    int _size;
    int _max_size;
    int semid;
    union semun semarg;
    struct sembuf sops[1];

public:
    BQ(int max_size);
    int enqueue(const string& s);

    string dequeue();

};

#endif //UNTITLED_BQ_H
