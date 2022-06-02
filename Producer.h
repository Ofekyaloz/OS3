//
// Created by ofek on 5/30/22.
//

#ifndef UNTITLED_PRODUCER_H
#define UNTITLED_PRODUCER_H
#include <string>
#include "BQ.h"

using namespace std;

class Producer {
    int _id;
    int _products_number;
    int _buf_size;
    BQ* _bq;

public:
    Producer(int id, int products_num, int buf_size);

    int getId();

    int getProducts();

    BQ* getBQ();

};


#endif //UNTITLED_PRODUCER_H
