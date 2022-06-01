//
// Created by ofek on 5/30/22.
//

#include "Producer.h"


Producer::Producer(int id, int products_num, int buf_size) {
    this->_id = id;
    this->_products_number = products_num;
    this->_buf_size = buf_size;
    this->_bq = new BQ(buf_size);
}

void* Producer::Produce(void* args) {
    string categories[3] = {"SPORTS", "NEWS", "WEATHER"};
    for (int i = 0; i < _products_number; ++i) {
        string msg = to_string(this->_id) + categories[i % 3] + to_string(this->_products_number);
    }
    return NULL;
}

int Producer::getId() {
    return this->_id;
}

int Producer::getProducts() {
    return this->_products_number;
}

BQ* Producer::getBQ() {
    return this->_bq;
}

Producer::~Producer() {
    free(this->_bq);
}
