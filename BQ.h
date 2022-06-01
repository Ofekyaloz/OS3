#ifndef UNTITLED_BQ_H
#define UNTITLED_BQ_H
#include <string>
#include <vector>
using namespace std;

class BQ {
    vector<string> queue;
    int _size;
    int _max_size;

public:
    BQ(int max_size);
    int enqueue(string s);

    string dequeue();

    int getSize() const;
};


#endif //UNTITLED_BQ_H
