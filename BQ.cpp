#include "BQ.h"

BQ::BQ (int max_size) {
    this->_max_size = max_size;
}


void BQ::enqueue(string s) {

    this->queue.push_back(s);
    this->_size++;

}

string BQ::dequeue() {
    this->queue.erase(queue.begin());
    this->_size--;
}

int BQ::getSize() const {
    return this->_size;
}

