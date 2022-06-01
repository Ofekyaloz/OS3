#include "BQ.h"

BQ::BQ (int max_size) {
    this->_max_size = max_size;
}


int BQ::enqueue(string s) {
    if(this->_size == this->_max_size) {
        return -1;
    }

    this->queue.push_back(s);
    this->_size++;

    return 1;
}

string BQ::dequeue() {
    if (this->_size > 0) {
        string tmp = this->queue.front();
        this->queue.erase(queue.begin());
        this->_size--;
        return tmp;
    }
    return nullptr;
}

int BQ::getSize() const {
    return this->_size;
}

