//
// Created by ofek on 5/30/22.
//

#include "Producer.h"


Producer::Producer(int id, int products_num, int buf_size) {
    this->_id = id;
    this->_products_number = products_num;
    this->_buf_size = buf_size;
}

void Producer::Produce() {
    string categories[3] = {"SPORTS", "NEWS", "WEATHER"};
    while (_products_number > 0) {
        string msg = to_string(this->_id) + categories[this->_products_number % 3] + to_string(this->_products_number);

    }

}
