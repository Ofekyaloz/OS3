//
// Created by ofek on 5/30/22.
//

#ifndef UNTITLED_PRODUCER_H
#define UNTITLED_PRODUCER_H
#include <string>

using namespace std;

class Producer {
    int _id;
    int _products_number;
    int _buf_size;

public:
    Producer(int id, int products_num, int buf_size);

    void Produce();
};


#endif //UNTITLED_PRODUCER_H
