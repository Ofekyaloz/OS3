//
// Created by ofek on 5/30/22.
//

#ifndef UNTITLED_UBQ_H
#define UNTITLED_UBQ_H
#include <string>
#include <vector>
using namespace std;

class UBQ {
    vector<string> queue;
    int _size;
    int _max_size;

public:
    UBQ(int max_size);
    void enqueue(string s);

    string dequeue();

    int getSize() const;
};


#endif //UNTITLED_UBQ_H
