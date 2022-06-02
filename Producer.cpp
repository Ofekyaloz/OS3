#include "Producer.h"


Producer::Producer(int id, int products_num, int buf_size) {
    this->_id = id;
    this->_products_number = products_num;
    this->_buf_size = buf_size;
    this->_bq = new BQ(buf_size);
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

